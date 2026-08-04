/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
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
#include "stm32l0xx_hal.h"

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
/* POT_S/POT_L pin names match the PCB labels "S"/"L". */
#define POT_S_Pin GPIO_PIN_1
#define POT_S_GPIO_Port GPIOA
#define POT_L_Pin GPIO_PIN_0
#define POT_L_GPIO_Port GPIOA
#define PWM_OUT1_Pin GPIO_PIN_3
#define PWM_OUT1_GPIO_Port GPIOA
#define PWM_OUT2_Pin GPIO_PIN_2
#define PWM_OUT2_GPIO_Port GPIOA
#define DIP1_Pin GPIO_PIN_7
#define DIP1_GPIO_Port GPIOA
#define DIP2_Pin GPIO_PIN_6
#define DIP2_GPIO_Port GPIOA
#define DIP3_Pin GPIO_PIN_5
#define DIP3_GPIO_Port GPIOA
#define DIP4_Pin GPIO_PIN_4
#define DIP4_GPIO_Port GPIOA
#define SolderPad_Pin GPIO_PIN_0
#define SolderPad_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
