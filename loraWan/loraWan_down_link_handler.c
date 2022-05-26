/*
* loraWan_down_link_handler.c
*
* Created: 26-05-2022 13:25:38
*  Author: Frederik
*/

#include <ATMEGA_FreeRTOS.h>
#include <message_buffer.h>
#include <lora_driver.h>
#include <stdio.h>

#include "headers/loraWan_down_link_handler.h"
#include "configuration_settings.h"

static TickType_t xLastWakeTime;

static MessageBufferHandle_t _downLinkMessageBuffer;

static lora_driver_payload_t payload;

void loraWan_down_link_handler_initialize(UBaseType_t task_priority, MessageBufferHandle_t downLinkBuffer){
	
	_downLinkMessageBuffer = downLinkBuffer;
	
	xTaskCreate(loraWan_down_link_handler_task_handler,
	"DownlinkHandlerTask",
	configMINIMAL_STACK_SIZE,
	NULL,
	task_priority,
	NULL);
}


void loraWan_down_link_handler_task_run(void){
	
	//need delay here probably
	
	size_t received = xMessageBufferReceive(_downLinkMessageBuffer,
	payload,
	sizeof(payload),
	portMAX_DELAY);
	
	printf("DownlinkBuffer: Settings Received\n")
	
	if (received != 5) return;
	
	// Reading all raw data bytes
	int16_t tagetTemp = payload.bytes[0];
	int16_t tempMargin = payload.bytes[1];
	uint16_t humThreshold =  payload.bytes[2];
	uint16_t co2Threshold =  payload.bytes[3] << 8 | payload.bytes[4];
	
	// Adjust starting point
	tagetTemp += 10;
	co2Threshold += 200;
	
	// Setting the configuration targets and thresholds
	configMutex_setTargetTemperature(tagetTemp);
	configMutex_setTemperatureMargin(tempMargin);
	configMutex_setHumidityThreshold(humThreshold);
	configMutex_setCO2Threshold(co2Threshold);
}


void loraWan_down_link_handler_task_handler(void *pvParameters){

	for (;;)
	{
		loraWan_down_link_handler_task_run();
	}
	
}
