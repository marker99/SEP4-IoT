/*
* co2_sensor.h
*
* Created: 12-05-2022 07:10:04
*  Author: Sander
*/

// FreeRTOS Includes
#include <ATMEGA_FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <event_groups.h>


// IO Includes
#include <stdio.h>

// Drivers for sensors
#include <mh_z19.h>


#ifndef co2_sensor_H_
#define co2_sensor_H_

void co2_sensor_initialize(UBaseType_t task_priority, EventGroupHandle_t readyGroup, EventGroupHandle_t startGroup);

void co2_sensor_taskHandler(void *pvParamerters);

void _co2_sensor_callback(uint16_t ppm);

void _co2_sensor_task_init();

void _co2_sensor_task_run();

uint16_t co2_sensor_getCO2();


#endif /* co2_sensor_H_ */