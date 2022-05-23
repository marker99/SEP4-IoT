/*
* temp_hum_sensor.c
*
* Created: 12-05-2022 07:33:43
*  Author: sande
*/

#include "temp_hum_sensor.h"
#include "../application_config.h"


static EventGroupHandle_t _dataReadyEventGroup;
static EventGroupHandle_t _measureEventGroup;

static TickType_t xLastWakeTime;
static TickType_t xFrequency = pdMS_TO_TICKS(60000);


static int16_t measuredTemperature;
static uint16_t measuredHumidity;

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
	printf("hum_Sensor Sensor ready\n");
	xLastWakeTime =  xTaskGetTickCount();
}


static void temp_hum_sensor_startMeasure(){
	printf("temp hum sensor started measuring\n");
	
	xTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(50));
	
	if (HIH8120_OK != hih8120_wakeup()){
		printf("temp/hum did not wake up\n");
	}else{
		printf("hih8120 woke up\n");
	}
	
	xTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(100));

	
	if (HIH8120_OK != hih8120_measure()){
		printf("the measurement went wrong\n");
	}else{
		printf("hih8120 started a measurement \n");
	}
	
	xTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(500));
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
		printf("Temp hum measurment is ready !\n");
		measuredTemperature = hih8120_getTemperature_x10();
		measuredHumidity = hih8120_getHumidityPercent_x10();
		
		// set the ready measurment bit in event group
		xEventGroupSetBits(_dataReadyEventGroup, BIT_TEMPHUM_READY_MEASURE);
	}
	
}


int16_t temp_hum_getTemperature(void){
	return measuredTemperature;
}

uint16_t temp_hum_getHumidity(void){
	return measuredHumidity;
}

void temp_hum_sensor_task_handler(void *pvParameters)
{

	temp_hum_sensor_task_init();
	
	for (;;)
	{
		temp_hum_sensor_task_run();
	}

}
