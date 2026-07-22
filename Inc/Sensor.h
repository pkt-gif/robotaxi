#ifndef INC_SENSOR_H_
#define INC_SENSOR_H_

#include "main.h"
#include "cmsis_os.h"

extern volatile uint16_t distance_Left;
extern volatile uint16_t distance_Front;
extern volatile uint16_t distance_Right;
extern volatile uint8_t  passenger_loaded;
extern osMutexId_t       sensorMutexHandle;  // freertos.c에서 생성, Sensor.c에서 사용

/**
 * @brief 공유 초음파 거리값을 안전하게 읽고 유효값 필터링을 적용한다.
 * @details 0cm 측정값은 직전 정상값으로 대체하고, 좌·우 거리는 최대 60cm로 제한한다.
 */
void RS(uint16_t *l, uint16_t *f, uint16_t *r);

/** 초음파 3개와 승객 감지 IR 센서를 주기적으로 갱신하는 Task. */
void StartSensorTask(void *argument);

#endif /* INC_SENSOR_H_ */
