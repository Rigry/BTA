/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define FBPN_Pin GPIO_PIN_1
#define FBPN_GPIO_Port GPIOB
#define SPN_Pin GPIO_PIN_2
#define SPN_GPIO_Port GPIOB
#define SPC_Pin GPIO_PIN_10
#define SPC_GPIO_Port GPIOB
#define FBPC_Pin GPIO_PIN_11
#define FBPC_GPIO_Port GPIOB
#define OUT4_Pin GPIO_PIN_12
#define OUT4_GPIO_Port GPIOB
#define OUT3_Pin GPIO_PIN_13
#define OUT3_GPIO_Port GPIOB
#define OUT2_Pin GPIO_PIN_14
#define OUT2_GPIO_Port GPIOB
#define OUT1_Pin GPIO_PIN_15
#define OUT1_GPIO_Port GPIOB
#define Level_Pin GPIO_PIN_6
#define Level_GPIO_Port GPIOC
#define IN3_Pin GPIO_PIN_7
#define IN3_GPIO_Port GPIOC
#define IN2_Pin GPIO_PIN_8
#define IN2_GPIO_Port GPIOC
#define IN1_Pin GPIO_PIN_9
#define IN1_GPIO_Port GPIOC
#define LED_CAN_Pin GPIO_PIN_12
#define LED_CAN_GPIO_Port GPIOC
#define LED_GREEN_Pin GPIO_PIN_2
#define LED_GREEN_GPIO_Port GPIOD
#define LED_RED_Pin GPIO_PIN_3
#define LED_RED_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
