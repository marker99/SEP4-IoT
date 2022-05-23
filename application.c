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
	
	application_task_init();
	
	for (;;){
		application_task_run();
	
	}
}

void application_task_init(){
	printf("The Application task has started\n");
	xLastWakeTime = xTaskGetTickCount();
}


void application_task_run(){
	
	xTaskDelayUntil(&xLastWakeTime, xFrequency);
	printf("Start measurment\n");
	start_measuring();
	printf("Waiting for sensors to set ready event group\n");
	
	wait_for_co2_measurment();
	printf("Application: Co2 measurment is ready\n");
	
	
	wait_for_temp_hum_measurment();
	printf("Application: TempHum measurement is ready\n");
	
	//might lead to memory leak if not destroyed ?
	measurment_t measurment;
	printf("Get measured data");
	measurment.humidity = temp_hum_getTemperature();
	measurment.tempratur = temp_hum_getHumidity();
	measurment.co2_ppm = co2_sensor_getCO2();
	printf("Send measurment !");
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
	pdTRUE, 
	pdTRUE, 
	portMAX_DELAY
	);
}

static void wait_for_co2_measurment(){
	xEventGroupWaitBits(
	_dataReadyEventGroup,
	BIT_CO2_READY_MEASURE,
	pdTRUE,
	pdTRUE,
	portMAX_DELAY
	);
}

