/*
* main.c
* Author : IHA
*
* Example main file including LoRaWAN setup
* Just for inspiration :)
a test from the new structure branch
*/

#include <stdio.h>
//#include <avr/io.h> 

#include <ATMEGA_FreeRTOS.h>
#include <task.h>
#include <semphr.h>

#include <stdio_driver.h>
#include <serial.h>

// Needed for LoRaWAN
#include <lora_driver.h>
#include <status_leds.h>

#include <display_7seg.h>
#include <hih8120.h>
#include <mh_z19.h>

//data object
#include "measurment.h"

// define two Tasks
void task1( void *pvParameters );
void task2( void *pvParameters );
void taskHandler(void *pvParameters);

// define semaphore handle
SemaphoreHandle_t xTask1Semaphore;
SemaphoreHandle_t xTask2Semaphore;

// Prototype for LoRaWAN handler
void lora_handler_initialise(UBaseType_t lora_handler_task_priority);

/*-----------------------------------------------------------*/
void create_tasks_and_semaphores(void)
{
	// Semaphores are useful to stop a Task proceeding, where it should be paused to wait,
	// because it is sharing a resource, such as the Serial port.
	// Semaphores should only be used whilst the scheduler is running, but we can set it up here.
	if ( xTask1Semaphore == NULL )  // Check to confirm that the Semaphore has not already been created.
	{
		xTask1Semaphore = xSemaphoreCreateMutex();  // Create a mutex semaphore.
		if ( ( xTask1Semaphore ) != NULL )
		{
			// dont give
		}
	}

	if ( xTask2Semaphore == NULL )  // Check to confirm that the Semaphore has not already been created.
	{
		xTask2Semaphore = xSemaphoreCreateMutex();  // Create a mutex semaphore.
		if ( ( xTask2Semaphore ) != NULL )
		{
			// dont give
		}
	}


	xTaskCreate(
	task1
	,  "CO2_sensor"  // A name just for humans
	,  configMINIMAL_STACK_SIZE  // This stack size can be checked & adjusted by reading the Stack Highwater
	,  NULL
	,  1  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
	,  NULL );

	xTaskCreate(
	task2
	,  "Temp/hum_sensor"  // A name just for humans
	,  configMINIMAL_STACK_SIZE  // This stack size can be checked & adjusted by reading the Stack Highwater
	,  NULL
	,  1  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
	,  NULL );
	
	xTaskCreate(
	taskHandler
	,  "Task handler"  // A name just for humans
	,  configMINIMAL_STACK_SIZE  // This stack size can be checked & adjusted by reading the Stack Highwater
	,  NULL
	,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
	,  NULL );
	
}

/*-----------------------------------------------------------*/
void task1( void *pvParameters )
{
	
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 3000/portTICK_PERIOD_MS;

	// Initialise the xLastWakeTime variable with the current time.
	xLastWakeTime = xTaskGetTickCount();
	
	for(;;)
	{
		
		if (xSemaphoreTake(xTask1Semaphore, portMAX_DELAY)){
			printf("CO2_sensor started measurment\n");
			
			// callback function gives semaphore
			if(MHZ19_OK != mh_z19_takeMeassuring()){
				printf("SOmething WIthTHE CO2 Went WONG!\n");
			}
			
		}
		
		xTaskDelayUntil(&xLastWakeTime, xFrequency);
	}
}

/*-----------------------------------------------------------*/
void task2( void *pvParameters )
{

	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 3000/portTICK_PERIOD_MS;

	// Initialise the xLastWakeTime variable with the current time.
	xLastWakeTime = xTaskGetTickCount();
	
	for(;;)
	{
		
		if (xSemaphoreTake(xTask2Semaphore, portMAX_DELAY)){
			status_leds_fastBlink(led_ST3);

			printf("Measure tempratur and humidity started\n");
			
			if (HIH8120_OK != hih8120_wakeup()){
				printf("temp/hum did not wake up\n");
			}
			
			xTaskDelayUntil(&xLastWakeTime,100);
			
			if (HIH8120_OK != hih8120_measure()){
				printf("the measurement went wrong\n");
			}
			
			xTaskDelayUntil(&xLastWakeTime,100);
			
			if(hih8120_isReady()){
				status_leds_ledOff(led_ST3);
				xSemaphoreGive(xTask2Semaphore);
			}
		}
		
		xTaskDelayUntil(&xLastWakeTime, xFrequency);
	}
}

void taskHandler( void *pvParameters) {
	
	xSemaphoreTake(xTask1Semaphore, portMAX_DELAY);
	xSemaphoreTake(xTask2Semaphore, portMAX_DELAY);
	
	int16_t tempratur;
	uint16_t humidity;
	uint16_t co2_ppm;
	
	measurment_t measurment;
	
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 30000/portTICK_PERIOD_MS;

	// Initialise the xLastWakeTime variable with the current time.
	xLastWakeTime = xTaskGetTickCount();
	
	for (;;)
	{
		xTaskDelayUntil(&xLastWakeTime, xFrequency);
		
		printf("Task handler : give semaphores! \n");
		xSemaphoreGive(xTask1Semaphore);
		xSemaphoreGive(xTask2Semaphore);
		
		vTaskDelay(2000);
		
		if (xSemaphoreTake(xTask1Semaphore, portMAX_DELAY)){
			mh_z19_returnCode_t return_code = mh_z19_getCo2Ppm(&co2_ppm);
			
			printf("co2 : %d, %d \n ",co2_ppm, return_code);
			
		}
		
		if (xSemaphoreTake(xTask2Semaphore, portMAX_DELAY)){
			
			tempratur = hih8120_getTemperature_x10();
			printf("Temp: %d \n", tempratur);
			
			humidity = hih8120_getHumidityPercent_x10();
			printf("humidity: %d \n", humidity);
		}
		
		measurment.co2_ppm = co2_ppm;
		measurment.humidity = humidity;
		measurment.tempratur = tempratur;
		
		printf("measurment: temp: %d, hum: %d, co2: %d \n", measurment.tempratur,measurment.humidity,measurment.co2_ppm);
		
		send_measurment(&measurment);
		
		
	}
}

void myCo2CallBack(uint16_t ppm)
{
	xSemaphoreGive(xTask1Semaphore);
}
/*-----------------------------------------------------------*/
void initialiseSystem()
{
	// Set output ports for leds used in the example
	DDRA |= _BV(DDA0) | _BV(DDA7);

	// Make it possible to use stdio on COM port 0 (USB) on Arduino board - Setting 57600,8,N,1
	stdio_initialise(ser_USART0);
	// maybe sei() ? for global interrupts
	// Let's create some tasks
	create_tasks_and_semaphores();

	// vvvvvvvvvvvvvvvvv BELOW IS LoRaWAN initialisation vvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
	// Status Leds driver
	status_leds_initialise(5); // Priority 5 for internal task
	// Initialize the LoRaWAN driver without down-link buffer
	lora_driver_initialise(1, NULL);
	// Create LoRaWAN task and start it up with priority 3
	lora_handler_initialise(2);

	// initialize Display drivers
	display_7seg_initialise(NULL);
	
	// Power up the display
	display_7seg_powerUp();


	// Initialize temp/hum
	if ( HIH8120_OK != hih8120_initialise() )
	{
		
		printf("failed initializing temp/hum \n");
	}

	// co2
	mh_z19_initialise(ser_USART3);
	mh_z19_injectCallBack(myCo2CallBack);
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


