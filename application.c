/*
* Application.c
*
* Created: 10-05-2022 10:54:35
* Author: Sander
*/

#include "application.h"
#include "system_config/event_group_config.h"

#include "sensors/headers/temp_hum_sensor.h"
#include "sensors/headers/co2_sensor.h"
#include "loraWan/headers/loraWan_up_link_handler.h"
#include "datastructures/measurment.h"
#include "LoRaWANHandler.h"
#include <stdlib.h>

static EventGroupHandle_t _dataReadyEventGroup;
static EventGroupHandle_t _measureEventGroup;

static TickType_t _xLastWakeTime;
static const TickType_t xFrequency = pdMS_TO_TICKS(30000);


// Declaration of private(static) functions
static void start_measuring();
static void wait_for_temp_hum_measurment();
static void wait_for_co2_measurment();


void application_initialize(UBaseType_t task_priority, EventGroupHandle_t readyGroup, EventGroupHandle_t startGroup){
    // Set the internal Ready and Start groups
    _dataReadyEventGroup = readyGroup;
    _measureEventGroup = startGroup;
    
    // Create the Tasks which runs the entire Application
    xTaskCreate(
    application_task_handler,
    "Application",
    configMINIMAL_STACK_SIZE,
    NULL,
    task_priority,
    NULL
    );
}


void application_task_handler(void *pvParameters){
    // Initialize Parameters
    application_task_init();
    
    // Endless Loop to Run Application
    for (;;){
        // Run Action split into a separate Function, helps with Testing
        application_task_run();
    }
}

void application_task_init(){
    printf("The Application task has started...\n");
    _xLastWakeTime = xTaskGetTickCount();
}


void application_task_run(){
    // Waiting Timer on Awake
    xTaskDelayUntil(&_xLastWakeTime, xFrequency);
    printf("\n> Starting a measurment\n");
    
    // Start the Measurement Sensors
    start_measuring();
    printf("\t> Waiting for sensors to set ready event group\n");
    
    // Wait for CO2 Sensor to become Ready
    wait_for_co2_measurment();
    printf("> Application: Co2 measurment is ready\n");
    
    // Wait for Temperature and Humidity Sensor to become Ready
    wait_for_temp_hum_measurment();
    printf("> Application: TempHum measurement is ready\n");
    
    // Initialize a New Measurement
    pMeasurment_t newMeasurment = pvPortMalloc(sizeof(measurment_t));

    // Ensuring the Measurement is created before attempting to fill it with Data
    if (newMeasurment != NULL){
        // Saving Temperature Measurement
        newMeasurment->temperature = temp_hum_getTemperature();
        // Saving Humidity Measurement
        newMeasurment->humidity = temp_hum_getHumidity();
        // Saving CO2 Measurement
        newMeasurment->co2PartsPrMillion = co2_sensor_getCO2();
        
        // Print for Debugging Purposes
        printf("> Measurement <\n\tTemp: %d\n\tHum: %d\n\tco2: %d\n> Measurement <\n", newMeasurment->temperature, newMeasurment->humidity, newMeasurment->co2PartsPrMillion);
    }
    
    // Short Delay (Lets other tasks run in the meantime
    vTaskDelay(100);
    
	uint8_t sizeToWrite = sizeof(measurment_t);
    // Copy the Measurement into the UpLink Message Buffer
    uint8_t writtenBytes = xMessageBufferSend(lorawan_handler_uplink_message_Buffer, (void *)newMeasurment, sizeToWrite, portMAX_DELAY);
	
    printf("\nwrite to up link buffer bytes: %d, size to write %d", writtenBytes, sizeToWrite);

	vTaskDelay(100);
    
    // Free the New Measurement, so as to not cause Memory Leakage
    vPortFree(newMeasurment);
}


static void start_measuring(){
    xEventGroupSetBits(
    _measureEventGroup,
    BIT_START_MEASURE
    );
}

static void wait_for_temp_hum_measurment(){
    xEventGroupWaitBits(
    _dataReadyEventGroup,
    BIT_TEMP_HUM_READY_MEASURE,
    pdTRUE,
    pdTRUE,
    portMAX_DELAY
    );
}

static void wait_for_co2_measurment(){
    xEventGroupWaitBits(
    _dataReadyEventGroup,
    BIT_CO2_READY_MEASURE,
    pdTRUE,
    pdTRUE,
    portMAX_DELAY
    );
}

