/*
* loraWan_up_link_handler.c
*
* Created: 25-05-2022 12:56:14
*  Author: Sander
*/
#include "headers/loraWan_up_link_handler.h"
#include "util/thread_safe_printf.h"
#include <ATMEGA_FreeRTOS.h>
#include <message_buffer.h>
#include <lora_driver.h>
#include <stdio.h>

#include "time_interval_config.h"
#include "measurment.h"

static lora_driver_payload_t *_uplink_payload_p;

static MessageBufferHandle_t _uplink_message_buffer;

static TickType_t _xLastWakeTime;
static const TickType_t xFrequency = LORA_WAN_UPLINK_FREQUENCY;

// could be define in a config
#define NUMBER_OF_MEASUREMENTS_TO_SEND 5
static uint8_t sizeOfPayLoadData;
static uint8_t payLoadSlot = 0;

//private methods
static void send_payload();
static void prepare_uplink_payload();


void loraWan_up_link_handler_initialize(UBaseType_t task_priority, MessageBufferHandle_t uplink_message_buffer){
	
	printf("sizeof(measurement): %d, count(measurements): %d\n", 4, NUMBER_OF_MEASUREMENTS_TO_SEND);

	sizeOfPayLoadData = 4 * NUMBER_OF_MEASUREMENTS_TO_SEND;
	
	printf("payload data: %d\n", sizeOfPayLoadData);
	
	// not used
	_uplink_message_buffer = uplink_message_buffer;

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
	// shifting the upload interval to make sure all measurements are ready before sending. 
	vTaskDelay(LORA_WAN_UP_LINK_TASK_START_OFFSET);
	
	_xLastWakeTime = xTaskGetTickCount();
}

void loraWan_up_link_handler_task_run(){
	
	xTaskDelayUntil(&_xLastWakeTime, xFrequency);
	
	/*
	thread_safe_printf("\nUplink: starting to read from buffer\n");
	xMessageBufferIsEmpty(_uplink_message_buffer) == pdFALSE){
	
	int16_t recivedBytes = xMessageBufferReceive(_uplink_message_buffer, &_measurment_data, sizeof(measurment_t), 0);
	
	thread_safe_printf("\nRead from uplink buffer bytes: %d\n", recivedBytes);
	thread_safe_printf(">Read from buffer! Measurement <\n\tTemp: %d\n\tHum: %d\n\tco2: %d\n> Measurement <\n", _measurment_data.temperature, _measurment_data.humidity, _measurment_data.co2PartsPrMillion);
	} */
	/*
	_measurment_data->co2PartsPrMillion = 1000;
	_measurment_data->humidity = 66;
	_measurment_data->temperature = 25;
	*/

	send_payload();
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
	
	printf("payload size: %d\n", sizeof(_uplink_payload_p->bytes));
}


void loraWan_up_link_handler_append_to_payload_data(pMeasurment_t newMeasurment){
	
	if(payLoadSlot/6 < NUMBER_OF_MEASUREMENTS_TO_SEND){
		//pack data into 4 bytes
		uint8_t temp = newMeasurment->temperature - 100;
		uint8_t hum = newMeasurment->humidity/10;
		uint16_t co2_ppm = newMeasurment->co2PartsPrMillion;
		
		_uplink_payload_p->bytes[payLoadSlot + 0] = temp;
		_uplink_payload_p->bytes[payLoadSlot + 1] = hum;
		_uplink_payload_p->bytes[payLoadSlot + 2] = co2_ppm >> 8;
		_uplink_payload_p->bytes[payLoadSlot + 3] = co2_ppm & 0xFF;
		payLoadSlot += 4;
		}
		else{
			
		thread_safe_printf("Lora wan up link handler: Could not add more measurements to the payload\n");
	}

}


static void send_payload(){
	// send to loraWan using loraWan drivers
	thread_safe_printf("Upload Message >%s<\n", lora_driver_mapReturnCodeToText(lora_driver_sendUploadMessage(true, _uplink_payload_p)));
	payLoadSlot = 0;
}

