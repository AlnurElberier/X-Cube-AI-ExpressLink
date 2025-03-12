/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file
  * @author  MCD Application Team
  * @version V2.0.0
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#ifndef  __RTE_COMPONENTS_H__
#define  __RTE_COMPONENTS_H__

/* Defines ------------------------------------------------------------------*/
/* ARM.CMSIS-FreeRTOS.10.4.6 */
#define RTE_RTOS_FreeRTOS_CORE /* RTOS FreeRTOS Core */
#define RTE_RTOS_FreeRTOS_CONFIG /* RTOS FreeRTOS Config for FreeRTOS API */
#define RTE_RTOS_FreeRTOS_EVENTGROUPS /* RTOS FreeRTOS Event Groups */
#define RTE_RTOS_FreeRTOS_HEAP_4 /* RTOS FreeRTOS Heap 4 */
/* STMicroelectronics.X-CUBE-MEMS1.9.5.0 */
#define ISM330DHCX_ACCGYR_I2C
#define HTS221_HUMTEMP_I2C

#endif /* __RTE_COMPONENTS_H__ */
