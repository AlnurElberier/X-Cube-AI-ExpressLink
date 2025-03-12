/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "stm32u5xx_hal.h"

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
#define BOARD "B-U585I-IOT02A"
#define PRIORITY_ExpressLinkTask (tskIDLE_PRIORITY + 4)
#define PRIORITY_MotionSensorPublishTask (tskIDLE_PRIORITY + 2)
#define EXPRESSLINK_UART huart3
#define DEMO_PUB_SUB 0
#define DEMO_OTA 1
#define PRIORITY_ShadowDemoTask (tskIDLE_PRIORITY + 2)
#define CONSOLE_UART huart1
#define SECURE_BOOT 1
#define TOPIC_INDEX_MOTION 3
#define PRIORITY_EnvSensorPublishTask (tskIDLE_PRIORITY + 2)
#define USE_SENSOR 1
#define DEMO_SHADOW 0
#define PRIORITY_MainTask (tskIDLE_PRIORITY + 1)
#define PRIORITY_PubSubTask (tskIDLE_PRIORITY + 3)
#define DEMO_SENSORS_ENV 0
#define PRIORITY_OTATask (tskIDLE_PRIORITY + 3)
#define DEMO_SENSORS_MOTION 0
#define TOPIC_INDEX_PubSub 0
#define TOPIC_INDEX_ENV 2
#define _ExpressLink_EVENT_Pin GPIO_PIN_15
#define _ExpressLink_EVENT_GPIO_Port GPIOD
#define ExpressLink_WAKE_Pin GPIO_PIN_2
#define ExpressLink_WAKE_GPIO_Port GPIOB
#define ExpressLink_RESET_Pin GPIO_PIN_7
#define ExpressLink_RESET_GPIO_Port GPIOE
#define LD2_Pin GPIO_PIN_13
#define LD2_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */

#define DEMO_AI 1
#define TOPIC_INDEX_AI 4
#define PRIORITY_InferencePublishTask (tskIDLE_PRIORITY + 2)

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
