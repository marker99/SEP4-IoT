/*
* Application.c
*
* Created: 10-05-2022 10:54:35
*  Author: Sander
*/

#include "application.h"
#include "application_config.h"

#include "sensors/temp_hum_sensor.h"
#include "sensors/co2_sensor.h"
#include "loraWan_up_link_handler.h"
#include "datastructures/measurment.h"


static EventGroupHandle_t _dataReadyEventGroup;
static EventGroupHandle_t _measureEventGroup;

static TickType_t xLastWakeTime;
const TickType_t xFrequency = pdMS_TO_TICKS(30000);


void application_initialize(UBaseType_t task_priority, EventGroupHandle_t readyGroup, EventGroupHandle_t startGroup){
	
	_dataReadyEventGroup = readyGroup;
	_measureEventGroup = startGroup;
	
	xTaskCreate(
	application_task_handler,
	"Application",
	configMINIMAL_STACK_SIZE,
	NULL,
	task_priority,
	NULL
	);
}


void application_task_handler(void *pvParameters){
	printf("The Application task has started");
	xLastWakeTime = xTaskGetTickCount();

	application_task_init();
	
	for (;;){
		application_task_run();
	
	}
}

void application_task_init(){
	
	
}


void application_task_run(){
	
	xTaskDelayUntil(&xLastWakeTime, xFrequency);
	
	start_measuring();
	
	wait_for_temp_hum_measurment();
	wait_for_co2_measurment();
	measurment_t measurment ;
	
	measurment.humidity = temp_hum_getTemperature();
	measurment.tempratur = temp_hum_getHumidity();
	measurment.co2_ppm = co2_sensor_getCO2();
	
	send_measurment(&measurment);
}


static void start_measuring(){
	
	xEventGroupSetBits(
	_measureEventGroup,
	BIT_START_MEASURE
	);
}

static void wait_for_temp_hum_measurment(){
	xEventGroupWaitBits(
	_dataReadyEventGroup, 
	BIT_TEMPHUM_READY_MEASURE, 
	pdFALSE, 
	pdTRUE, 
	portMAX_DELAY
	);
}

static void wait_for_co2_measurment(){
	xEventGroupWaitBits(
	_dataReadyEventGroup,
	BIT_CO2_READY_MEASURE,
	pdFALSE,
	pdTRUE,
	portMAX_DELAY
	);
}

