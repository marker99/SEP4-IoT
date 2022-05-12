/*
 * temp_hum_sensor.h
 *
 * Created: 12-05-2022 07:28:46
 *  Author: sande
 */ 
// freeRtos include
#include <ATMEGA_FreeRTOS.h>
#include <task.h>
#include <semphr.h>

// IO includes
#include <stdio.h>
#include <avr/io.h>
#include <serial.h>

// drivers for sensors
#include <hih8120.h>
#ifndef TEMP_HUM_SENSOR_H_
#define TEMP_HUM_SENSOR_H_

void temp_hum_sensor_initialize(UBaseType_t task_priority);


void temp_hum_sensor_task_handler(void *pvParameters);

int16_t GetTemerature(void);


uint16_t GetHumidity(void);



#endif /* TEMP_HUM_SENSOR_H_ */