/*
* loraWan_down_link_handler.c
*
* Created: 26-05-2022 13:25:38
*  Author: Frederik
*/

// FreeRTOS Includes
#include <ATMEGA_FreeRTOS.h>
#include <message_buffer.h>
#include <lora_driver.h>
#include <stdio.h>

// Includes
#include "headers/loraWan_down_link_handler.h"
#include "configuration_settings.h"
#include "settings.h"
#include "util/thread_safe_printf.h"

// Static Variables
//static TickType_t _xLastWakeTime;
static MessageBufferHandle_t _downLinkMessageBuffer;
static pSettings_t _buffer_settings;


void loraWan_down_link_handler_initialize(UBaseType_t taskPriority, MessageBufferHandle_t downLinkBuffer){
    // Set Internal Message Buffer Variable to the initialised one
    _downLinkMessageBuffer = downLinkBuffer;
    
	_buffer_settings = pvPortMalloc(sizeof(settings_t));
	
    // Create the DownLink Handler Task
    xTaskCreate(loraWan_down_link_handler_task,
    "down_link_handler_task",
    configMINIMAL_STACK_SIZE,
    NULL,
    taskPriority,
    NULL);
}


void loraWan_down_link_handler_task_run(void){
    
    // May need a Delay here
    
	vTaskDelay(500);
    // Read the Data from the Message Buffer
    size_t received = xMessageBufferReceive(_downLinkMessageBuffer,
    _buffer_settings,
    sizeof(settings_t),
    portMAX_DELAY);
    
    // Inform User the DownlinkBuffer has received Settings
    thread_safe_printf("DownlinkBuffer: Settings Received\n");
    
    // Ensure Byte count is correct
    if (received != 5) {
        thread_safe_printf("Incorrect size of Settings\n\tNeeded: %d | Got: %d", 5, received);
        return;
    }
    /*
    // Reading all raw data bytes
    int16_t temperature_target = _buffer_settings.bytes[0];                       // First Byte
    int16_t temperature_margin = _buffer_settings.bytes[1];                       // Second Byte
    uint16_t humidity_threshold =  _buffer_settings.bytes[2];                     // Third Byte
    uint16_t co2_threshold =  _buffer_settings.bytes[3] << 8 | _buffer_settings.bytes[4];  // Fourth and Fifth Byte
    */
	
	int16_t temperature_target = _buffer_settings->temperatur_target;                     // First Byte
	int16_t temperature_margin = _buffer_settings->temperatur_margin;                       // Second Byte
	uint16_t humidity_threshold =  _buffer_settings->humidity;               // Third Byte
	uint16_t co2_threshold = _buffer_settings->co2_ppm;
	
    // Adjust starting point
    temperature_target += 10;
    co2_threshold += 200;
    
    // Setting the configuration targets and thresholds
    configMutex_setTargetTemperature(temperature_target);
    configMutex_setTemperatureMargin(temperature_margin);
    configMutex_setHumidityThreshold(humidity_threshold);
    configMutex_setCO2Threshold(co2_threshold);
}


void loraWan_down_link_handler_task(void *pvParameters){

    for (;;)
    {
        loraWan_down_link_handler_task_run();
    }
    
}
