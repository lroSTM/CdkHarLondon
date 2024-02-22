/*
 * Copyright (C) 2021 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 * Derived from simple_sub_pub_demo.c
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */

#include "logging_levels.h"
/* define LOG_LEVEL here if you want to modify the logging level from the default */

#define LOG_LEVEL LOG_INFO

#include "logging.h"

/* Standard includes. */
#include <string.h>
#include <stdio.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "kvstore.h"

/* MQTT library includes. */
#include "core_mqtt.h"
#include "core_mqtt_agent.h"
#include "sys_evt.h"

/* Subscription manager header include. */
#include "subscription_manager.h"


/* Sensor includes */
#include "b_u585i_iot02a_motion_sensors.h"

/* AI includes */
#include "ai_dpu.h"

extern UBaseType_t uxRand(void);

#define MQTT_PUBLISH_MAX_LEN (200)
#define MQTT_PUBLISH_TIME_BETWEEN_MS (5000)
//#define MQTT_PUBLISH_TOPIC "har_inference_result"
#define MQTT_PUBLICH_TOPIC_STR_LEN (256)
#define MQTT_PUBLISH_BLOCK_TIME_MS (200)
#define MQTT_PUBLISH_NOTIFICATION_WAIT_MS (1000)
#define MQTT_NOTIFY_IDX (1)
#define MQTT_PUBLISH_QOS (MQTTQoS0)

#define MOT_EVT (1 << 0)

#define ISM330DHCX_TAG_ACC                   (0x02)

/**
 * @brief Defines the structure to use as the command callback context in this
 * demo.
 */
struct MQTTAgentCommandContext
{
	MQTTStatus_t xReturnStatus;
	TaskHandle_t xTaskToNotify;
	uint32_t ulNotificationValue;
};

/* Private variables ---------------------------------------------------------*/
static float pfAIOutput[AI_NETWORK_OUT_1_SIZE];

/**
 * Specifies the labels for the classes of the demo.
 */
static const char *sAiClassLabels[CTRL_X_CUBE_AI_MODE_CLASS_NUMBER] = CTRL_X_CUBE_AI_MODE_CLASS_LIST;
/**
 * DPUs context
 */
static AIProcCtx_t xAIProcCtx;
/**
 * Microphone task handle
 */
static TaskHandle_t xMotTask;

/*-----------------------------------------------------------*/
static void prvPublishCommandCallback( MQTTAgentCommandContext_t * pxCommandContext,
		MQTTAgentReturnInfo_t * pxReturnInfo )
{
	TaskHandle_t xTaskHandle = ( TaskHandle_t ) pxCommandContext;

	configASSERT( pxReturnInfo != NULL );

	uint32_t ulNotifyValue = pxReturnInfo->returnCode;

	if( xTaskHandle != NULL )
	{
		/* Send the context's ulNotificationValue as the notification value so
		 * the receiving task can check the value it set in the context matches
		 * the value it receives in the notification. */
		( void ) xTaskNotifyIndexed( xTaskHandle,
				MQTT_NOTIFY_IDX,
				ulNotifyValue,
				eSetValueWithOverwrite );
	}
}

/*-----------------------------------------------------------*/

static BaseType_t prvPublishAndWaitForAck( MQTTAgentHandle_t xAgentHandle,
		const char * pcTopic,
		const void * pvPublishData,
		size_t xPublishDataLen )
{
	MQTTStatus_t xStatus;
	size_t uxTopicLen = 0;

	configASSERT( pcTopic != NULL );
	configASSERT( pvPublishData != NULL );
	configASSERT( xPublishDataLen > 0 );

	uxTopicLen = strnlen( pcTopic, UINT16_MAX );

	MQTTPublishInfo_t xPublishInfo =
	{
			.qos             = MQTT_PUBLISH_QOS,
			.retain          = 0,
			.dup             = 0,
			.pTopicName      = pcTopic,
			.topicNameLength = ( uint16_t ) uxTopicLen,
			.pPayload        = pvPublishData,
			.payloadLength   = xPublishDataLen
	};

	MQTTAgentCommandInfo_t xCommandParams =
	{
			.blockTimeMs                 = MQTT_PUBLISH_BLOCK_TIME_MS,
			.cmdCompleteCallback         = prvPublishCommandCallback,
			.pCmdCompleteCallbackContext = ( void * ) xTaskGetCurrentTaskHandle(),
	};

	if( xPublishInfo.qos > MQTTQoS0 )
	{
		xCommandParams.pCmdCompleteCallbackContext = ( void * ) xTaskGetCurrentTaskHandle();
	}

	/* Clear the notification index */
	xTaskNotifyStateClearIndexed( NULL, MQTT_NOTIFY_IDX );


	xStatus = MQTTAgent_Publish( xAgentHandle,
			&xPublishInfo,
			&xCommandParams );

	if( xStatus == MQTTSuccess )
	{
		uint32_t ulNotifyValue = 0;
		BaseType_t xResult = pdFALSE;

		xResult = xTaskNotifyWaitIndexed( MQTT_NOTIFY_IDX,
				0xFFFFFFFF,
				0xFFFFFFFF,
				&ulNotifyValue,
				pdMS_TO_TICKS( MQTT_PUBLISH_NOTIFICATION_WAIT_MS ) );

		if( xResult )
		{
			xStatus = ( MQTTStatus_t ) ulNotifyValue;

			if( xStatus != MQTTSuccess )
			{
				LogError( "MQTT Agent returned error code: %d during publish operation.",
						xStatus );
				xResult = pdFALSE;
			}
		}
		else
		{
			LogError( "Timed out while waiting for publish ACK or Sent event. xTimeout = %d",
					pdMS_TO_TICKS( MQTT_PUBLISH_NOTIFICATION_WAIT_MS ) );
			xResult = pdFALSE;
		}
	}
	else
	{
		LogError( "MQTTAgent_Publish returned error code: %d.", xStatus );
	}

	return( xStatus == MQTTSuccess );
}

static BaseType_t xIsMqttConnected(void)
{
	/* Wait for MQTT to be connected */
	EventBits_t uxEvents = xEventGroupWaitBits(xSystemEvents,
			EVT_MASK_MQTT_CONNECTED,
			pdFALSE,
			pdTRUE,
			0);

	return ((uxEvents & EVT_MASK_MQTT_CONNECTED) == EVT_MASK_MQTT_CONNECTED);
}

/* CRC init function */
static void CRC_Init(void)
{
	CRC_HandleTypeDef hcrc;
	hcrc.Instance = CRC;
	hcrc.Init.DefaultPolynomialUse = DEFAULT_POLYNOMIAL_ENABLE;
	hcrc.Init.DefaultInitValueUse = DEFAULT_INIT_VALUE_ENABLE;
	hcrc.Init.InputDataInversionMode = CRC_INPUTDATA_INVERSION_NONE;
	hcrc.Init.OutputDataInversionMode = CRC_OUTPUTDATA_INVERSION_DISABLE;
	hcrc.InputDataFormat = CRC_INPUTDATA_FORMAT_BYTES;
	__HAL_RCC_CRC_CLK_ENABLE();
	if (HAL_CRC_Init(&hcrc) != HAL_OK)
	{
		LogError("CRC Init Error");
	}
}

static BaseType_t xInitSensors(int fifo_level)
{
	int32_t lBspError = BSP_ERROR_NONE;

	lBspError  = BSP_MOTION_SENSOR_Init( 0,MOTION_ACCELERO );
	lBspError |= BSP_MOTION_SENSOR_SetOutputDataRate( 0, MOTION_ACCELERO, CTRL_X_CUBE_AI_SENSOR_ODR );
	lBspError |= BSP_MOTION_SENSOR_SetFullScale(0, MOTION_ACCELERO, CTRL_X_CUBE_AI_SENSOR_FS);
	lBspError |= BSP_MOTION_SENSOR_Fifo( 0,MOTION_ACCELERO,fifo_level);
	lBspError |= BSP_MOTION_SENSOR_Enable(0, MOTION_ACCELERO);

	return (lBspError == BSP_ERROR_NONE ? pdTRUE : pdFALSE);
}

void vMotionSensorPublishTask(void *pvParameters)
{
	BaseType_t xResult = pdFALSE;
	BaseType_t xExitFlag = pdFALSE;
	char payloadBuf[MQTT_PUBLISH_MAX_LEN];
	MQTTAgentHandle_t xAgentHandle = NULL;
	char pcTopicString[MQTT_PUBLICH_TOPIC_STR_LEN] = {0};
	size_t uxTopicLen = 0;
	uint32_t ulNotifiedValue = 0;
	char * pcDeviceId = NULL;
	int lTopicLen = 0;


	(void)pvParameters; /* unused parameter */

	/**
	 * Initialize the CRC IP required by X-CUBE-AI.
	 * Must be called before any usage of the ai library API.
	 */
	CRC_Init();

	/**
	 * get task handle for notifications
	 */
	xMotTask = xTaskGetCurrentTaskHandle();

	/**
	 * get the AI model
	 */
	AiDPULoadModel(&xAIProcCtx, "network");

	/**
	 * initialise sensors
	 */
	xResult = xInitSensors(xAIProcCtx.In_Height);

	if (xResult != pdTRUE)
	{
		LogError("Error while motion sensor.");
		vTaskDelete(NULL);
	}


	pcDeviceId = KVStore_getStringHeap( CS_CORE_THING_NAME, NULL );

	if( pcDeviceId == NULL )
	{
		xExitFlag = pdTRUE;
	}
	else
	{
		lTopicLen = snprintf( pcTopicString, ( size_t ) MQTT_PUBLICH_TOPIC_STR_LEN, "%s/har_inference_result", pcDeviceId );
	}

	if( ( lTopicLen <= 0 ) || ( lTopicLen > MQTT_PUBLICH_TOPIC_STR_LEN ) )
	{
		LogError( "Error while constructing topic string." );
		xExitFlag = pdTRUE;
	}

	vSleepUntilMQTTAgentReady();

	xAgentHandle = xGetMqttAgentHandle();

	while (xExitFlag == pdFALSE)
	{
		TimeOut_t xTimeOut;
		vTaskSetTimeOutState(&xTimeOut);

		if (xTaskNotifyWait(0, 0xFFFFFFFF, &ulNotifiedValue, portMAX_DELAY) == pdTRUE)
		{
			AiProcess(&xAIProcCtx, pfAIOutput);
		}

		float max_out = pfAIOutput[0];
		uint32_t max_idx = 0;
		for (uint32_t i = 1; i < CTRL_X_CUBE_AI_MODE_CLASS_NUMBER; i++)
		{
			if (pfAIOutput[i] > max_out)
			{
				max_idx = i;
				max_out = pfAIOutput[i];
			}
		}
		size_t bytesWritten = snprintf(payloadBuf, (size_t)MQTT_PUBLISH_MAX_LEN,
				"{\"class\":\"%s\"}", sAiClassLabels[max_idx]);

		if (xIsMqttConnected() == pdTRUE)
		{
			if (bytesWritten < MQTT_PUBLISH_MAX_LEN)
			{
				xResult = prvPublishAndWaitForAck(xAgentHandle,
						pcTopicString,
						payloadBuf,
						( size_t ) bytesWritten);
				if (xResult == pdTRUE)
				{
					LogInfo(payloadBuf);
				}
			}
			else if (bytesWritten > 0)
			{
				LogError("Not enough buffer space.");
			}
			else
			{
				LogError("MQTT Publish call failed.");
			}
		}
	}
	AiDPUReleaseModel(&xAIProcCtx);
	vPortFree( pcDeviceId );
}

void ISM330DHCX_EXTI_Callback(uint16_t Pin)
{

	int ret;
	ret = ISM330DHCX_Get_Fifo(0, xAIProcCtx.In_Height, xAIProcCtx.p_buffer);
	int8_t* p_in = xAIProcCtx.p_buffer;
	int16_t* p_out = (int16_t*)p_in;
	for (int i = 0; i < xAIProcCtx.In_Height; i++) {
		if ((*p_in >> 3) == ISM330DHCX_TAG_ACC) {
			p_in++; // skip tag
			*p_out++ = *(int16_t*)p_in;
			p_in += 2;
			*p_out++ = *(int16_t*)p_in;
			p_in += 2;
			*p_out++ = *(int16_t*)p_in;
			p_in += 2;
		} else {
			p_in += 7; // Skip non-ACC data
		}
	}

	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	BaseType_t rslt = pdFALSE;
	rslt = xTaskNotifyFromISR(xMotTask,
			MOT_EVT,
			eSetBits,
			&xHigherPriorityTaskWoken);
	configASSERT(rslt == pdTRUE);
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}
