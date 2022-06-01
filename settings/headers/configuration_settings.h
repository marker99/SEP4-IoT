/*
* configuration_settings.h
*
* Created: 26-05-2022 13:57:23
*  Author: Frederik
*/


#include <stdint.h>

#ifndef CONFIGURATION_SETTINGS_H_
#define CONFIGURATION_SETTINGS_H_

// Mutex created for the configuration settings
void configMutex_create();

int16_t configMutex_getTargetTemperature();
int16_t configMutex_getTemperatureMargin();
uint16_t configMutex_getHumidityThreshold();
uint16_t configMutex_getCO2Threshold();

void configMutex_setTargetTemperature(int16_t tagetTemp);
void configMutex_setTemperatureMargin(int16_t tempMargin);
void configMutex_setHumidityThreshold(uint16_t humThreshold);
void configMutex_setCO2Threshold(uint16_t co2Threshold);

#endif /* CONFIGURATION_SETTINGS_H_ */