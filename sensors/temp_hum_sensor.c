/*
* temp_hum_sensor.c
*
* Created: 12-05-2022 07:33:43
*  Author: sande
*/

#include "temp_hum_sensor.h"


static EventGroupHandle_t _dataReadyEventGroup;
static EventGroupHandle_t _measureEventGroup;

void temp_hum_sensor_initialize(UBaseType_t task_priority, EventGroupHandle_t readyGroup, EventGroupHandle_t measureGroup)
{
	// Initialize temp/hum
	if ( HIH8120_OK != hih8120_initialise() )
	{
		printf("failed initializing temp/hum \n");
	}
	
	_dataReadyEventGroup = readyGroup;
	_measureEventGroup = measureGroup;

	xTaskCreate(
	temp_hum_sensor_task_handler,
	"Temp/humSensorTask",
	configMINIMAL_STACK_SIZE,
	NULL,
	task_priority,
	NULL);
}

void temp_hum_sensor_task_init(){
	
}

void temp_hum_sensor_task_run(){
	
	if (HIH8120_OK != hih8120_wakeup()){
		printf("temp/hum did not wake up\n");
	}
	
	// need some delay
	
	if (HIH8120_OK != hih8120_measure()){
		printf("the measurement went wrong\n");
	}
	
	// need some delay
	
	if(hih8120_isReady()){
		// set the ready measurment bit in event group
	}
	
}


int16_t temp_hum_getTemperature(void){
	
	if (hih8120_isReady()){
		return hih8120_getTemperature_x10();
	}
	return 0;
}

uint16_t temp_hum_getHumidity(void){
	// double check, would probably not be necessary when called after event group is set as ready
	if (hih8120_isReady()){
		return hih8120_getHumidityPercent_x10();
	}
	return 0;
}

void temp_hum_sensor_task_handler(void *pvParameters)
{

	temp_hum_sensor_task_init();
	
	for (;;)
	{
		temp_hum_sensor_task_run();
	}

}
