/**
 * @file AutoDrive.c
 * @brief 3방향 초음파 센서 융합 기반 자율주행 및 장애물 회피 제어.
 *
 * 전방 거리에 따라 긴급 회피, 코너 탐색, 직진 주행으로 상태를 나누고
 * 좌·우 거리 오차를 P 제어 입력으로 사용해 차동 구동을 수행한다.
 */
#include "AutoDrive.h"
#include "Motor.h"
#include "Sensor.h"    // RS(), distance_*, sensorMutexHandle
#include "cmsis_os.h"

extern volatile uint8_t manual_mode;

void StartDriveTask(void *argument)
{
    /* 다른 주변장치와 센서 Task가 안정화된 뒤 자율주행 판단을 시작한다. */
    osDelay(100);
    /* 반복적으로 같은 방향에 갇히지 않도록 회피 동작 시간을 교대로 변경한다. */
    static uint8_t twist_toggle = 0;

    for (;;)
    {
        if (manual_mode == 0)
        {
            uint16_t distance_Left, distance_Front, distance_Right;
            /* mutex로 보호된 최신 거리값을 한 번에 스냅샷으로 읽는다. */
            RS(&distance_Left, &distance_Front, &distance_Right);

            /* 1. 초비상 대처 — 전방/측면 극근접 */
            if (distance_Front <= 6 || distance_Left <= 5 || distance_Right <= 5)
            {
                Motor_Stop(); osDelay(30);

                Motor_Backward(); Motor_SetSpeed(650, 650);
                osDelay(150 + (twist_toggle * 50));
                Motor_Stop(); osDelay(30);

                /* 후진 후 주변 공간을 다시 측정해 회전 방향을 결정한다. */
                RS(&distance_Left, &distance_Front, &distance_Right);

                if (distance_Left >= 25 || distance_Right >= 25) {
                    if (distance_Left >= distance_Right) {
                        Motor_Left(); Motor_SetSpeed(700, 700);
                    } else {
                        Motor_Right(); Motor_SetSpeed(700, 700);
                    }
                    /* 최소 회전 시간을 확보한 뒤 전방이 열리면 조기에 회전을 끝낸다. */
                    uint32_t turnStart = osKernelGetTickCount();
                    while (osKernelGetTickCount() - turnStart < 650) {
                        if (osKernelGetTickCount() - turnStart < 250) {
                            osDelay(20); continue;
                        }
                        RS(&distance_Left, &distance_Front, &distance_Right);
                        if (distance_Front >= 30) break;
                        osDelay(20);
                    }
                } else {
                    if (distance_Left > distance_Right) {
                        Motor_Left(); Motor_SetSpeed(700, 700);
                    } else {
                        Motor_Right(); Motor_SetSpeed(700, 700);
                    }
                    osDelay(200 + (twist_toggle * 50));
                }

                twist_toggle = !twist_toggle;
                Motor_Stop(); osDelay(30);
                continue;
            }

            /* 2. 신중한 탐색 코너링 */
            else if (distance_Front <= 24)
            {
                Motor_Backward(); Motor_SetSpeed(450, 450);
                osDelay(20);
                Motor_Stop(); osDelay(30);

                RS(&distance_Left, &distance_Front, &distance_Right);

                if (distance_Left > distance_Right + 10) {
                    Motor_Left(); Motor_SetSpeed(650, 650);
                } else if (distance_Right > distance_Left + 10) {
                    Motor_Right(); Motor_SetSpeed(650, 650);
                } else {
                    Motor_Backward(); Motor_SetSpeed(550, 550);
                    osDelay(80);
                    Motor_Stop(); osDelay(30);

                    if (distance_Left > distance_Right) {
                        Motor_Left(); Motor_SetSpeed(700, 700);
                    } else {
                        Motor_Right(); Motor_SetSpeed(700, 700);
                    }
                    osDelay(120);
                    Motor_Stop(); osDelay(30);
                    continue;
                }

                uint32_t turnStart = osKernelGetTickCount();
                while (osKernelGetTickCount() - turnStart < 650)
                {
                    if (osKernelGetTickCount() - turnStart < 200) {
                        osDelay(20); continue;
                    }
                    RS(&distance_Left, &distance_Front, &distance_Right);
                    if (distance_Front >= 35) break;
                    osDelay(20);
                }
                Motor_Stop();
                osDelay(30);
            }

            /* 3. 직진 — P-제어 차동 조향 */
            else
            {
                Motor_Forward();

                uint16_t base_spd = 380;

                if (distance_Front <= 40) {
                    int temp_spd = 280 + (distance_Front - 15) * 3;
                    if (temp_spd > 380)      base_spd = 380;
                    else if (temp_spd < 280) base_spd = 280;
                    else                     base_spd = (uint16_t)temp_spd;
                }

                int diff = (int)distance_Left - (int)distance_Right;

                /* 데드존: 오차 8cm 이내 직진 유지 */
                if (diff > -8 && diff < 8) diff = 0;
                if (diff >  30) diff =  30;
                if (diff < -30) diff = -30;

                int p_gain  = 10;
                int offset  = diff * p_gain;
                /* 좌측 공간이 넓으면 좌측 속도를 낮추고 우측 속도를 높여 좌측으로 보정한다. */
                int left_spd  = base_spd - offset;
                int right_spd = base_spd + offset;
                int min_spd = 280;

                if (distance_Left  <= 6 && distance_Left  < distance_Right - 5) min_spd = 200;
                else if (distance_Right <= 6 && distance_Right < distance_Left  - 5) min_spd = 200;

                if (left_spd  > 650) left_spd  = 650;
                if (left_spd  < min_spd) left_spd  = min_spd;
                if (right_spd > 650) right_spd = 650;
                if (right_spd < min_spd) right_spd = min_spd;

                Motor_SetSpeed((uint16_t)left_spd, (uint16_t)right_spd);
            }
        }
        else
        {
            /* 수동 모드에서는 모터 명령을 변경하지 않고 제어권을 ManualTask에 넘긴다. */
            osDelay(10);
        }

        osDelay(10);
    }
}
