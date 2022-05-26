/*
* Application.h
*
* Created: 23-05-2022 10:51:33
*  Author: Sander
*/


#ifndef APPLICATION_H_
#define APPLICATION_H_

#include <ATMEGA_FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <event_groups.h>


void application_initialize(UBaseType_t task_priority, EventGroupHandle_t readyGroup, EventGroupHandle_t startGroup);

void application_task_handler(void *pvParameters);

void application_task_init();

void application_task_run();



#endif /* APPLICATION_H_ */