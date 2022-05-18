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


typedef enum temp_hum_statusCodes {
	HIH8120_OK	/**< Everything went well */
	,HIH8120_OUT_OF_HEAP /**< Not enough heap to initialize the driver */
	,HIH8120_DRIVER_NOT_INITIALISED /**< Driver must be initialize before use */
	,HIH8120_TWI_BUSY /**< The two wire/I2C interface is busy */
} temp_hum_statusCodes_t;


#ifndef TEMP_HUM_SENSOR_H_
#define TEMP_HUM_SENSOR_H_

void temp_hum_sensorInitialize(UBaseType_t task_priority);

void temp_hum_sensorTaskHandler(void *pvParameters);

int16_t temp_hum_GetTemperature(void);

uint16_t temp_hum_GetHumidity(void);

#endif /* TEMP_HUM_SENSOR_H_ */