/*
 * loraWan_up_link_handler.c
 *
 * Created: 25-05-2022 12:56:14
 *  Author: sande
 */ 
#include "loraWan_up_link_handler.h"
#include <ATMEGA_FreeRTOS.h>
#include <message_buffer.h>
#include <lora_driver.h>


void loraWan_uplin_handler_initialize(){
	
	
}




void send_measurment(measurment_t *newMeasurment){
	
	//prepare payload
	_uplink_payload.len = 6;
	_uplink_payload.portNo = 2;
	
	//extract data from measurment
	int16_t temp = newMeasurment->temperatur;
	uint16_t hum = newMeasurment->humidity;
	uint16_t co2_ppm = newMeasurment->co2_ppm;

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
