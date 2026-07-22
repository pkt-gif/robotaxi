/**
 * @file ManualControl.c
 * @brief 블루투스 명령 기반 수동주행, 모드 전환 및 승객 탑승 처리.
 *
 * 명령: G=자율주행 시작, M=모드 전환, F/B/L/R=방향 제어,
 * S=정지, T=자율주행 중 강제 수동 전환.
 */
#include "ManualControl.h"
#include "Motor.h"
#include "LED.h"
#include "Buzzer.h"
#include "DHT11.h"
#include "Sensor.h"    // passenger_loaded
#include "gpio.h"
#include "tim.h"
#include "cmsis_os.h"

extern volatile uint8_t manual_mode;
extern volatile uint8_t bt_data;
extern TIM_HandleTypeDef htim2;

void StartManualTask(void *argument)
{
    uint8_t cmd = 0;

    for (;;)
    {
        if (bt_data != 0)
        {
            /* 공유 수신 버퍼에서 명령을 가져온 뒤 중복 실행을 막기 위해 비운다. */
            cmd = bt_data;
            bt_data = 0;

            switch (cmd)
            {
                case 'G': case 'g':
                    /* 대기 중인 수동 모드에서 즉시 자율주행을 시작한다. */
                    if (manual_mode == 1) {
                        manual_mode = 0;
                        passenger_loaded = 0;
                        Send_DHT11_Data();
                        Buzzer_Beep(2, 100, 1000);
                    }
                    break;

                case 'M': case 'm':
                    /* 수동 상태에서는 탑승 확인 후 출발하고, 자율 상태에서는 정차한다. */
                    if (manual_mode == 1) {
                        if (passenger_loaded == 1 ||
                            HAL_GPIO_ReadPin(IR_SENSOR_GPIO_Port, IR_SENSOR_Pin) == GPIO_PIN_RESET) {
                            manual_mode = 0;
                            passenger_loaded = 0;
                            Send_DHT11_Data();
                            Buzzer_Beep(2, 100, 1000);
                        } else {
                            /* 승객이 감지되지 않으면 경고음을 내고 수동 모드를 유지한다. */
                            Buzzer_Beep(3, 50, 2000);
                        }
                    } else {
                        manual_mode = 1;
                        Motor_Stop();
                        Motor_SetSpeed(0, 0);
                        HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_1);
                        passenger_loaded = 0;
                        Send_DHT11_Data();
                        Buzzer_Beep(1, 500, 1000);
                    }
                    break;

                case 'F': case 'f':
                    if (manual_mode == 1) {
                        Motor_Forward();
                        Motor_SetSpeed(380, 380);
                        LED_All_Off();
                        HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_1);
                    }
                    break;

                case 'B': case 'b':
                    if (manual_mode == 1) {
                        Motor_Backward();
                        Motor_SetSpeed(380, 380);
                        LED_Back_On();
                        /* 후진 중에는 TIM2 PWM을 연속 출력해 경고음을 발생시킨다. */
                        TIM2->PSC  = 255;
                        TIM2->ARR  = 1000;
                        TIM2->CCR1 = 1000;
                        HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
                    }
                    break;

                case 'L': case 'l':
                    if (manual_mode == 1) {
                        Motor_Left();
                        Motor_SetSpeed(380, 380);
                        LED_Left_On();
                        HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_1);
                    }
                    break;

                case 'R': case 'r':
                    if (manual_mode == 1) {
                        Motor_Right();
                        Motor_SetSpeed(380, 380);
                        LED_Right_On();
                        HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_1);
                    }
                    break;

                case 'S': case 's':
                    if (manual_mode == 1) {
                        Motor_Stop();
                        Motor_SetSpeed(0, 0);
                        LED_All_Off();
                        HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_1);
                    }
                    break;

                case 'T': case 't':
                    /* 자율주행을 즉시 중단하고 안전하게 수동 정지 상태로 전환한다. */
                    if (manual_mode == 0) {
                        manual_mode = 1;
                        Motor_Stop();
                        Motor_SetSpeed(0, 0);
                        HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_1);
                        passenger_loaded = 0;
                        Send_DHT11_Data();
                        Buzzer_Beep(1, 200, 2500);
                    }
                    break;
            }
        }
        osDelay(10);
    }
}
