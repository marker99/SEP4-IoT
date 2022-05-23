/*
 * Application.h
 *
 * Created: 23-05-2022 10:51:33
 *  Author: Sander
 */ 


#ifndef APPLICATION_H_
#define APPLICATION_H_


application_initialize(UBaseType_t task_priority, EventGroupHandle_t readyGroup, EventGroupHandle_t startGroup);

void applicaiton_task_handler(void *pvParameters);

void applicaiton_task_init();

void applicaiton_task_run();


#endif /* APPLICATION_H_ */