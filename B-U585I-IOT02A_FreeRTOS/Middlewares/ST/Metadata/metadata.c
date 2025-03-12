/**
 ******************************************************************************
 * @file           : metadata.c
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
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "metadata.h"
#include "ExpressLink.h"
#include "FreeRTOS.h"
#include "task.h"

#define DEBUG_LEVEL         0

#define ADDRESS_HEADER      0
#define ADDRESS_ENDPOINT    (ADDRESS_HEADER      + METADATA_MAX_LENGTH_HEADER   )
#define ADDRESS_SSID        (ADDRESS_ENDPOINT    + METADATA_MAX_LENGTH_ENDPOINT )
#define ADDRESS_PSWD        (ADDRESS_SSID        + METADATA_MAX_LENGTH_SSID     )
#define ADDRESS_APN         (ADDRESS_PSWD        + METADATA_MAX_LENGTH_PSWD     )

#define BUFFER_SIZE         (METADATA_MAX_LENGTH_HEADER   + \
                             METADATA_MAX_LENGTH_ENDPOINT + \
                             METADATA_MAX_LENGTH_SSID     + \
                             METADATA_MAX_LENGTH_PSWD     + \
                             METADATA_MAX_LENGTH_APN      + 1)

#define MODIFY_ENDOINT  '0'
#define MODIFY_SSID     '1'
#define MODIFY_PASSWORD '2'
#define MODIFY_APN      '3'
#define STORAGE_WRITE   '4'
#define STORAGE_ERASE   '5'
#define SET_DEFAULT     '6'
#define GET_CERT        '7'
#define GET_THING_NAME  '8'
#define DEVICE_RESET    '9'
#define PASS_THROUGH    'A'
#define pass_through    'a'

static uint32_t metadata_get_data(metadata_t *pMetadata);
static uint32_t metadata_set_data(void);
static uint32_t metadata_erase_data(void);
static uint32_t metadata_set_default(void);

static char metadata_buffer[BUFFER_SIZE] = { 0 };

static char *header   = &metadata_buffer[ADDRESS_HEADER  ];
static char *endpoint = &metadata_buffer[ADDRESS_ENDPOINT];
static char *ssid     = &metadata_buffer[ADDRESS_SSID    ];
static char *pswd     = &metadata_buffer[ADDRESS_PSWD    ];
static char *apn      = &metadata_buffer[ADDRESS_APN     ];

extern UART_HandleTypeDef CONSOLE_UART;
extern UART_HandleTypeDef EXPRESSLINK_UART;

static void strip(char *s);

/**
 * @brief  Ask user if they want to change config
 * @param  none
 * @retval none
 */
void metadata_init(metadata_t *pMetadata)
{
  setvbuf(stdin, NULL, _IONBF, 0);

  if (metadata_get_data(pMetadata) != METADATA_SUCCESS)
  {
#if (DEBUG_LEVEL > 0)
	  printf("header error\r\n");
#endif
    metadata_process_command(SET_DEFAULT);
    metadata_get_data(pMetadata);
  }
}

/**
 * @brief  Display the menu
 * @param  none
 * @retval menu selection
 */
char metadata_display_menu(void)
{
  volatile char choice = '\0';

  printf("\r\n%c - Modify Endpoint", MODIFY_ENDOINT);
  printf("\r\n%c - Modify Wi-Fi SSID", MODIFY_SSID);
  printf("\r\n%c - Modify Wi-Fi Password", MODIFY_PASSWORD);
  printf("\r\n%c - Modify APN", MODIFY_APN);
  printf("\r\n%c - Write metadate to NVM", STORAGE_WRITE);
  printf("\r\n%c - Erase metadata", STORAGE_ERASE);
  printf("\r\n%c - Set default", SET_DEFAULT);
  printf("\r\n%c - Get device certificate", GET_CERT);
  printf("\r\n%c - Get device ID", GET_THING_NAME);
  printf("\r\n%c - Reset the device", DEVICE_RESET);
  printf("\r\n%c - PassThrough", PASS_THROUGH);
  printf("\r\n");

  choice = getchar();

  return choice;
}

/**
 * @brief  Process the commands
 * @param  none
 * @retval status
 */
uint32_t metadata_process_command(char command)
{
  switch (command)
  {
  case MODIFY_ENDOINT:
    printf("\r\n%s\r\n", "Enter Endpoint:");
    fgets(endpoint, METADATA_MAX_LENGTH_ENDPOINT, stdin);
    strip(endpoint);
    printf("Endpoint set to: %s\r\n", endpoint);
    break;

  case MODIFY_SSID:
    printf("\r\n%s\r\n", "Enter Wi-Fi SSID:");
    fgets(ssid, METADATA_MAX_LENGTH_SSID, stdin);
    strip(ssid);
    printf("WiFi SSID set to: %s\r\n", ssid);
    break;

  case MODIFY_PASSWORD:
    printf("\r\n%s\r\n", "Enter Wi-Fi PSWD:");
    fgets(pswd, METADATA_MAX_LENGTH_PSWD, stdin);
    strip(pswd);
    printf("WiFi Password set to: %s\r\n", pswd);
    break;

  case MODIFY_APN:
	printf("\r\n%s\r\n", "APN:");
	fgets(apn, METADATA_MAX_LENGTH_APN, stdin);
	strip(apn);
	printf("APN set to: %s\r\n", apn);
	break;

  case STORAGE_WRITE:
    printf("Writing metadata to flash\r\n");
    snprintf(header, METADATA_MAX_LENGTH_HEADER, "%s", METADATA_HEADER);
    metadata_erase_data();
    metadata_set_data();
    break;

  case STORAGE_ERASE:
    printf("Erasing storage\r\n");
    metadata_erase_data();
    break;

  case SET_DEFAULT:
    printf("Default settings\r\n");
    metadata_set_default();
    break;

  case GET_CERT:
  {
    char *response;

    uint32_t str_len = 0;
    uint32_t i = 0;

    char *str = NULL;

    response = ExpressLink_GetCertificate();
    str = (char*) &response[3];

    str_len = strlen(str);

    for (i = 0; i < str_len; i++)
    {
      if (str[i] != '\\')
      {
        printf("%c", str[i]);
      }
      else if (str[i + 1] != 'A')
      {
        printf("%c", str[i]);
      }
      else
      {
        printf("\r\n");
        i++;
      }
    }
  }
    break;

  case GET_THING_NAME:
  {
    char *response;
    response = ExpressLink_GetThingName();
    printf("ThingName %s", response);
  }
    break;

  case DEVICE_RESET:
    NVIC_SystemReset();
    break;

  case pass_through:
  case PASS_THROUGH:
	  printf("\r\n\n---- PassThrough mode. Push reset button to exit. ----\r\n\n");
	  HAL_UART_Receive_DMA(&CONSOLE_UART    , ( uint8_t *)&EXPRESSLINK_UART.Instance->TDR, 1);
	  HAL_UART_Receive_DMA(&EXPRESSLINK_UART, ( uint8_t *)&CONSOLE_UART.Instance->TDR    , 1);
	  HAL_SuspendTick();
	  vTaskSuspendAll();
	  while(1);
	  break;

  default:
    printf("%c , %s\r\n", command, "Error Re-Enter Number");
  }

  return METADATA_SUCCESS;
}

static uint32_t metadata_set_data(void)
{
#if defined(HAL_ICACHE_MODULE_ENABLED)
  HAL_ICACHE_Disable();
#endif

  uint32_t status = METADATA_SUCCESS;
  uint32_t address = METADATA_START_ADDRESS;

#if defined(__STM32H5xx_HAL_H) || defined(STM32L5xx_HAL_H)
  uint32_t saved_flash_latency = __HAL_FLASH_GET_LATENCY();
  __HAL_FLASH_SET_LATENCY(__HAL_FLASH_GET_LATENCY() +2);
  (void)__HAL_FLASH_GET_LATENCY();
#endif

  HAL_FLASH_Unlock();

#if (FLASH_ADDRESS_INC_SIZE == 8)
  uint64_t *x = (uint64_t*) metadata_buffer;
#endif

  for (int i = 0; i < BUFFER_SIZE; i += FLASH_ADDRESS_INC_SIZE)
  {
#if (FLASH_ADDRESS_INC_SIZE == 8)
#if (DEBUG_LEVEL > 0)
	printf("Writing to BANK1 address : 0x%08X\r\n", (int)address + i);
#endif

    if(HAL_FLASH_Program(FLASH_PROGRAM_SIZE, address + i, x[i/FLASH_ADDRESS_INC_SIZE]) != HAL_OK)
    {
      status = METADATA_ERROR;
    }

#if defined(FLASH_BANK_2)
#if (DEBUG_LEVEL > 0)
    printf("Writing to BANK2 address : 0x%08X\r\n", (int)(address + i + FLASH_BANK_SIZE));
#endif

    if(HAL_FLASH_Program(FLASH_PROGRAM_SIZE, address + i + FLASH_BANK_SIZE, x[i/FLASH_ADDRESS_INC_SIZE]) != HAL_OK)
    {
      status = METADATA_ERROR;
    }
#endif /* FLASH_BANK_2 */
#else  /* FLASH_ADDRESS_INC_SIZE */

#if (DEBUG_LEVEL > 0)
	printf("Writing to BANK1 address : 0x%08X\r\n", (int)address + i);
#endif
    if (HAL_FLASH_Program(FLASH_PROGRAM_SIZE, address + i, (uint32_t) metadata_buffer + i) != HAL_OK)
    {
      status = METADATA_ERROR;
    }
#if defined(FLASH_BANK_2)
#if (DEBUG_LEVEL > 0)
    printf("Writing to BANK2 address : 0x%08X\r\n", (int)(address + i + FLASH_BANK_SIZE));
#endif

    if (HAL_FLASH_Program(FLASH_PROGRAM_SIZE, address + i + FLASH_BANK_SIZE, (uint32_t) metadata_buffer + i) != HAL_OK)
    {
      status = METADATA_ERROR;
    }
#endif /* FLASH_BANK_2 */
#endif /* FLASH_ADDRESS_INC_SIZE */
  }

  HAL_FLASH_Lock();

#if defined(__STM32H5xx_HAL_H) || defined(STM32L5xx_HAL_H)
  __HAL_FLASH_SET_LATENCY(saved_flash_latency);
  (void)__HAL_FLASH_GET_LATENCY();;
#endif

#if defined(HAL_ICACHE_MODULE_ENABLED)
  HAL_ICACHE_Enable();
#endif

  return status;
}

static uint32_t metadata_erase_data(void)
{
  FLASH_EraseInitTypeDef EraseInitStruct;
  uint32_t SectorError = 0;
  uint32_t status = METADATA_SUCCESS;
  FLASH_OBProgramInitTypeDef OBInit;

#if defined(HAL_ICACHE_MODULE_ENABLED)
  HAL_ICACHE_Disable();
#endif

#if defined(__STM32H5xx_HAL_H) || defined(STM32L5xx_HAL_H)
  uint32_t saved_flash_latency = __HAL_FLASH_GET_LATENCY();
  __HAL_FLASH_SET_LATENCY(__HAL_FLASH_GET_LATENCY() +2);
  (void)__HAL_FLASH_GET_LATENCY();
#endif

  HAL_FLASH_Unlock();

  HAL_FLASHEx_OBGetConfig(&OBInit);

#if defined(__STM32H5xx_HAL_H)
  EraseInitStruct.Sector     = METADATA_FLASH_BANK;
  EraseInitStruct.TypeErase  = FLASH_TYPEERASE_SECTORS;
  EraseInitStruct.NbSectors    = 1;

#if (DEBUG_LEVEL > 0)
  printf("Erasing page number %d\r\n", (int)EraseInitStruct.NbSectors);
#endif

#else /* __STM32H5xx_HAL_H */
  EraseInitStruct.Page = METADATA_FLASH_BANK;
  EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
  EraseInitStruct.NbPages = 1;

#if (DEBUG_LEVEL > 0)
  printf("Erasing page number %d\r\n", (int)EraseInitStruct.Page);
#endif
#endif /* __STM32H5xx_HAL_H */

#if defined(FLASH_BANK_1)
  EraseInitStruct.Banks = FLASH_BANK_1;
#endif

  if (HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError) != HAL_OK)
  {
    status = METADATA_ERROR;
  }
#if defined(FLASH_BANK_2)
  EraseInitStruct.Banks = FLASH_BANK_2;

  if (HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError) != HAL_OK)
  {
    status = METADATA_ERROR;
  }
#endif

  HAL_FLASH_Lock();

#if defined(__STM32H5xx_HAL_H) || defined(STM32L5xx_HAL_H)
  __HAL_FLASH_SET_LATENCY(saved_flash_latency);
  (void)__HAL_FLASH_GET_LATENCY();;
#endif

#if defined(HAL_ICACHE_MODULE_ENABLED)
  HAL_ICACHE_Enable();
#endif

  return status;
}

static uint32_t metadata_get_data(metadata_t *pMetadata)
{
#if defined(HAL_ICACHE_MODULE_ENABLED)
  HAL_ICACHE_Disable();
#endif

  char *x = (char*) METADATA_START_ADDRESS;

  for (int i = 0; i < BUFFER_SIZE; i++)
  {
    metadata_buffer[i] = *(x++);
  }

  metadata_buffer[BUFFER_SIZE - 1] = '\0';

  if (strcmp(header, METADATA_HEADER) != 0)
  {
    return METADATA_ERROR;
  }

  pMetadata->pENDPOINT = endpoint;
  pMetadata->pWIFI_SSID = ssid;
  pMetadata->pWIFI_PASSWORD = pswd;
  pMetadata->pAPN = apn;

#if defined(HAL_ICACHE_MODULE_ENABLED)
  HAL_ICACHE_Enable();
#endif

  return METADATA_SUCCESS;
}

/**
 * @brief  Set the metadata to default. This will not update the EEPROM
 * @param  none 
 * @retval status
 */
static uint32_t metadata_set_default(void)
{
  snprintf(header, METADATA_MAX_LENGTH_HEADER, "%s", METADATA_HEADER);
  snprintf(ssid, METADATA_MAX_LENGTH_SSID, "%s", EXPRESSLINK_WIFI_SSID);
  snprintf(pswd, METADATA_MAX_LENGTH_PSWD, "%s", EXPRESSLINK_WIFI_PASSWORD);
  snprintf(endpoint, METADATA_MAX_LENGTH_ENDPOINT, "%s", EXPRESSLINK_AWS_IOT_ENDPOINT);
  snprintf(apn, METADATA_MAX_LENGTH_APN, "%s", EXPRESSLINK_APN);

  metadata_process_command(STORAGE_WRITE);

  return METADATA_SUCCESS;
}

static void strip(char *s)
{
  if (s[strlen(s) - 2] == '\r')
  {
    s[strlen(s) - 2] = '\0';
  }
  else
  {
    s[strlen(s) - 1] = '\0';
  }
}
