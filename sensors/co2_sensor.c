/*
* co2_sensor.c
*
* Created: 12-05-2022 07:19:51
*  Author: Sander
*/

#define DEVICE = "MHZ19 [CO2]"

// Includes
#include "co2_sensor.h"
#include "../application_config.h"
#include <status_leds.h>

// Static Variables
static EventGroupHandle_t _dataReadyEventGroup;
static EventGroupHandle_t _measureEventGroup;
static TickType_t _xLastWakeTime;

// Variables
uint16_t measuredCO2;

// Declaration of private functions
static void CO2_wakeupAndMeasure();


void co2_sensor_initialize(UBaseType_t task_priority, EventGroupHandle_t readyGroup, EventGroupHandle_t measureGroup)
{
    
    mh_z19_initialise(ser_USART3);
    
    mh_z19_injectCallBack(_co2_sensor_callback);
    
    _dataReadyEventGroup = readyGroup;
    _measureEventGroup = measureGroup;
    
    xTaskCreate(
    co2_sensor_taskHandler,
    "CO2_Sensor_Task",
    configMINIMAL_STACK_SIZE,
    NULL,
    task_priority,
    NULL);
    
    status_leds_ledOn(led_ST4);
}

void _co2_sensor_callback(uint16_t ppm)
{
    measuredCO2 = ppm;
    status_leds_ledOn(led_ST4);
    xEventGroupSetBits(_dataReadyEventGroup, BIT_CO2_READY_MEASURE);
}

void _co2_sensor_task_init(){
    printf("%s Sensor is Starting Up\n", DEVICE);
    _xLastWakeTime = xTaskGetTickCount();
}

void _co2_sensor_task_run(){
    xEventGroupWaitBits(_measureEventGroup,
    BIT_START_CO2_MEASURE,
    pdTRUE,
    pdTRUE,
    portMAX_DELAY);
    
    //Sensors start measuring
    CO2_wakeupAndMeasure();
    
    // Small Delay
    vTaskDelay(pdMS_TO_TICKS(50));
}

uint16_t co2_sensor_getCO2(){
    return measuredCO2;
}


void co2_sensor_taskHandler(void *pvParamerters)
{
    // Initialize Sensor Task
    _co2_sensor_task_init();
    
    // Run Sensor Task
    for(;;)
    {
        _co2_sensor_task_run();
    }
}



static void CO2_wakeupAndMeasure(){
    printf("%s Sensor has started measuring\n", DEVICE);
    status_leds_fastBlink(led_ST4);
    vTaskDelay(pdMS_TO_TICKS(50));
    
    
    mh_z19_returnCode_t returnCode;
    
    if(MHZ19_OK != (returnCode = mh_z19_takeMeassuring())){
        printf("%s Sensor failed to measure\n", DEVICE);
    }
    
    vTaskDelay(pdMS_TO_TICKS(50));
}




