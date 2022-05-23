/*
 * co2_sensor.c
 *
 * Created: 12-05-2022 07:19:51
 *  Author: sande
 */ 

#include "co2_sensor.h"


static EventGroupHandle_t _dataReadyEventGroup;
static EventGroupHandle_t _measureEventGroup;
static int16_t CO2;
static TickType_t xLastWakeTime;

int16_t getCO2(){
	return CO2;
}

static void CO2_wakeupAndMeasure(){
	xTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(50));
	
	mh_z19_returnCode_t returnCode;
	
	if(MHZ19_OK != (returnCode = mh_z19_takeMeassuring())){
		printf("CO2 sensor failed to measure.")
	}
	
	xTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(50));
	
	if(MHZ19_OK != (returnCode = mh_z19_getCo2Ppm(CO2))){
		printf"("CO" sensor failed to get the CO2 value."
	}
	
	xTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(10));
}
static uint16_t measuredCO2;

void co2_sensor_initialize(UBaseType_t task_priority, EventGroupHandle_t readyGroup, EventGroupHandle_t measureGroup)
{
	
	//  
	if(MHZ19_OK != mh_z19_initialise(ser_USART3)){
		printf("failed initializing CO2 \n");
	}
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
	*measuredCO2 = *ppm
	// could set a variable, and/or set the "measurment event group" to done;
}

void _co2_sensor_task_init(){
	
}

void _co2_sensor_task_run(){
	
	xEventGroupWaitBits(_measureEventGroup,
	BIT_CO2_READY_MEASURE,
	pdFALSE,
	pdTRUE,
	portMAX_DELAY);
	
	CO2_wakeupAndMeasure();
	
	int16_t CO2 = mh_z19_getCo2Ppm(CO2);
	
	xEventGroupSetBits(_readingsReadyEventGroup, BIT_CO2_READY_MEASURE);
	
	xTaskDelayUntil( &xLastWakeTime, pdMS_TO_TICKS(50));
		
	
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

uint16_t co2_sensor_getCO2(){
	return measuredCO2
}

