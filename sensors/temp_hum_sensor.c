/*
* temp_hum_sensor.c
*
* Created: 12-05-2022 07:33:43
*  Author: sande
*/

#include "temp_hum_sensor.h"
#include "../application_config.h"
#include <status_leds.h>

static EventGroupHandle_t _dataReadyEventGroup;
static EventGroupHandle_t _measureEventGroup;

static TickType_t xLastWakeTime;

// declaration of private methods
static void temp_hum_sensor_startMeasure();



void temp_hum_sensor_initialize(UBaseType_t task_priority, EventGroupHandle_t readyGroup, EventGroupHandle_t measureGroup)
{
	// Initialize temp/hum
	if ( HIH8120_OK == hih8120_initialise() )
	{
		_dataReadyEventGroup = readyGroup;
		_measureEventGroup = measureGroup;

		xTaskCreate(
		temp_hum_sensor_task_handler,
		"Temp/humSensorTask",
		configMINIMAL_STACK_SIZE,
		NULL,
		task_priority,
		NULL);
		
		status_leds_ledOn(led_ST3);
	}
	
	printf("failed initializing temp/hum \n");
}


void temp_hum_sensor_task_init(){
	printf("hum temp Sensor ready\n");
	xLastWakeTime =  xTaskGetTickCount();
}


static void temp_hum_sensor_startMeasure(){
	printf("temp hum sensor started measuring\n");
	status_leds_fastBlink(led_ST3);
	
	
	vTaskDelay(pdMS_TO_TICKS(50));
	
	if (HIH8120_OK != hih8120_wakeup()){
		printf("temp/hum did not wake up\n");
	}
	
    vTaskDelay(pdMS_TO_TICKS(50));

	if (HIH8120_OK != hih8120_measure()){
		printf("the measurement went wrong\n");
	}
	
	vTaskDelay(pdMS_TO_TICKS(50));
}


void temp_hum_sensor_task_run(){
	
	
	xEventGroupWaitBits(_measureEventGroup,
	BIT_START_TEMPHUM_MEASURE,
	pdTRUE,
	pdTRUE,
	portMAX_DELAY
	);
	
	
	//Sensors start measuring
	temp_hum_sensor_startMeasure();
	
	
	if(hih8120_isReady()){
		// set the ready measurment bit in event group
		status_leds_ledOn(led_ST3);
		xEventGroupSetBits(_dataReadyEventGroup, BIT_TEMPHUM_READY_MEASURE);
	}
	
}


int16_t temp_hum_getTemperature(void){
	if(hih8120_isReady()){
		return hih8120_getTemperature_x10();
	}
}

uint16_t temp_hum_getHumidity(void){
	if(hih8120_isReady()){
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
