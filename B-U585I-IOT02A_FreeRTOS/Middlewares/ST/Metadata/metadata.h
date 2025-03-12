/**
 ******************************************************************************
 * @file           : metadata.h
 * @version        : v 1.0.0
 * @brief          : This file implements metadata configuration
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * ththe "License"; You may not use this file except in compliance with the
 *                             opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __METADATA_H
#define __METADATA_H

#include "main.h"
#include "STMicroelectronics.I-CUBE-ExpressLink_conf.h"

#define METADATA_HEADER                "EXPRESSLINK12" /* This is a Metadata configuration Rev 1.2 for ExpressLink. */

#define METADATA_MAX_LENGTH_HEADER     (16)
#define METADATA_MAX_LENGTH_ENDPOINT   (128)
#define METADATA_MAX_LENGTH_SSID       (32)
#define METADATA_MAX_LENGTH_PSWD       (64)
#define METADATA_MAX_LENGTH_APN        (128)

#define METADATA_SUCCESS               (0)
#define METADATA_ERROR                 (!METADATA_SUCCESS)

#define FLASH_START_ADDRESS            (0x08000000)
#define FLASH_BANK_1_START_ADDRESS     (FLASH_START_ADDRESS)
#define FLASH_BANK_2_START_ADDRESS     (FLASH_START_ADDRESS+FLASH_BANK_SIZE)

#if defined(__STM32H5xx_HAL_H)
  #define METADATA_START_ADDRESS       (0x08000000 + FLASH_BANK_SIZE - (FLASH_SECTOR_SIZE))
  #define METADATA_FLASH_BANK          (FLASH_N_PAGES_PER_BANK - 1)
  #define FLASH_N_PAGES_PER_BANK       (FLASH_BANK_SIZE/FLASH_SECTOR_SIZE)
#elif defined(STM32WBxx_HAL_H)
  #define METADATA_START_ADDRESS       (0x08000000 + (FLASH_BANK_SIZE/2) - (FLASH_PAGE_SIZE))
  #define METADATA_FLASH_BANK          ((FLASH_N_PAGES_PER_BANK/2) - 1)
  #define FLASH_N_PAGES_PER_BANK       (FLASH_BANK_SIZE/FLASH_PAGE_SIZE)
#else
  #define METADATA_START_ADDRESS       (0x08000000 + FLASH_BANK_SIZE - (FLASH_PAGE_SIZE))
  #define METADATA_FLASH_BANK          (FLASH_N_PAGES_PER_BANK - 1)
  #define FLASH_N_PAGES_PER_BANK       (FLASH_BANK_SIZE/FLASH_PAGE_SIZE)
#endif

#if defined(FLASH_TYPEPROGRAM_QUADWORD)/* H5 and U5 */
  #define FLASH_PROGRAM_SIZE           FLASH_TYPEPROGRAM_QUADWORD
  #define FLASH_ADDRESS_INC_SIZE       (16)

  #ifndef FLASH_DBANK_SUPPORT
    #define FLASH_DBANK_SUPPORT
  #endif
#else
  #define FLASH_PROGRAM_SIZE           FLASH_TYPEPROGRAM_DOUBLEWORD
  #define FLASH_ADDRESS_INC_SIZE       (8)
#endif

#if defined(OB_SWAP_BANK_ENABLE) /* H5 */
  #define SWAP_BANK_ENABLE             OB_SWAP_BANK_ENABLE
  #define SWAP_BANK_DISABLE            OB_SWAP_BANK_DISABLE
  #define USER_SWAP_BANK               OB_USER_SWAP_BANK
#elif defined(OB_BFB2_ENABLE) /* L4 and L4+ */
  #define SWAP_BANK_ENABLE             OB_BFB2_ENABLE
  #define SWAP_BANK_DISABLE            OB_BFB2_DISABLE
  #define USER_SWAP_BANK               OB_USER_BFB2
#elif defined(FLASH_DBANK_SUPPORT)
#if defined (STM32G0xx_HAL_H)
  #define SWAP_BANK_ENABLE             OB_USER_DUALBANK_SWAP_DISABLE
  #define SWAP_BANK_DISABLE            OB_USER_DUALBANK_SWAP_ENABLE
  #define USER_SWAP_BANK               OB_USER_BANK_SWAP
  #define OB_DUALBANK_DUAL             OB_USER_DUALBANK_ENABLE
#else
  #define SWAP_BANK_ENABLE             OB_USER_DUALBANK_SWAP_ENABLE
  #define SWAP_BANK_DISABLE            OB_USER_DUALBANK_SWAP_DISABLE
  #define USER_SWAP_BANK               OB_USER_BANK_SWAP
  #define OB_DUALBANK_DUAL             OB_USER_DUALBANK_ENABLE
#endif
#endif

typedef struct metadata_t
{
  char *pENDPOINT;
  char *pWIFI_SSID;
  char *pWIFI_PASSWORD;
  char *pAPN;
}metadata_t;


void     metadata_init(metadata_t *pMetadata);
char     metadata_display_menu(void);
uint32_t metadata_process_command(char command);

#endif /* __METADATA_H */
