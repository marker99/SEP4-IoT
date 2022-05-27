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

static lora_driver_payload_t *_uplink_payload_p;
static MessageBufferHandle_t _uplink_message_buffer;
static pMeasurment_t _measurment_data;

static TickType_t _xLastWakeTime;
static const TickType_t xFrequency = pdMS_TO_TICKS(65000);

// could be define in a config
//static uint8_t numberOfMeasurmentsSend = 3;
#define NUMBER_OF_MEASUREMENTS_TO_SEND 3
static uint8_t sizeOfPayLoadData;
static uint8_t payLoadSlot = 0;

//private methods
static void send_payload();
static void prepare_uplink_payload();
static void append_to_payload_data(pMeasurment_t);


void loraWan_up_link_handler_initialize(UBaseType_t task_priority, MessageBufferHandle_t uplink_message_buffer){
	
    printf("sizeof(measurement): %d, count(measurements): %d", sizeof(measurment_t), NUMBER_OF_MEASUREMENTS_TO_SEND)
    
	sizeOfPayLoadData = sizeof(measurment_t) * NUMBER_OF_MEASUREMENTS_TO_SEND;
	
    printf("payload data: %d", sizeOfPayLoadData)
    
	_uplink_message_buffer = uplink_message_buffer;
	
	_measurment_data = pvPortMalloc(sizeof(measurment_t));
	
	prepare_uplink_payload();
	
	xTaskCreate(
	loraWan_up_link_handler_task,
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
	
	printf("\nUplink: starting to read from buffer\n");
	
	while(xMessageBufferIsEmpty(_uplink_message_buffer) == pdFALSE){
		
		int16_t recivedBytes = xMessageBufferReceive(_uplink_message_buffer, _measurment_data, sizeof(measurment_t), 0);
		
		
		printf("\nRead from uplink buffer bytes: %d\n", recivedBytes);
		printf(">Read from buffer! Measurement <\n\tTemp: %d\n\tHum: %d\n\tco2: %d\n> Measurement <\n", _measurment_data->temperature, _measurment_data->humidity, _measurment_data->co2PartsPrMillion);
		
		append_to_payload_data(_measurment_data);
		vTaskDelay(pdMS_TO_TICKS(300));
	}
	
	
	payLoadSlot = 0;
	send_payload();
	//printf("Upload Message >%s<\n", lora_driver_mapReturnCodeToText(lora_driver_sendUploadMessage(false, _uplink_payload_p)));
}

void loraWan_up_link_handler_task(void *pvParameters){
	
	loraWan_up_link_handler_task_init();
	
	
	for(;;){
		loraWan_up_link_handler_task_run();
	}

}


static void prepare_uplink_payload(){
	_uplink_payload_p = pvPortMalloc(sizeof(lora_driver_payload_t));
	
	_uplink_payload_p->portNo = 2;
	_uplink_payload_p->len = sizeOfPayLoadData;
}

static void append_to_payload_data(pMeasurment_t newMeasurment){
	printf("Payload slot: %d", payLoadSlot);
	
	uint16_t temp = newMeasurment->temperature;
	uint16_t hum = newMeasurment->humidity;
	uint16_t co2_ppm = newMeasurment->co2PartsPrMillion;
	
    // Right Shift 8 bits to get rid of the Lower Byte of the value
	_uplink_payload_p->bytes[payLoadSlot + 0] = temp >> 8;
    // And with 0xFF (1111 1111) to get rid of the Upper Byte of the value
	_uplink_payload_p->bytes[payLoadSlot + 1] = temp & 0xFF;

	_uplink_payload_p->bytes[payLoadSlot + 2] = hum >> 8;
	_uplink_payload_p->bytes[payLoadSlot + 3] = hum & 0xFF;
	
	_uplink_payload_p->bytes[payLoadSlot + 4] = co2_ppm >> 8;
	_uplink_payload_p->bytes[payLoadSlot + 5] = co2_ppm & 0xFF;
	
	payLoadSlot += sizeof(measurment_t);
}


static void send_payload(){
	printf("Upload Message >john<\n");
	int i;
	for(i = 0; i<sizeOfPayLoadData; i++){
		printf("%x", _uplink_payload_p->bytes[i]);
	}
	printf("\n");
	
	// send to loraWan using loraWan drivers
	//printf("Upload Message >%s<\n", lora_driver_mapReturnCodeToText(lora_driver_sendUploadMessage(false, _uplink_payload_p)));
}
