#ifndef INC_MANUALCONTROL_H_
#define INC_MANUALCONTROL_H_

#include "main.h"

/**
 * @brief 블루투스 명령을 해석해 주행 모드와 차량 구동 장치를 제어하는 Task.
 * @param argument CMSIS-RTOS Task 인자(현재 사용하지 않음).
 */
void StartManualTask(void *argument);

#endif /* INC_MANUALCONTROL_H_ */
