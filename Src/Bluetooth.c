/**
 * @file Bluetooth.c
 * @brief USART1 인터럽트 기반 블루투스 명령 수신 및 오류 복구.
 */
#include "Bluetooth.h"
#include "usart.h"
#include "gpio.h"

volatile uint8_t bt_data = 0;

void Bluetooth_Init(void)
{
    /* 이전 Overrun 상태를 제거한 뒤 첫 1바이트 비동기 수신을 등록한다. */
    __HAL_UART_CLEAR_OREFLAG(&huart1);
    HAL_UART_Receive_IT(&huart1, (uint8_t*)&bt_data, 1);
}

/* 정상 수신 콜백 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1) {
        /* 수신 확인용 보드 LED를 토글하고 다음 명령 수신을 즉시 재등록한다. */
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
        HAL_UART_Receive_IT(&huart1, (uint8_t*)&bt_data, 1);
    }
}

/* ORE 에러 자동 복구 콜백 */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1) {
        /* Overrun/Noise/Frame 오류 플래그를 지워 수신 중단 상태를 자동 복구한다. */
        __HAL_UART_CLEAR_OREFLAG(huart);
        __HAL_UART_CLEAR_NEFLAG(huart);
        __HAL_UART_CLEAR_FEFLAG(huart);
        HAL_UART_Receive_IT(&huart1, (uint8_t*)&bt_data, 1);
    }
}
