#ifndef INC_AUTODRIVE_H_
#define INC_AUTODRIVE_H_

#include "main.h"

/**
 * @brief 초음파 센서값을 기반으로 장애물을 회피하며 주행하는 FreeRTOS Task.
 * @param argument CMSIS-RTOS Task 인자(현재 사용하지 않음).
 */
void StartDriveTask(void *argument);

#endif /* INC_AUTODRIVE_H_ */
