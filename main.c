/*
* main.c
* Author : IHA
*
* Example main file including LoRaWAN setup
* Just for inspiration :)
*/

#include <stdio.h>
#include <avr/io.h>

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

// define two Tasks
void task1( void *pvParameters );
void task2( void *pvParameters );

// define semaphore handle
SemaphoreHandle_t xTestSemaphore;

// Prototype for LoRaWAN handler
void lora_handler_initialise(UBaseType_t lora_handler_task_priority);

/*-----------------------------------------------------------*/
void create_tasks_and_semaphores(void)
{
	// Semaphores are useful to stop a Task proceeding, where it should be paused to wait,
	// because it is sharing a resource, such as the Serial port.
	// Semaphores should only be used whilst the scheduler is running, but we can set it up here.
	if ( xTestSemaphore == NULL )  // Check to confirm that the Semaphore has not already been created.
	{
		xTestSemaphore = xSemaphoreCreateMutex();  // Create a mutex semaphore.
		if ( ( xTestSemaphore ) != NULL )
		{
			xSemaphoreGive( ( xTestSemaphore ) );  // Make the mutex available for use, by initially "Giving" the Semaphore.
		}
	}

	xTaskCreate(
	task1
	,  "Task1"  // A name just for humans
	,  configMINIMAL_STACK_SIZE  // This stack size can be checked & adjusted by reading the Stack Highwater
	,  NULL
	,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
	,  NULL );

	xTaskCreate(
	task2
	,  "Task2"  // A name just for humans
	,  configMINIMAL_STACK_SIZE  // This stack size can be checked & adjusted by reading the Stack Highwater
	,  NULL
	,  1  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
	,  NULL );
}

/*-----------------------------------------------------------*/
void task1( void *pvParameters )
{
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 500/portTICK_PERIOD_MS; // 500 ms

	// Initialise the xLastWakeTime variable with the current time.
	xLastWakeTime = xTaskGetTickCount();
	
	for(;;)
	{
		xTaskDelayUntil( &xLastWakeTime, xFrequency );
		puts("Task1"); // stdio functions are not reentrant - Should normally be protected by MUTEX
		PORTA ^= _BV(PA0);
		

		if(MHZ19_OK != mh_z19_takeMeassuring()){
			printf("SOmething WIthTHE CO2Went WONG!\n");
			
		}
			
	}
}

/*-----------------------------------------------------------*/
void task2( void *pvParameters )
{
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 1000/portTICK_PERIOD_MS; // 1000 ms

	// Initialise the xLastWakeTime variable with the current time.
	xLastWakeTime = xTaskGetTickCount();
	status_leds_fastBlink(led_ST3);
	if (HIH8120_OK != hih8120_wakeup()){
		printf("temp/hum did not wake up\n");
	}
	xTaskDelayUntil(&xLastWakeTime,100);
	if (HIH8120_OK != hih8120_measure()){
		printf("the measurement went wrong\n");
	}

	display_7seg_displayHex("JOHN"); // why no work ?

	for(;;)
	{
		xTaskDelayUntil( &xLastWakeTime, xFrequency );
		puts("Task2 hallo!"); // stdio functions are not reentrant - Should normally be protected by MUTEX
		PORTA ^= _BV(PA7);
		
		if(hih8120_isReady()){
			printf("The measurement was ready!\n");
			uint16_t temperature = hih8120_getTemperature_x10();
			printf("Temp: %d \n", temperature);
			uint16_t humidity = hih8120_getHumidityPercent_x10();
			printf("humidity: %d \n", humidity);
			
			if (HIH8120_OK != hih8120_wakeup()){
				printf("temp/hum did not wake up\n");
			}
			xTaskDelayUntil(&xLastWakeTime,100);
			if (HIH8120_OK != hih8120_measure()){
				printf("the measurement went wrong\n");
			}
		}
	}
}

void myCo2CallBack(uint16_t ppm)
{
	printf("call back CO2: %d\n", ppm);
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
	lora_handler_initialise(3);

	// initialize Display drivers
	display_7seg_initialise(NULL);
	
	// Power up the display
	display_7seg_powerUp();


	// Initialize temp/hum
	if ( HIH8120_OK != hih8120_initialise() )
	{
	
		printf("failed initializing temp/hum");
	}

	// co2
	mh_z19_initialise(ser_USART3);
	mh_z19_injectCallBack(myCo2CallBack);
}


/*-----------------------------------------------------------*/
int main(void)
{
	initialiseSystem(); // Must be done as the very first thing
	printf("Program Started john\n");
	vTaskStartScheduler(); // Initialize and run the freeRTOS scheduler. Execution should never return from here.
	
	
	/* Replace with your application code */
	while (1)
	{
		
		
	}
}


