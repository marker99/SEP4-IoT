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
    return _targetTemperature;

    // Are Semaphores needed for simply Reading the value and returning? Is it not enough to just return the "Field Variable"?
    /*
    // Initialize Temperature as 0
    int16_t temperature = 0;
    
    // Trying to see if the semaphore can be obtained, if true the resource can then be accessed
    if(xSemaphoreTake(semaphore, portMAX_DELAY) == pdTRUE){
    temperature = _targetTemperature;
    }
    
    // Finished with the resource, the semaphore is then released again
    xSemaphoreGive(semaphore);
    return temperature;
    */
}

int16_t configMutex_getTemperatureMargin(){
    return _temperatureMargin;
    /*
    // TODO: ??
    int16_t margin = 0;
    
    if(xSemaphoreTake(semaphore, portMAX_DELAY) == pdTRUE){
    margin = _temperatureMargin;
    }
    
    xSemaphoreGive(semaphore);
    return margin;
    */
}

uint16_t configMutex_getHumidityThreshold(){
    return _humidityThreshold;
    /*
    // Initialize Humidity to 0
    uint16_t humidity = 0;
    
    if(xSemaphoreTake(semaphore, portMAX_DELAY) == pdTRUE){
    humidity = _humidityThreshold;
    }
    
    xSemaphoreGive(semaphore);
    return humidity;
    */
}

uint16_t configMutex_getCO2Threshold(){
    return _co2Threshold;
    /*
    // Initialize CO2 to 0
    uint16_t co2 = 0;
    
    if(xSemaphoreTake(semaphore, portMAX_DELAY) == pdTRUE){
    co2 = _co2Threshold;
    }
    
    xSemaphoreGive(semaphore);
    return co2;
    */
}

void configMutex_setTargetTemperature(int16_t tagetTemp){
    // Obtain Semaphore to ensure Write access is only allowed from here
    if (xSemaphoreTake(semaphore, portMAX_DELAY) == pdTRUE){
        _targetTemperature = tagetTemp;
    }

    // Drop Semaphore to allow other places to Claim Access to the Data
    xSemaphoreGive(semaphore);
}

void configMutex_setTemperatureMargin(int16_t tempMargin){
    // Obtain Semaphore to ensure Write access is only allowed from here
    if(xSemaphoreTake(semaphore, portMAX_DELAY) = pdTRUE){
        _temperatureMargin = tempMargin;
    }

    // Drop Semaphore to allow other places to Claim Access to the Data
    xSemaphoreGive(semaphore);
}

void configMutex_setHumidityThreshold(uint16_t humThreshold){
    // Obtain Semaphore to ensure Write access is only allowed from here
    if(xSemaphoreTake(semaphore, portMAX_DELAY) = pdTRUE){
        _humidityThreshold = humThreshold;
    }
    
    // Drop Semaphore to allow other places to Claim Access to the Data
    xSemaphoreGive(semaphore);
}

void configMutex_setCO2Threshold(uint16_t co2Threshold){
    // Obtain Semaphore to ensure Write access is only allowed from here
    if (xSemaphoreTake(semaphore, portMAX_DELAY) = pdTRUE){
        _co2Threshold = co2Threshold;
    }
    
    // Drop Semaphore to allow other places to Claim Access to the Data
    xSemaphoreGive(semaphore);
}

