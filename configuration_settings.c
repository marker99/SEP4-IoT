/*
* configuration_settings.c
*
* Created: 26-05-2022 13:58:09
*  Author: Frederik
*/

#include <stdint.h>
#include "ATMEGA_FreeRTOS.h"
#include <semphr.h>

static int16_t _targetTemperature;
static int16_t _temperatureMargin;
static uint16_t _humidityThreshold;
static uint16_t _co2Threshold;

static SemaphoreHandle_t semaphore;

void configMutex_create(){
	semaphore = xSemaphoreCreateMutex();
}

int16_t configMutex_getTargetTemperature(){
	
	int16_t temperature = 0;
	
	// Trying to see if the semaphore can be obtained, if true the resource can then be accessed
	if(xSemaphoreTake(semaphore, portMAX_DELAY) == pdTRUE){
		temperature = _targetTemperature;
	}
	
	// Finished with the resource, the semaphore is then released again
	xSemaphoreGive(semaphore);
	return temperature;
	
}

int16_t configMutex_getTemperatureMargin(){
	
	int16_t temperature = 0;
	
	if(xSemaphoreTake(semaphore, portMAX_DELAY) == pdTRUE){
		temperature = _temperatureMargin;
	}
	
	xSemaphoreGive(semaphore);
	return temperature;
}

uint16_t configMutex_getHumidityThreshold(){
	
	uint16_t humidity = 0;
	
	if(xSemaphoreTake(semaphore, portMAX_DELAY) == pdTRUE){
		humidity = _humidityThreshold;
	}
	
	xSemaphoreGive(semaphore);
	return humidity;
	
}

uint16_t configMutex_getCO2Threshold(){
	
	uint16_t co2;
	
	if(xSemaphoreTake(semaphore, portMAX_DELAY) == pdTRUE){
		co2 = _co2Threshold;
	}
	
	xSemaphoreGive(semaphore);
	return co2;
	
}

void configMutex_setTargetTemperature(int16_t tagetTemp){
	
	if (xSemaphoreTake(semaphore, portMAX_DELAY) == pdTRUE){
		_targetTemperature = tagetTemp;
	}
	xSemaphoreGive(semaphore);
	
}

void configMutex_setTemperatureMargin(int16_t tempMargin){
	
	if(xSemaphoreTake(semaphore, portMAX_DELAY) = pdTRUE){
		_temperatureMargin = tempMargin;
	}
	xSemaphoreGive(semaphore);
	
}

void configMutex_setHumidityThreshold(uint16_t humThreshold){
	
	if(xSemaphoreTake(semaphore, portMAX_DELAY) = pdTRUE){
		_humidityThreshold = humThreshold;
	}
	xSemaphoreGive(semaphore);
	
}

void configMutex_setCO2Threshold(uint16_t co2Threshold){
	
	if (xSemaphoreTake(semaphore, portMAX_DELAY) = pdTRUE){
		_co2Threshold = co2Threshold;
	}
	xSemaphoreGive(semaphore);
	
}

