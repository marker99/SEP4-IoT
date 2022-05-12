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
#include <>

typedef struct application_task{
	
	EventGroupHandle_t data_ready_event_group;
	EventGroupHandle_t measure_event_group;
	
	

} application_task_t;



application_task_t application_task_initialize(UBaseType_t task_priority){
	
	
	

	xTaskCreate(
	 application_task,
	 "Application",
	 configMINIMAL_STACK_SIZE,
	 (application_task_t) 1,
	 task_priority,
	 NULL
	)
}


void application_task(application_task_t application){
	printf("The Application task has started")
	application.data_ready_event_group->
	
	for (;;){	
		
	}
}