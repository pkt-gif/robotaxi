/**
 * @file Sensor.c
 * @brief 초음파 3방향 거리 수집, 공유 데이터 보호 및 승객 감지.
 */
#include "Sensor.h"
#include "Ultrasonic.h"
#include "Buzzer.h"
#include "gpio.h"
#include "cmsis_os.h"

extern volatile uint8_t manual_mode;
extern osMutexId_t sensorMutexHandle;

volatile uint16_t distance_Left  = 0;
volatile uint16_t distance_Front = 0;
volatile uint16_t distance_Right = 0;
volatile uint8_t  passenger_loaded = 0;

void RS(uint16_t *l, uint16_t *f, uint16_t *r)
{
    /* 타임아웃(0cm) 발생 시 사용할 마지막 정상 측정값. */
    static uint16_t last_l = 60, last_f = 60, last_r = 60;
    uint16_t raw_l, raw_f, raw_r;

    /* SensorTask가 갱신하는 세 값을 동일한 측정 시점의 스냅샷으로 복사한다. */
    osMutexAcquire(sensorMutexHandle, osWaitForever);
    raw_l = distance_Left;
    raw_f = distance_Front;
    raw_r = distance_Right;
    osMutexRelease(sensorMutexHandle);

    if (raw_l > 0) last_l = raw_l;
    if (raw_f > 0) last_f = raw_f;
    if (raw_r > 0) last_r = raw_r;

    /* 측면 센서는 조향 판단 범위인 60cm까지만 사용한다. */
    *l = (last_l > 60) ? 60 : last_l;
    *r = (last_r > 60) ? 60 : last_r;
    *f = last_f;
}

void StartSensorTask(void *argument)
{
    static GPIO_PinState last_ir_state = GPIO_PIN_SET;

    for (;;)
    {
        /* 센서 간 초음파 간섭을 줄이기 위해 순차 측정 사이에 간격을 둔다. */
        uint16_t l = HCSR04_Read(LEFT_TRIG_GPIO_Port,  LEFT_TRIG_Pin,  LEFT_ECHO_GPIO_Port,  LEFT_ECHO_Pin);
        osDelay(12);
        uint16_t f = HCSR04_Read(FRONT_TRIG_GPIO_Port, FRONT_TRIG_Pin, FRONT_ECHO_GPIO_Port, FRONT_ECHO_Pin);
        osDelay(12);
        uint16_t r = HCSR04_Read(RIGHT_TRIG_GPIO_Port, RIGHT_TRIG_Pin, RIGHT_ECHO_GPIO_Port, RIGHT_ECHO_Pin);
        osDelay(12);

        /* 자율주행 Task가 읽는 공유 거리값을 한 번의 임계구역에서 갱신한다. */
        osMutexAcquire(sensorMutexHandle, osWaitForever);
        distance_Left  = l;
        distance_Front = f;
        distance_Right = r;
        osMutexRelease(sensorMutexHandle);

        GPIO_PinState current_ir = HAL_GPIO_ReadPin(IR_SENSOR_GPIO_Port, IR_SENSOR_Pin);

        if (manual_mode == 1) {
            /* IR 입력의 High->Low 에지에서 탑승 이벤트를 한 번만 등록한다. */
            if (last_ir_state == GPIO_PIN_SET && current_ir == GPIO_PIN_RESET) {
                if (passenger_loaded == 0) {
                    passenger_loaded = 1;
                    Buzzer_Beep(1, 100, 1500);
                }
            }
        }
        last_ir_state = current_ir;
    }
}
