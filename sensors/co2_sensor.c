/*
 * co2_sensor.c
 *
 * Created: 12-05-2022 07:19:51
 *  Author: sande
 */ 

#include "co2_sensor.h"

// freeRtos include
#include <ATMEGA_FreeRTOS.h>
#include <task.h>
#include <semphr.h>

// IO includes
#include <stdio.h>
#include <avr/io.h>
#include <serial.h>

void co2_sensor_initialize(UBaseType_t task_priority)
{
	
	//  
	mh_z19_initialise(ser_USART3);
	mh_z19_injectCallBack(_co2_sensor_callback);
	
	xTaskCreate(
	co2_sensor_taskHandler,
	"CO2_Sensor", 
	configMINIMAL_STACK_SIZE, 
	NULL, 
	task_priority, 
	NULL);
	
}

void co2_sensor_taskHandler(void *pvParamerters)
{

	_co2_sensor_task_init();
	
	for(;;)
	{
		_co2_sensor_task_run();
	}
}

void _co2_sensor_callback(uint16_t ppm)
{
	// could set a variable, or set the "measurment event group" to done;
	
}

void _co2_sensor_task_init(){
	
}

void _co2_sensor_task_run(){
	
		/*
		// replace with check on event group
		if (xSemaphoreTake(xTask1Semaphore, portMAX_DELAY)){
			printf("CO2_sensor started measurment\n");
			// callback called when measurment is done
			if(MHZ19_OK != mh_z19_takeMeassuring()){
				printf("SOmething WIthTHE CO2 Went WONG!\n");
			}
		}*/
		// remove when event group is implemented
		
}


