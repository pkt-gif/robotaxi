#ifndef INC_BLUETOOTH_H_
#define INC_BLUETOOTH_H_

#include "main.h"

/* UART 수신 인터럽트에서 갱신되는 1바이트 블루투스 명령. */
extern volatile uint8_t bt_data;

/** USART1의 1바이트 인터럽트 수신을 시작한다. */
void Bluetooth_Init(void);

#endif /* INC_BLUETOOTH_H_ */
