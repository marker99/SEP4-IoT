/*
* loraWan_up_link_handler.c
*
* Created: 25-05-2022 12:56:14
*  Author: Sander
*/
#include "headers/loraWan_up_link_handler.h"
#include <ATMEGA_FreeRTOS.h>
#include <message_buffer.h>
#include <lora_driver.h>
#include <stdio.h>

static lora_driver_payload_t _uplink_payload;
static MessageBufferHandle_t _uplink_message_buffer;
static pMeasurment_t _measurment_data;

static TickType_t _xLastWakeTime;
static const TickType_t xFrequency = pdMS_TO_TICKS(100000);


void loraWan_up_link_handler_initialize(UBaseType_t task_priority, MessageBufferHandle_t uplink_message_buffer){
	
	_uplink_message_buffer = uplink_message_buffer;
	
	_measurment_data = pvPortMalloc(sizeof(measurment_t));
	
	xTaskCreate(
	loraWan_up_link_handler_task_run,
	"LoraWanUplinkHandler",
	configMINIMAL_STACK_SIZE,
	NULL,
	task_priority,
	NULL);
	
}



void loraWan_up_link_handler_task_init(){
	_xLastWakeTime = xTaskGetTickCount();
	
}

void loraWan_up_link_handler_task_run(){
	
	xTaskDelayUntil(&_xLastWakeTime, xFrequency);
	
	printf("Uplink: starting to read from buffer");
	
	while(xMessageBufferIsEmpty(_uplink_message_buffer) == pdFALSE){	
		
		int16_t recivedBytes = xMessageBufferReceive(_uplink_message_buffer, _measurment_data, sizeof(measurment_t), portMAX_DELAY);
		
		printf("Read from uplink buffer bytes: %d", recivedBytes);
		
		if ( _measurment_data != NULL){
			printf("Data buffer was not null ");
			//send_measurment(_data_buffer);
			
		}
	}
		

		
}

	void loraWan_uplink_handler_task(void *pvParameters){
		
		loraWan_up_link_handler_task_init();
		
		
		for(;;){
			
			
			loraWan_up_link_handler_task_run();
		}

	}


	void send_measurment(pMeasurment_t newMeasurment){
		
		//prepare payload
		_uplink_payload.len = 6;
		_uplink_payload.portNo = 2;
		
		//extract data from measurment
		int16_t temp = newMeasurment->temperature;
		uint16_t hum = newMeasurment->humidity;
		uint16_t co2_ppm = newMeasurment->co2PartsPrMillion;

		// format data into bytes (two for each value)
		_uplink_payload.bytes[0] = temp >> 8;
		_uplink_payload.bytes[1] = temp & 0xFF;

		_uplink_payload.bytes[2] = hum >> 8;
		_uplink_payload.bytes[3] = hum & 0xFF;
		
		_uplink_payload.bytes[4] = co2_ppm >> 8;
		_uplink_payload.bytes[5] = co2_ppm & 0xFF;
		
		// send to loraWan using loraWan drivers
		printf("Upload Message >%s<\n", lora_driver_mapReturnCodeToText(lora_driver_sendUploadMessage(false, &_uplink_payload)));
	}
