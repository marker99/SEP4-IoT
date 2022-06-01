/*
* loraWan_up_link_handler.c
*
* Created: 25-05-2022 12:56:14
*  Author: Sander
*/
#include "headers/loraWan_up_link_handler.h"
#include "util/headers/thread_safe_printf.h"
#include <ATMEGA_FreeRTOS.h>
#include <message_buffer.h>
#include <lora_driver.h>
#include <stdio.h>

#include "up_link_data_config.h"
#include "time_interval_config.h"
#include "measurment.h"

static lora_driver_payload_t *_uplink_payload_p;

static TickType_t _xLastWakeTime;
static const TickType_t xFrequency = LORA_WAN_UPLINK_FREQUENCY;



static uint8_t payLoadSlot = 0;

//private methods
static void send_payload();
static void prepare_uplink_payload();


void loraWan_up_link_handler_initialize(UBaseType_t task_priority){
	

	prepare_uplink_payload();
	
	xTaskCreate(
	loraWan_up_link_handler_task,
	"LoraWanUplinkHandler",
	configMINIMAL_STACK_SIZE-40,
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
	UBaseType_t uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);
	thread_safe_printf("\n>uplink end Stack Size %d\n", uxHighWaterMark);
}

void loraWan_up_link_handler_task(void *pvParameters){
	
	loraWan_up_link_handler_task_init();

	for(;;){
		loraWan_up_link_handler_task_run();
	}

}


static void prepare_uplink_payload(){
	_uplink_payload_p = pvPortMalloc(sizeof(lora_driver_payload_t));
	
	_uplink_payload_p->portNo = UP_LINK_PORT_NO;
	_uplink_payload_p->len = SIZE_OF_PAYLOAD_DATA;

}


void loraWan_up_link_handler_append_to_payload_data(pMeasurment_t newMeasurment){
	
	// append to payload data as long as there is space left
	if(payLoadSlot/SIZE_OF_PAYLOAD_DATA < NUMBER_OF_MEASUREMENTS_TO_SEND){
		//pack data into 4 bytes
		uint8_t temp = newMeasurment->temperature - 100;
		uint8_t hum = newMeasurment->humidity/10;
		uint16_t co2_ppm = newMeasurment->co2PartsPrMillion;
		
		_uplink_payload_p->bytes[payLoadSlot + 0] = temp;
		_uplink_payload_p->bytes[payLoadSlot + 1] = hum;
		_uplink_payload_p->bytes[payLoadSlot + 2] = co2_ppm >> 8;
		_uplink_payload_p->bytes[payLoadSlot + 3] = co2_ppm & 0xFF;
		payLoadSlot += NUMBER_OF_BYTES_FOR_A_MEASURMENT;
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

