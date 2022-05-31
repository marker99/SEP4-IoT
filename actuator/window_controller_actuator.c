/*
* window_controller_actuator.c
*
* Created: 30-05-2022 19:12:32
*  Author: Sander
*/
#include <ATMEGA_FreeRTOS.h>
#include <rc_servo.h>
#include <stdbool.h>
#include <task.h>

#include "headers/window_controller.h"
#include "configuration_settings.h"
#include "time_interval_config.h"
#include "temp_hum_sensor.h"
#include "co2_sensor.h"
#include "util/thread_safe_printf.h"


static TickType_t _xLastWakeTime;
static const TickType_t xFrequency = WINDOW_CONTROLLER_REQUENCY;


static bool _window_is_closed;

//private methods
static void _close_window();
static void _open_window();
static bool _humidity_threshold_exceeded();
static bool _co2_threshold_exceeded();
static bool _max_temperature_exceeded();
static bool _min_temperature_exceeded();

void window_controller_actuator_initlizer(UBaseType_t taskPriority){
	
	rc_servo_initialise();
	
	xTaskCreate(window_controller_actuator_task,
	"WindowControllerTask",
	configMINIMAL_STACK_SIZE+200,
	NULL,
	taskPriority,
	NULL);
}

void window_controller_actuator_task_init(){
	
	vTaskDelay(WINDOW_CONTROLLER_TASK_START_OFFSET);

	_xLastWakeTime = xTaskGetTickCount();
}


void window_controller_actuator_task_run(){
	xTaskDelayUntil(&_xLastWakeTime, xFrequency);
	thread_safe_printf("\n>Window Controller < : check indoor climate!\n");
	
	
	if (_co2_threshold_exceeded() || _max_temperature_exceeded() ){
		thread_safe_printf(">Window Controller < : limit exceeded!\n");
		_open_window();
	}else if (_min_temperature_exceeded()){
		_close_window();
	} else if ( _humidity_threshold_exceeded()){
		_open_window();
	}
	
		
	UBaseType_t uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);
	thread_safe_printf("\n>window controller end Stack Size %d\n", uxHighWaterMark);
}


void window_controller_actuator_task(void *pvParameters){
	
	window_controller_actuator_task_init();

	for (;;)
	{
		window_controller_actuator_task_run();
	}
	
}

static bool _humidity_threshold_exceeded(){
	return temp_hum_getHumidity() > configMutex_getHumidityThreshold();
}

static bool _co2_threshold_exceeded(){
	return co2_sensor_getCO2() > configMutex_getCO2Threshold();
}

static bool _max_temperature_exceeded(){
	return temp_hum_getTemperature()  > configMutex_getTargetTemperature() + configMutex_getTemperatureMargin();
}

static bool _min_temperature_exceeded(){
	return temp_hum_getTemperature()  < configMutex_getTargetTemperature() - configMutex_getTemperatureMargin();
}


static void _close_window(){
	rc_servo_setPosition( SERVO_NO, -100);
	_window_is_closed = 1;
}

static void _open_window(){
	rc_servo_setPosition( SERVO_NO, 100);
	_window_is_closed = 0;
}