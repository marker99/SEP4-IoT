/*
* main.c
* Author : IHA
* Example main file including LoRaWAN setup
* Just for inspiration :)
*/

#include <stdio.h>

#include <ATMEGA_FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <stdio_driver.h>
#include <serial.h>
#include <status_leds.h>

#include "application.h"
#include "LoRaWANHandler.h"
//data object
#include "datastructures/measurment.h"

void initialiseSystem()
{
	// Set output ports for leds
	DDRA |= _BV(DDA0) | _BV(DDA7);
	// Make it possible to use stdio on COM port 0 (USB) on Arduino board - Setting 57600,8,N,1
	stdio_initialise(ser_USART0);
	// maybe sei() ? for global interrupts

	// vvvvvvvvvvvvvvvvv BELOW IS LoRaWAN initialisation vvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
	// Status Leds driver
	status_leds_initialise(5); // Priority 5 for internal task
	
	// initialize lorawan 
	lora_handler_initialize(1);

	// initialize Display drivers
	display_7seg_initialise(NULL);
	
	// Power up the display
	display_7seg_powerUp();

	EventGroupHandle_t readyGroup = xEventGroupCreate();
	EventGroupHandle_t startGroup = xEventGroupCreate();

	application_initialize(1, readyGroup, startGroup);
	temp_hum_sensor_initialize(1, readyGroup, startGroup);
	co2_sensor_initialize(1, readyGroup, startGroup);
}


/*-----------------------------------------------------------*/
int main(void)
{
	initialiseSystem(); // Must be done as the very first thing
	printf("main: Program Started  \n");
	
	vTaskStartScheduler(); // Initialize and run the freeRTOS scheduler. Execution should never return from here.
	
	printf("I was here: not good ! \n");
	while(1){};
}


