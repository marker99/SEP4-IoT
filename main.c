/*
* main.c
* Author : IHA
* Modified by: Sander
* Clarifying Comments: Andreas
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
#include <rc_servo.h>

#include "application.h"
#include "LoRaWANHandler.h"
// Data Object
#include "datastructures/measurment.h"

void initialiseSystem()
{
	// Set output ports for LEDs
	DDRA |= _BV(DDA0) | _BV(DDA7);
	
	// Make it possible to use STDIO on COM port 0 (USB) on Arduino board - Setting 57600, 8, N, 1
	stdio_initialise(ser_USART0);

	// vvvvvvvvvvvvvvvvv BELOW IS LoRaWAN initialization vvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
	// Status LEDs driver
	status_leds_initialise(5); // Priority 5 for internal task
	
	// Initialize LoRaWAN
	lora_handler_initialize(1);
/*
should be removed as we dont use it 
	// Initialize Display drivers
	display_7seg_initialise(NULL);
	
	// Power up the display
	display_7seg_powerUp();
*/
	// Initialize Event Groups [ Measurement ready | Start measuring ]
	EventGroupHandle_t readyGroup = xEventGroupCreate();
	EventGroupHandle_t startGroup = xEventGroupCreate();

	
	// Initialize the Application, along with the Sensors
	application_initialize(1, readyGroup, startGroup);
	temp_hum_sensor_initialize(2, readyGroup, startGroup);
	co2_sensor_initialize(2, readyGroup, startGroup);
}


/*-----------------------------------------------------------*/
int main(void)
{
	initialiseSystem(); // Must be done as the very first thing

	// Informing Users - See, we are more interactive than a the Java JAX WebService
	printf("main: Program Started\n");
	
	// Initialize and run the freeRTOS scheduler. Execution should never return from here.
	vTaskStartScheduler();
	
	// If reached, an Error has occurred in the vTaskStartScheduler
	printf("The Application has somehow Crashed...\nPlease Restart the Device...\n");
	while(1){};
}


