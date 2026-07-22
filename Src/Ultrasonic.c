/**
 * @file Ultrasonic.c
 * @brief TIM4 기반 HC-SR04 Trigger/Echo 거리 측정 드라이버.
 */
#include "Ultrasonic.h"
#include "tim.h"

extern TIM_HandleTypeDef htim4;

uint16_t HCSR04_Read(GPIO_TypeDef *trigPort, uint16_t trigPin,
                     GPIO_TypeDef *echoPort, uint16_t echoPin)
{
    uint32_t startTick;

    /* 데이터시트 요구사항에 맞춰 Low 2us 후 High 10us Trigger 펄스를 만든다. */
    HAL_GPIO_WritePin(trigPort, trigPin, GPIO_PIN_RESET);
    __HAL_TIM_SET_COUNTER(&htim4, 0);
    while (__HAL_TIM_GET_COUNTER(&htim4) < 2);

    HAL_GPIO_WritePin(trigPort, trigPin, GPIO_PIN_SET);
    __HAL_TIM_SET_COUNTER(&htim4, 0);
    while (__HAL_TIM_GET_COUNTER(&htim4) < 10);

    HAL_GPIO_WritePin(trigPort, trigPin, GPIO_PIN_RESET);

    /* Echo 상승 대기 시간이 5ms를 넘으면 유효하지 않은 측정으로 처리한다. */
    startTick = HAL_GetTick();
    while (HAL_GPIO_ReadPin(echoPort, echoPin) == GPIO_PIN_RESET) {
        if (HAL_GetTick() - startTick > 5) return 0;
    }

    /* Echo High 폭을 측정하며, 비정상적으로 긴 펄스는 30ms에서 종료한다. */
    __HAL_TIM_SET_COUNTER(&htim4, 0);
    startTick = HAL_GetTick();
    while (HAL_GPIO_ReadPin(echoPort, echoPin) == GPIO_PIN_SET) {
        if (HAL_GetTick() - startTick > 30) return 0;
    }

    /* 왕복 음파 시간(us)을 58로 나눠 편도 거리(cm)로 환산한다. */
    return (uint16_t)(__HAL_TIM_GET_COUNTER(&htim4) / 58);
}
