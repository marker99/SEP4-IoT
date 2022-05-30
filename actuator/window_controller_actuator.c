/*
 * window_controller_actuator.c
 *
 * Created: 30-05-2022 19:12:32
 *  Author: Sander
 */ 

#include "headers/window_controller.h"
#include "configuration_settings.h"

#include <rc_servo.h>

#include <ATMEGA_FreeRTOS.h>
#include <task.h>




void window_controller_actuator_initlizer(UBaseType_t taskPriority){
	
	rc_servo_initialise();
	rc_servo_setPosition( SERVO_NO, -100);
	
	xTaskCreate(window_controller_actuator_task, 
	"Window controller task",
	configMINIMAL_STACK_SIZE,
	NULL,
	taskPriority,
	NULL);
}



void window_controller_actuator_task_run(void){
	
	
}


void window_controller_actuator_task(void *pvParameters){

	for (;;)
	{
		 window_controller_actuator_task_run();		 
	}
	
}