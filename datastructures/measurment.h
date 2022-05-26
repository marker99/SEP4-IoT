/*
 *
 * Created: 08-05-2022 21:04:31
 *  Author: Pander
 */ 


#ifndef MEASURMENT_H_
#define MEASURMENT_H_

#include <stdint.h>

typedef struct measurment* pMeasurment_t;

typedef struct measurment
{
	int16_t temperature;
	uint16_t humidity;
	uint16_t co2PartsPrMillion;
	
} measurment_t;


#endif 