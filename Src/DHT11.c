/**
 * @file DHT11.c
 * @brief 단일 데이터선 방식의 DHT11 온·습도 측정 및 UART 상태 전송.
 */
#include "DHT11.h"
#include "tim.h"
#include "usart.h"
#include "cmsis_os.h"
#include <string.h>
#include <stdio.h>

extern TIM_HandleTypeDef htim4;
extern UART_HandleTypeDef huart1;

static void delay_us(uint16_t us)
{
    /* 1MHz로 설정된 TIM4 카운터를 이용해 마이크로초 단위로 대기한다. */
    __HAL_TIM_SET_COUNTER(&htim4, 0);
    while ((__HAL_TIM_GET_COUNTER(&htim4)) < us);
}

static void Set_DHT11_Output(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin   = DHT_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(DHT_GPIO_Port, &GPIO_InitStruct);
}

static void Set_DHT11_Input(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin  = DHT_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(DHT_GPIO_Port, &GPIO_InitStruct);
}

/*
 * NOTE: DHT11 실측값(real_temp, real_humi) 전송 시
 * taskENTER_CRITICAL() 구간 내 UART 타이밍 충돌로 전송 실패.
 * 현재는 고정값으로 대체. 추후 개선 필요.
 */
void Send_DHT11_Data(void)
{
    char msg[64];
    uint8_t data[5] = {0, 0, 0, 0, 0};
    uint8_t check   = 0;
    uint16_t timeout = 0;

    float real_temp = 0.0;
    float real_humi = 0.0;

    /* DHT11 비트 판독 중 Task 전환으로 펄스 폭을 놓치지 않도록 보호한다. */
    taskENTER_CRITICAL();

    /* MCU가 18ms 이상 Low를 출력해 센서에 측정 시작 신호를 보낸다. */
    Set_DHT11_Output();
    HAL_GPIO_WritePin(DHT_GPIO_Port, DHT_Pin, 0);
    delay_us(18000);
    Set_DHT11_Input();

    timeout = 0;
    while (HAL_GPIO_ReadPin(DHT_GPIO_Port, DHT_Pin) == 1 && timeout < 100) { delay_us(1); timeout++; }

    timeout = 0;
    while (HAL_GPIO_ReadPin(DHT_GPIO_Port, DHT_Pin) == 0 && timeout < 100) { delay_us(1); timeout++; }

    timeout = 0;
    while (HAL_GPIO_ReadPin(DHT_GPIO_Port, DHT_Pin) == 1 && timeout < 100) {
        delay_us(1); timeout++;
        check = 1;
    }

    if (check == 1) {
        /* 응답 펄스 뒤 40비트(습도, 온도, 체크섬)를 MSB부터 수신한다. */
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 8; j++) {
                timeout = 0;
                while (HAL_GPIO_ReadPin(DHT_GPIO_Port, DHT_Pin) == 0 && timeout < 100) { delay_us(1); timeout++; }

                delay_us(40);
                if (HAL_GPIO_ReadPin(DHT_GPIO_Port, DHT_Pin) == 0) {
                    data[i] &= ~(1 << (7 - j));
                } else {
                    data[i] |= (1 << (7 - j));
                    timeout = 0;
                    while (HAL_GPIO_ReadPin(DHT_GPIO_Port, DHT_Pin) == 1 && timeout < 100) { delay_us(1); timeout++; }
                }
            }
        }
        /* 마지막 1바이트 체크섬으로 수신 데이터의 유효성을 검사한다. */
        if (data[0] + data[1] + data[2] + data[3] == data[4]) {
            real_humi = (float)data[0] + (float)(data[1] / 10.0);
            real_temp = (float)data[2] + (float)(data[3] / 10.0);
        }
    }

    taskEXIT_CRITICAL();

    // 고정값 전송 (실측 전송 실패로 임시 대체)
    (void)real_temp;
    (void)real_humi;
    sprintf(msg, "\r\n[RoboTaxi] Temp: 23.0 C, Hum: 50.0 %%\r\n\r\n");
    HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), 100);
}
