/**
 * @file LED.c
 * @brief 수동 주행 방향을 표시하는 좌·우·후방 LED 제어.
 */
#include "LED.h"
#include "gpio.h"

void LED_Left_On(void)
{
    HAL_GPIO_WritePin(LED_LEFT_GPIO_Port,  LED_LEFT_Pin,  GPIO_PIN_SET);
    HAL_GPIO_WritePin(LED_RIGHT_GPIO_Port, LED_RIGHT_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED_BACK_GPIO_Port,  LED_BACK_Pin,  GPIO_PIN_RESET);
}

void LED_Right_On(void)
{
    HAL_GPIO_WritePin(LED_LEFT_GPIO_Port,  LED_LEFT_Pin,  GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED_RIGHT_GPIO_Port, LED_RIGHT_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LED_BACK_GPIO_Port,  LED_BACK_Pin,  GPIO_PIN_RESET);
}

void LED_Back_On(void)
{
    HAL_GPIO_WritePin(LED_LEFT_GPIO_Port,  LED_LEFT_Pin,  GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED_RIGHT_GPIO_Port, LED_RIGHT_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED_BACK_GPIO_Port,  LED_BACK_Pin,  GPIO_PIN_SET);
}

void LED_All_Off(void)
{
    HAL_GPIO_WritePin(LED_LEFT_GPIO_Port,  LED_LEFT_Pin,  GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED_RIGHT_GPIO_Port, LED_RIGHT_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED_BACK_GPIO_Port,  LED_BACK_Pin,  GPIO_PIN_RESET);
}
