/*
 * Application.c
 *
 * Created: 10-05-2022 10:54:35
 *  Author: Sander
 */ 

#include <ATMEGA_FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <event_groups.h>

static EventGroupHandle_t _dataReadyEventGroup;
static EventGroupHandle_t _measureEventGroup;

application_initialize(UBaseType_t task_priority, EventGroupHandle_t readyGroup, EventGroupHandle_t startGroup){
	
	_dataReadyEventGroup = readyGroup;
	_measureEventGroup = startGroup;
	
	xTaskCreate(
	 application_task,
	 "Application",
	 configMINIMAL_STACK_SIZE,
	 NULL,
	 task_priority,
	 NULL
	)
}


void application_task( ){
	printf("The Application task has started")
	
	for (;;){	
		
	}
}