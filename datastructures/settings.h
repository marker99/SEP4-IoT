/*
 * settings.h
 *
 * Created: 26-05-2022 14:29:16
 *  Author: sander
 */ 


#ifndef SETTINGS_H_
#define SETTINGS_H_

typedef	struct settings* pSettings_t; 

typedef struct settings
{
	int16_t temperatur_target;
	int16_t temperatur_margin;
	uint16_t humidity;
	uint16_t co2_ppm;
	
} settings_t;


#endif /* SETTINGS_H_ */