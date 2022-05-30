/*
* main.c
* Author : IHA
* Modified by: Sander
* Clarifying Comments: Andreas
* Example main file including LoRaWAN setup
*/

#include <ATMEGA_FreeRTOS.h>
#include <task.h>
//#include <semphr.h>
#include <serial.h>
#include <status_leds.h>
#include <stdio.h>
//#include <rc_servo.h>
#include "util/thread_safe_printf.h"
#include "application.h"
#include "LoRaWANHandler.h"
#include "sensors/headers/temp_hum_sensor.h"
#include "sensors/headers/co2_sensor.h"
#include "actuator/headers/window_controller.h"
#include "configuration_settings.h"
#include "loraWan/headers/LoRaWANHandler.h"

// Data Object
#include "datastructures/measurment.h"

void initialiseSystem()
{
	// Set output ports for LEDs
	DDRA |= _BV(DDA0) | _BV(DDA7);
	
	// thread safe stdio printf
	thread_safe_printf_initialize();
	
	//create config settings mutex
	configMutex_create();
	
	// vvvvvvvvvvvvvvvvv BELOW IS LoRaWAN initialization vvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
	// Status LEDs driver
	status_leds_initialise(5); // Priority 5 for internal task
	
	// Initialize LoRaWAN
	lora_handler_initialize(1);
	
	// Initialize Event Groups [ Measurement ready | Start measuring ]
	EventGroupHandle_t readyGroup = xEventGroupCreate();
	EventGroupHandle_t startGroup = xEventGroupCreate();


	// Initialize the Application, along with the Sensors
	window_controller_actuator_initlizer(2);
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
	thread_safe_printf("The Application has somehow Crashed...\nPlease Restart the Device...\n");
	while(1){};
}


