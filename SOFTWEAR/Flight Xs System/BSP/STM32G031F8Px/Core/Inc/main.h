/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
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
#include "stm32g0xx_hal.h"

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

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define MPU6050_SCL_Pin GPIO_PIN_8
#define MPU6050_SCL_GPIO_Port GPIOB
#define MPU6050_SDA_Pin GPIO_PIN_9
#define MPU6050_SDA_GPIO_Port GPIOB
#define KEY_Pin GPIO_PIN_15
#define KEY_GPIO_Port GPIOC
#define MOTOR1_Pin GPIO_PIN_0
#define MOTOR1_GPIO_Port GPIOA
#define MOTOR2_Pin GPIO_PIN_1
#define MOTOR2_GPIO_Port GPIOA
#define BT_TX_Pin GPIO_PIN_2
#define BT_TX_GPIO_Port GPIOA
#define BT_RX_Pin GPIO_PIN_3
#define BT_RX_GPIO_Port GPIOA
#define DEBUG_LED_Pin GPIO_PIN_4
#define DEBUG_LED_GPIO_Port GPIOA
#define STATUS_LED_Pin GPIO_PIN_5
#define STATUS_LED_GPIO_Port GPIOA
#define MOTOR3_Pin GPIO_PIN_6
#define MOTOR3_GPIO_Port GPIOA
#define MOTOR4_Pin GPIO_PIN_7
#define MOTOR4_GPIO_Port GPIOA
#define BAT_ADC_Pin GPIO_PIN_2
#define BAT_ADC_GPIO_Port GPIOB
#define SPL06__SCL_Pin GPIO_PIN_11
#define SPL06__SCL_GPIO_Port GPIOA
#define SPL06_SDA_Pin GPIO_PIN_12
#define SPL06_SDA_GPIO_Port GPIOA
#define DEBUG_TX_Pin GPIO_PIN_6
#define DEBUG_TX_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
