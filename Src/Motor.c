/**
 * @file Motor.c
 * @brief H-Bridge 방향 신호와 TIM3 PWM을 이용한 차동 구동 모터 제어.
 */
#include <Motor.h>
#include "tim.h"

extern TIM_HandleTypeDef htim3;

void Motor_Init(void)
{
    /* TIM3 CH1/CH2를 각각 좌·우 모터의 속도 제어 PWM으로 사용한다. */
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);

    Motor_Stop();
}

void Motor_Stop(void)
{
    /* 네 방향 핀과 PWM 출력을 모두 0으로 만들어 모터를 정지한다. */
    HAL_GPIO_WritePin(MOTOR_GPIO, MOTOR_LEFT1, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOTOR_GPIO, MOTOR_LEFT2, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOTOR_GPIO, MOTOR_RIGHT1, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOTOR_GPIO, MOTOR_RIGHT2, GPIO_PIN_RESET);

    Motor_SetSpeed(0, 0);
}

void Motor_Forward(void)
{
    HAL_GPIO_WritePin(MOTOR_GPIO, MOTOR_LEFT1, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOTOR_GPIO, MOTOR_LEFT2, GPIO_PIN_SET);

    HAL_GPIO_WritePin(MOTOR_GPIO, MOTOR_RIGHT1, GPIO_PIN_SET);
    HAL_GPIO_WritePin(MOTOR_GPIO, MOTOR_RIGHT2, GPIO_PIN_RESET);
}

void Motor_Backward(void)
{
    HAL_GPIO_WritePin(MOTOR_GPIO, MOTOR_LEFT1, GPIO_PIN_SET);
    HAL_GPIO_WritePin(MOTOR_GPIO, MOTOR_LEFT2, GPIO_PIN_RESET);

    HAL_GPIO_WritePin(MOTOR_GPIO, MOTOR_RIGHT1, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOTOR_GPIO, MOTOR_RIGHT2, GPIO_PIN_SET);
}

void Motor_Left(void)
{
    // 왼쪽 바퀴 후진
    HAL_GPIO_WritePin(MOTOR_GPIO, MOTOR_LEFT1, GPIO_PIN_SET);
    HAL_GPIO_WritePin(MOTOR_GPIO, MOTOR_LEFT2, GPIO_PIN_RESET);

    // 오른쪽 바퀴 전진
    HAL_GPIO_WritePin(MOTOR_GPIO, MOTOR_RIGHT1, GPIO_PIN_SET);
    HAL_GPIO_WritePin(MOTOR_GPIO, MOTOR_RIGHT2, GPIO_PIN_RESET);
}

void Motor_Right(void)
{
    // 왼쪽 바퀴 전진
    HAL_GPIO_WritePin(MOTOR_GPIO, MOTOR_LEFT1, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOTOR_GPIO, MOTOR_LEFT2, GPIO_PIN_SET);

    // 오른쪽 바퀴 후진
    HAL_GPIO_WritePin(MOTOR_GPIO, MOTOR_RIGHT1, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOTOR_GPIO, MOTOR_RIGHT2, GPIO_PIN_SET);
}

void Motor_SetSpeed(uint16_t left_speed, uint16_t right_speed)
{
    /* 비교값이 클수록 각 모터에 인가되는 PWM 듀티비가 증가한다. */
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, left_speed);
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, right_speed);
}
