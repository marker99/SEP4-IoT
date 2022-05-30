/*
 * window_controller.h
 *
 * Created: 30-05-2022 19:06:19
 *  Author: Sander
 */ 


#ifndef WINDOW_CONTROLLER_H_
#define WINDOW_CONTROLLER_H_
#include <ATMEGA_FreeRTOS.h>

#define SERVO_NO 1 //J13 (Servo#1))

void window_controller_actuator_initlizer(UBaseType_t taskPriority);

void window_controller_actuator_task_run();

void window_controller_actuator_task(void *pvParameters);


#endif /* WINDOW_CONTROLLER_H_ */