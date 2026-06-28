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
#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define S_IR_1_Pin GPIO_PIN_0
#define S_IR_1_GPIO_Port GPIOC
#define S_IR_2_Pin GPIO_PIN_1
#define S_IR_2_GPIO_Port GPIOC
#define S_red_Pin GPIO_PIN_2
#define S_red_GPIO_Port GPIOC
#define S_yellow_Pin GPIO_PIN_3
#define S_yellow_GPIO_Port GPIOC
#define N_IR_1_Pin GPIO_PIN_0
#define N_IR_1_GPIO_Port GPIOA
#define N_IR_2_Pin GPIO_PIN_1
#define N_IR_2_GPIO_Port GPIOA
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA
#define N_red_Pin GPIO_PIN_6
#define N_red_GPIO_Port GPIOA
#define N_yellow_Pin GPIO_PIN_7
#define N_yellow_GPIO_Port GPIOA
#define S_green_Pin GPIO_PIN_4
#define S_green_GPIO_Port GPIOC
#define W_IR_1_Pin GPIO_PIN_5
#define W_IR_1_GPIO_Port GPIOC
#define E_IR_1_Pin GPIO_PIN_0
#define E_IR_1_GPIO_Port GPIOB
#define E_IR_2_Pin GPIO_PIN_1
#define E_IR_2_GPIO_Port GPIOB
#define E_red_Pin GPIO_PIN_2
#define E_red_GPIO_Port GPIOB
#define W_IR_2_Pin GPIO_PIN_6
#define W_IR_2_GPIO_Port GPIOC
#define W_red_Pin GPIO_PIN_7
#define W_red_GPIO_Port GPIOC
#define W_yellow_Pin GPIO_PIN_8
#define W_yellow_GPIO_Port GPIOC
#define W_green_Pin GPIO_PIN_9
#define W_green_GPIO_Port GPIOC
#define N_green_Pin GPIO_PIN_8
#define N_green_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define E_yellow_Pin GPIO_PIN_3
#define E_yellow_GPIO_Port GPIOB
#define E_green_Pin GPIO_PIN_4
#define E_green_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
