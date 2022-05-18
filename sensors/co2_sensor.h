/*
 * co2_sensor.h
 *
 * Created: 12-05-2022 07:10:04
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
#include <mh_z19.h>

#ifndef co2_sensor_H_
#define co2_sensor_H_

void co2_sensor_initialize(UBaseType_t task_priority);

void co2_sensor_taskHandler(void *pvParamerters);

void _co2_sensor_callback(uint16_t ppm);

#endif /* co2_sensor_H_ */