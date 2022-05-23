/*
 * co2_sensor.c
 *
 * Created: 12-05-2022 07:19:51
 *  Author: sande
 */ 

#include "co2_sensor.h"


static EventGroupHandle_t _dataReadyEventGroup;
static EventGroupHandle_t _measureEventGroup;

void co2_sensor_initialize(UBaseType_t task_priority, EventGroupHandle_t readyGroup, EventGroupHandle_t measureGroup)
{
	
	//  
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
	// could set a variable, and/or set the "measurment event group" to done;
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


