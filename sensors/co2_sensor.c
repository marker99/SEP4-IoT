/*
 * co2_sensor.c
 *
 * Created: 12-05-2022 07:19:51
 *  Author: sande
 */ 

#include "co2_sensor.h"
#include "../application_config.h"

static EventGroupHandle_t _dataReadyEventGroup;
static EventGroupHandle_t _measureEventGroup;
static TickType_t xLastWakeTime;

uint16_t measuredCO2;

// declaration of private methods
static void CO2_wakeupAndMeasure();



void co2_sensor_initialize(UBaseType_t task_priority, EventGroupHandle_t readyGroup, EventGroupHandle_t measureGroup)
{
	
	mh_z19_initialise(ser_USART3);
	
	mh_z19_injectCallBack(_co2_sensor_callback);
	
	_dataReadyEventGroup = readyGroup;
	_measureEventGroup = measureGroup;
	
	xTaskCreate(
	co2_sensor_taskHandler,
	"CO2_Sensor", 
	configMINIMAL_STACK_SIZE, 
	NULL, 
	task_priority, 
	NULL);
}

void _co2_sensor_callback(uint16_t ppm)
{
	measuredCO2 = ppm;
	xEventGroupSetBits(_dataReadyEventGroup, BIT_CO2_READY_MEASURE);
}

void _co2_sensor_task_init(){
	printf("Co2 Sensor ready\n");
	xLastWakeTime = xTaskGetTickCount();
}

void _co2_sensor_task_run(){
	
	xEventGroupWaitBits(_measureEventGroup,
	BIT_START_CO2_MEASURE,
	pdTRUE,
	pdTRUE,
	portMAX_DELAY);
	
	CO2_wakeupAndMeasure();
	
	vTaskDelay(pdMS_TO_TICKS(50));
}

uint16_t co2_sensor_getCO2(){
	return measuredCO2;
}


void co2_sensor_taskHandler(void *pvParamerters)
{

	_co2_sensor_task_init();
	
	for(;;)
	{
		_co2_sensor_task_run();
	}
}



static void CO2_wakeupAndMeasure(){
	printf("co2 sensor started measuring\n");
	vTaskDelay(pdMS_TO_TICKS(50));
	
	mh_z19_returnCode_t returnCode;
	
	if(MHZ19_OK != (returnCode = mh_z19_takeMeassuring())){
		printf("CO2 sensor failed to measure.\n");
	}
	
	vTaskDelay(pdMS_TO_TICKS(50));
}




