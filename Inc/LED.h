#ifndef INC_LED_H_
#define INC_LED_H_

#include "main.h"

/* 방향 지시등은 한 번에 하나만 켜며, 후진 시에는 후방 LED를 사용한다. */
void LED_Left_On(void);
void LED_Right_On(void);
void LED_Back_On(void);
void LED_All_Off(void);

#endif /* INC_LED_H_ */
