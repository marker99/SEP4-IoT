/*
 *
 * Created: 08-05-2022 21:04:31
 *  Author: Pander
 */ 


#ifndef MEASURMENT_H_
#define MEASURMENT_H_

typedef struct measurment* pMeasurment_t;

typedef struct measurment
{
	int16_t temperatur;
	uint16_t humidity;
	uint16_t co2_ppm;
	
} measurment_t;


#endif 