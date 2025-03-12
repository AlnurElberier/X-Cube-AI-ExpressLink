/**
******************************************************************************
* @file           : ExpressLink.h 
* @version        : v 1.0.0
* @brief          : This file implements AWS ExpressLink driver header
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

#include "STMicroelectronics.I-CUBE-ExpressLink_conf.h"

#ifndef INC_EXPRESSLINK_H_
#define INC_EXPRESSLINK_H_

/* Private typedef -----------------------------------------------------------*/

#define EXPRESSLINK_LOG_LEVEL_0      0
#define EXPRESSLINK_LOG_LEVEL_1      1
#define EXPRESSLINK_LOG_LEVEL_2      2

#define ExpressLink_BOOT_DELAY       3000

#define ExpressLink_COMMAND_TIMEOUT  10000

#define ExpressLink_MAX_TOPIC_SIZE   128

#define ExpressLink_TechSpec "OK v1.1.1"
/* USER CODE BEGIN PTD */
typedef struct MQTTPublishInfo_t
{
  char * pTopic;
  char * pPayload;
  uint32_t payloadLength;
}MQTTPublishInfo_t;


typedef enum ExpressLink_event_id_t
{
  EXPRESSLINK_EVENT_NONE = (uint32_t)0,
  EXPRESSLINK_EVENT_MESSAGE,
  EXPRESSLINK_EVENT_STARTUP,
  EXPRESSLINK_EVENT_CONLOST,
  EXPRESSLINK_EVENT_OVERRUN,
  EXPRESSLINK_EVENT_OTA,
  EXPRESSLINK_EVENT_CONNECT,
  EXPRESSLINK_EVENT_CONFMODE,
  EXPRESSLINK_EVENT_SUBACK,
  EXPRESSLINK_EVENT_SUBNACK,
  EXPRESSLINK_EVENT_SHADOW_INIT = (uint32_t)20,
  EXPRESSLINK_EVENT_SHADOW_INIT_FAILED,
  EXPRESSLINK_EVENT_SHADOW_INIT_DOC,
  EXPRESSLINK_EVENT_SHADOW_UPDATE,
  EXPRESSLINK_EVENT_SHADOW_DELTA,
  EXPRESSLINK_EVENT_SHADOW_DELETE,
  EXPRESSLINK_EVENT_SHADOW_SUBACK,
  EXPRESSLINK_EVENT_SHADOW_SUBNACK,
  EXPRESSLINK_EVENT_ERROR = (uint32_t)0xFFFFFFFF,
} ExpressLink_event_id_t;

typedef struct ExpressLink_event_t
{
  ExpressLink_event_id_t id;
  uint32_t param;
} ExpressLink_event_t;

typedef enum ExpressLink_state_t
{
  EXPRESSLINK_STATE_INIT      = (uint32_t)(0),
  EXPRESSLINK_STATE_READY     = (uint32_t)(1<<0),
  EXPRESSLINK_STATE_CONNECTED = (uint32_t)(1<<2),
  EXPRESSLINK_STATE_CONNECT   = (uint32_t)(1<<3),
  EXPRESSLINK_STATE_SLEEP     = (uint32_t)(1<<4),
  EXPRESSLINK_STATE_ERROR     = (uint32_t)(1<<5),
} ExpressLink_state_t;

typedef struct ExpressLink_OTA_Data_t
{
  uint32_t count;
  uint32_t* data;
  uint32_t crc;
} ExpressLink_OTA_Data_t;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */
char * ExpressLink_Test                  (void);
void   ExpressLink_Reset                 (void);
char * ExpressLink_Connect               (void);
char * ExpressLink_ConnectAsync          (void);
char * ExpressLink_Disonnect             (void);
char * ExpressLink_Sleep                 (int duration);
char * ExpressLink_SendMessage           (int topicNumber, char * message);
void   ExpressLink_EventCallback         (ExpressLink_event_t event);
char * ExpressLink_SubscribeToTopic      (int topicNumber);
char * ExpressLink_UnsubscribeFromTopic  (int topicNumber);

int      ExpressLink_OTA_GetState        (void);
char *   ExpressLink_OTA_Accept          (void);
char *   ExpressLink_OTA_Apply           (void);
char *   ExpressLink_OTA_Close           (void);
char *   ExpressLink_OTA_Flush           (void);
uint32_t ExpressLink_OTA_Read            (ExpressLink_OTA_Data_t * OTA_Data, uint32_t byte_count);
char *   ExpressLink_OTA_Seek            (uint32_t address);

char * ExpressLink_SetEndpoint           (char * endpoint);
char * ExpressLink_SetSSID               (char * ssid);
char * ExpressLink_SetPassphrase         (char * passphrase);
char * ExpressLink_SetAPN                (char * apn);
char * ExpressLink_SetTopic              (int topicNumber, char * topic);
char * ExpressLink_SetConfMode           (void);
char * ExpressLink_SetDefenderPeriod     (int period);
char * ExpressLink_SetQoS                (int QoS);
void   ExpressLink_SetState              (ExpressLink_state_t NewState);
char * ExpressLink_SetCustomName         (char * CustomName);

char * ExpressLink_ShadowEnable          (void);
char * ExpressLink_ShadowDisable         (void);
char * ExpressLink_ShadowSetToken        (int token);
char * ExpressLink_ShadowInit            (int index);
char * ExpressLink_ShadowSubscribe       (int index);
char*  ExpressLink_ShadowUnSubscribe     (int index);
char * ExpressLink_ShadowSetTopic        (int topicNumber, char *topic);
char * ExpressLink_ShadowDoc             (int index);
char * ExpressLink_ShadowGetDoc          (int index);
char * ExpressLink_ShadowUpdate          (int index, char * new_state);
char * ExpressLink_ShadowGetUpdate       (int index);
char * ExpressLink_ShadowGetDelta        (int index);
char * ExpressLink_ShadowDelete          (int index);
char * ExpressLink_ShadowGetDelete       (int index);

ExpressLink_state_t ExpressLink_GetState (void);

ExpressLink_event_t ExpressLink_GetEvent (void);

char *   ExpressLink_GetTime             (void);
char *   ExpressLink_GetConnection       (void);
char *   ExpressLink_GetThingName        (void);
char *   ExpressLink_GetConf             (char * conf);
void     ExpressLink_GetTopic            (char * topic);
uint32_t ExpressLink_GetMessage          (MQTTPublishInfo_t * msg);
char*    ExpressLink_GetMessageTopic     (int index);

char* expresslink_execute_command(char *command, unsigned long timeout_ms);

#define ExpressLink_GetSSID()        ExpressLink_GetConf((char *)&"SSID"       )
#define ExpressLink_GetAbout()       ExpressLink_GetConf((char *)&"About"      )
#define ExpressLink_GetVersion()     ExpressLink_GetConf((char *)&"Version"    )
#define ExpressLink_GetTechSpec()    ExpressLink_GetConf((char *)&"TechSpec"   )
#define ExpressLink_GetCertificate() ExpressLink_GetConf((char *)&"Certificate")
#define ExpressLink_GetAPN()         ExpressLink_GetConf((char *)&"APN"        )
#define ExpressLink_GetCustomName()  ExpressLink_GetConf((char *)&"CustomName" )

#endif /* INC_EXPRESSLINK_H_ */
