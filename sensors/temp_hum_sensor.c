/*
* temp_hum_sensor.c
*
* Created: 12-05-2022 07:33:43
*  Author: Sander
*/

#define DEVICE "HIH8129 [Temperature & Humidity]"

#include "temp_hum_sensor.h"
#include "event_group_config.h"
#include <status_leds.h>
#include "util/thread_safe_printf.h"
static EventGroupHandle_t _dataReadyEventGroup;
static EventGroupHandle_t _measureEventGroup;

static TickType_t _xLastWakeTime;

// declaration of private methods
static void temp_hum_sensor_startMeasure();



void temp_hum_sensor_initialize(UBaseType_t task_priority, EventGroupHandle_t readyGroup, EventGroupHandle_t measureGroup)
{
	// Initialize Temperature & Humidity Sensor
	if ( HIH8120_OK == hih8120_initialise() )
	{
		_dataReadyEventGroup = readyGroup;
		_measureEventGroup = measureGroup;

		xTaskCreate(
		temp_hum_sensor_task_handler,
		"Temp/hum_Sensor_Task",
		configMINIMAL_STACK_SIZE,
		NULL,
		task_priority,
		NULL);
		
		status_leds_ledOn(led_ST3);
	}else{
		
		printf("Failed to Initialize %s Sensor\n", DEVICE);
	}
}


void temp_hum_sensor_task_init(){
	thread_safe_printf("+ %s\n", DEVICE);
	_xLastWakeTime =  xTaskGetTickCount();
}


static void temp_hum_sensor_startMeasure(){
	// Inform User a Measurement is starting
	thread_safe_printf("%s Sensor started measuring\n", DEVICE);
	status_leds_fastBlink(led_ST3);
	
	// Small Delay
	vTaskDelay(pdMS_TO_TICKS(50));
	
	// Check if Sensor is able to Wake Up
	if (HIH8120_OK != hih8120_wakeup()){
		thread_safe_printf("%s did not wake up\n", DEVICE);
	}
	
	// Small Delay
	vTaskDelay(pdMS_TO_TICKS(50));

	// Check if a Measurement could be taken
	if (HIH8120_OK != hih8120_measure()){
		thread_safe_printf("%s measurement went wrong\n", DEVICE);
	}
	
	// Small Delay
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
	
	// Indicate when Ready to Measure
	if(hih8120_isReady()){
		// Set the ready measurment bit in event group
		status_leds_ledOn(led_ST3);
		xEventGroupSetBits(_dataReadyEventGroup, BIT_TEMP_HUM_READY_MEASURE);
	}
}

/*
** Get the Temperature as a 10x scaled value
** This means at 23.5c this will return 235
*/
int16_t temp_hum_getTemperature(void){
	if(hih8120_isReady()){
		return hih8120_getTemperature_x10();
	}
}

/*
** Get the Relative Humidity as a 10x scaled value
** This means at 24.0% relative humidity this will return 240
*/
uint16_t temp_hum_getHumidity(void){
	if(hih8120_isReady()){
		return hih8120_getHumidityPercent_x10();
	}
	return 0;
}

void temp_hum_sensor_task_handler(void *pvParameters)
{
	// Initialize Sensor Task
	temp_hum_sensor_task_init();
	
	// Run Sensor Task
	for (;;)
	{
		temp_hum_sensor_task_run();
	}
}
