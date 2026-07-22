#ifndef INC_MOTOR_H_
#define INC_MOTOR_H_

#include "main.h"

/* H-Bridge 모터 드라이버에 연결된 좌·우 모터 방향 제어 핀. */
#define MOTOR_GPIO GPIOC

#define MOTOR_LEFT1  GPIO_PIN_0
#define MOTOR_LEFT2  GPIO_PIN_1
#define MOTOR_RIGHT1 GPIO_PIN_2
#define MOTOR_RIGHT2 GPIO_PIN_3

/** 좌·우 모터 PWM을 시작하고 정지 상태로 초기화한다. */
void Motor_Init(void);

/* H-Bridge 입력 조합으로 차량의 진행 방향을 설정한다. */
void Motor_Forward(void);
void Motor_Backward(void);
void Motor_Left(void);
void Motor_Right(void);
void Motor_Stop(void);

/** TIM3 PWM 비교값으로 좌·우 모터 속도를 독립적으로 설정한다. */
void Motor_SetSpeed(uint16_t left_speed, uint16_t right_speed);

#endif
