/* USER CODE BEGIN Header */
/**
******************************************************************************
* @file           : compute_inference.h
* @brief          : X-CUBE-AI compute inference task
******************************************************************************
* @attention
*
* Copyright (c) 2022 STMicroelectronics.
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
#ifndef __COMPUTE_INFERENCE_H__
#define __COMPUTE_INFERENCE_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/


/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/


/* Exported functions prototypes ---------------------------------------------*/

void vInferencePublishTask(void *argument);

#ifdef __cplusplus
}
#endif

#endif /* __ENV_SENSOR_PUBLISH_H__ */
