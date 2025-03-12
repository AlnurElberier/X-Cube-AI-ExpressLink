/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : compute_inference.c
 * @brief          : Demo Computing Sine inference
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

/* Includes ------------------------------------------------------------------*/
/* Includes ------------------------------------------------------------------*/

#include "main.h"

/* USER CODE BEGIN Includes */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "event_groups.h"

#include "ExpressLink.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ai_datatypes_defines.h"
#include "ai_platform.h"
#include "network.h"
#include "network_data.h"

/* Private typedef -----------------------------------------------------------*/


/* Private macro -------------------------------------------------------------*/

#define AI_MQTT_PUBLISH_TIME_BETWEEN_MS         ( 3000 )

/* 0: no debug, 1: Errors only, 2: INFO, 3: all (Error, Info, Debug)  */
#define DEBUG_LEVEL 1

#include "debug.h"
/* Private variables ---------------------------------------------------------*/

extern SemaphoreHandle_t ExpressLink_MutexHandle;

#define BUFFER_SIZE_ENV (EXPRESSLINK_TX_BUFFER_SIZE/4)

static char payloadBuf[BUFFER_SIZE_ENV];
static char topic     [BUFFER_SIZE_ENV];

static ai_u8 activations[AI_NETWORK_DATA_ACTIVATIONS_SIZE];
static ai_u8 in_data[AI_NETWORK_IN_1_SIZE_BYTES];
static ai_u8 out_data[AI_NETWORK_OUT_1_SIZE_BYTES];

/* AI buffer IO handlers */
static ai_buffer *ai_input;
static ai_buffer *ai_output;

static ai_handle network = AI_HANDLE_NULL;  // AI model handle
static ai_network_report report;            // Model report structure
static float xval = 0.0f;                   // Input variable for inference
static float step = 0.01f;                  // Step size for input value changes




/* Private function prototypes -----------------------------------------------*/
static int32_t AI_Init(ai_handle *network);

/* User code ---------------------------------------------------------*/
void vInferencePublishTask(void *argument) {
	ai_error err;
	ai_i32 n_batch;
	char *response;

	PRINTF("[INFO] InferencePublish started\r\n");

	if (AI_Init(&network) != 0) {
		PRINTF_ERROR("[ERROR] AI Init failed\r\n");

		vTaskSuspend( NULL);
	}

	/* Acquire the express link mutex */
	xSemaphoreTake(ExpressLink_MutexHandle, portMAX_DELAY);

	/* Get the thing name */
	response = ExpressLink_GetThingName();

	/* Format the topic <ThingName>/env_sensor_data */
	response[strlen(response) - 2] = '\0';
	snprintf(topic, BUFFER_SIZE_ENV, "%s/ai_data", response);

	/* Set the topic */
	response = ExpressLink_SetTopic(TOPIC_INDEX_AI, topic);

	/* Release the express link mutex */
	xSemaphoreGive(ExpressLink_MutexHandle);

	for (;;) {

		((ai_float *)in_data)[0] = (ai_float)xval;

		/** @brief Create the AI buffer IO handlers
		 *  @note  ai_inuput/ai_output are already initilaized after the
		 *         ai_network_get_report() call. This is just here to illustrate
		 *         the case where get_report() is not called.
		 */
		ai_input = ai_network_inputs_get(network, NULL);
		ai_output = ai_network_outputs_get(network, NULL);

		/** @brief Set input/output buffer addresses */
		ai_input[0].data = AI_HANDLE_PTR(in_data);
		ai_output[0].data = AI_HANDLE_PTR(out_data);


		/** @brief Perform the inference */
		n_batch = ai_network_run(network, &ai_input[0], &ai_output[0]);
		if (n_batch != 1) {
			err = ai_network_get_error(network);
			PRINTF_ERROR("[ERROR] ai run error %d, %d\n", err.type, err.code);
			return;
		}

		PRINTF("[INFO] {\"inference_in\": %f, \"inference_out\": %f}\r\n",
				((float*) in_data)[0],
				((float*) out_data)[0]);

		/* Format the message with the new sensor data */
		snprintf(payloadBuf, BUFFER_SIZE_ENV,
				"{\"inference_in\": %f, \"inference_out\": %f}",
				((float*) in_data)[0],
				((float*) out_data)[0]);

		PRINTF_INFO("[INFO] Sending MQTT message: %s\r\n", payloadBuf);


		/* Acquire the express link mutex */
		xSemaphoreTake(ExpressLink_MutexHandle, portMAX_DELAY);
		/* Publish the sensor data message */
		taskENTER_CRITICAL();
		response = ExpressLink_SendMessage(TOPIC_INDEX_AI, payloadBuf);
		taskEXIT_CRITICAL();

		if (strcmp(response, "OK\r\n") != 0) {
			PRINTF_ERROR(
					"\r\n[ERROR] ExpressLink_SendMessage. Generating reset\r\n");
			NVIC_SystemReset();
		}

		/* Release the express link mutex */
		xSemaphoreGive(ExpressLink_MutexHandle);

		xval += step;
		if (xval >= 6.0f) {
		    xval = 0.0f;  // Reset xval instead of returning
		}

		vTaskDelay(pdMS_TO_TICKS(AI_MQTT_PUBLISH_TIME_BETWEEN_MS));
	}
}

static int32_t AI_Init(ai_handle *network)
{
    ai_error err;

    const ai_handle acts[] = { activations };
    err = ai_network_create_and_init(network, acts, NULL);
    if (err.type != AI_ERROR_NONE) {
        PRINTF_ERROR("[ERROR] ai init_and_create error\n");
        return -1;
    }

    if (ai_network_get_report(*network, &report) != true) {
        PRINTF_ERROR("[ERROR] ai get report error\n");
        return -1;
    }

    PRINTF("[INFO] Model name      : %s\n", report.model_name);
    PRINTF_INFO("[INFO] Model signature : %s\n", report.model_signature);

    ai_input = &report.inputs[0];
    ai_output = &report.outputs[0];

    PRINTF_INFO("[INFO] input[0] : (%d, %d, %d)\n",
            AI_BUFFER_SHAPE_ELEM(ai_input, AI_SHAPE_HEIGHT),
            AI_BUFFER_SHAPE_ELEM(ai_input, AI_SHAPE_WIDTH),
            AI_BUFFER_SHAPE_ELEM(ai_input, AI_SHAPE_CHANNEL));

    PRINTF_INFO("[INFO] output[0] : (%d, %d, %d)\n",
            AI_BUFFER_SHAPE_ELEM(ai_output, AI_SHAPE_HEIGHT),
            AI_BUFFER_SHAPE_ELEM(ai_output, AI_SHAPE_WIDTH),
            AI_BUFFER_SHAPE_ELEM(ai_output, AI_SHAPE_CHANNEL));

    return 0;  // Success
}

