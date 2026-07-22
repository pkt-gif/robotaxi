/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "cmsis_os.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
//extern osMessageQueueId_t btQueueHandle;
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED_BACK_Pin GPIO_PIN_13
#define LED_BACK_GPIO_Port GPIOC
#define motor_IN1_Pin GPIO_PIN_0
#define motor_IN1_GPIO_Port GPIOC
#define motor_IN2_Pin GPIO_PIN_1
#define motor_IN2_GPIO_Port GPIOC
#define motor_IN3_Pin GPIO_PIN_2
#define motor_IN3_GPIO_Port GPIOC
#define motor_IN4_Pin GPIO_PIN_3
#define motor_IN4_GPIO_Port GPIOC
#define Buzzer_Pin GPIO_PIN_0
#define Buzzer_GPIO_Port GPIOA
#define DHT_Pin GPIO_PIN_1
#define DHT_GPIO_Port GPIOA
#define LEFT_TRIG_Pin GPIO_PIN_12
#define LEFT_TRIG_GPIO_Port GPIOB
#define LEFT_ECHO_Pin GPIO_PIN_13
#define LEFT_ECHO_GPIO_Port GPIOB
#define FRONT_TRIG_Pin GPIO_PIN_14
#define FRONT_TRIG_GPIO_Port GPIOB
#define RIGHT_ECHO_Pin GPIO_PIN_15
#define RIGHT_ECHO_GPIO_Port GPIOB
#define LED_LEFT_Pin GPIO_PIN_8
#define LED_LEFT_GPIO_Port GPIOC
#define LED_RIGHT_Pin GPIO_PIN_9
#define LED_RIGHT_GPIO_Port GPIOC
#define RIGHT_TRIG_Pin GPIO_PIN_8
#define RIGHT_TRIG_GPIO_Port GPIOA
#define FRONT_ECHO_Pin GPIO_PIN_9
#define FRONT_ECHO_GPIO_Port GPIOA
#define IR_SENSOR_Pin GPIO_PIN_12
#define IR_SENSOR_GPIO_Port GPIOC

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
