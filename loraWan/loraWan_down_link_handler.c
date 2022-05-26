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

// Static Variables
static TickType_t _xLastWakeTime;
static MessageBufferHandle_t _downLinkMessageBuffer;
static lora_driver_payload_t payload;


void loraWan_down_link_handler_initialize(UBaseType_t taskPriority, MessageBufferHandle_t downLinkBuffer){
    // Set Internal Message Buffer Variable to the initialised one
    _downLinkMessageBuffer = downLinkBuffer;
    
    // Create the DownLink Handler Task
    xTaskCreate(loraWan_down_link_handler_task_handler,
    "down_link_handler_task",
    configMINIMAL_STACK_SIZE,
    NULL,
    taskPriority,
    NULL);
}


void loraWan_down_link_handler_task_run(void){
    
    // May need a Delay here
    
    // Read the Data from the Message Buffer
    size_t received = xMessageBufferReceive(_downLinkMessageBuffer,
    payload,
    sizeof(payload),
    portMAX_DELAY);
    
    // Inform User the DownlinkBuffer has received Settings
    printf("DownlinkBuffer: Settings Received\n")
    
    // Ensure Byte count is correct
    if (received != 5) {
        printf("Incorrect size of Settings\n\tNeeded: %d | Got: %d", 5, received)
        return;
    }
    
    // Reading all raw data bytes
    int16_t temperature_target = payload.bytes[0];                       // First Byte
    int16_t temperature_margin = payload.bytes[1];                       // Second Byte
    uint16_t humidity_threshold =  payload.bytes[2];                     // Third Byte
    uint16_t co2_threshold =  payload.bytes[3] << 8 | payload.bytes[4];  // Fourth and Fifth Byte
    
    // Adjust starting point
    temperature_target += 10;
    co2_threshold += 200;
    
    // Setting the configuration targets and thresholds
    configMutex_setTargetTemperature(temperature_target);
    configMutex_setTemperatureMargin(temperature_margin);
    configMutex_setHumidityThreshold(humidity_threshold);
    configMutex_setCO2Threshold(co2_threshold);
}


void loraWan_down_link_handler_task_handler(void *pvParameters){

    for (;;)
    {
        loraWan_down_link_handler_task_run();
    }
    
}
