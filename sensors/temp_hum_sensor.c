/*
 * temp_hum_sensor.c
 *
 * Created: 12-05-2022 07:33:43
 *  Author: sande
 */ 

#include "temp_hum_sensor.h"



void temp_hum_sensor_initialize(UBaseType_t task_priority)
{
	// Initialize temp/hum
	if ( HIH8120_OK != hih8120_initialise() )
	{
		
		printf("failed initializing temp/hum \n");
	}

}



int16_t GetTemerature(void)
{

}

uint16_t GetHumidity(void)
{

}
