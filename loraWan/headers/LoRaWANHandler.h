/*
 * LoRaWANHandler.h
 *
 * Created: 26-05-2022 13:54:03
 *  Author: sande
 */ 

#pragma once

#include "loraWan_uplink_handler.h"
#include <message_buffer.h>


MessageBufferHandle_t lorawan_handler_uplink_massage_Buffer;
MessageBufferHandle_t lorawan_handler_downlink_message_buffer;

void lora_handler_task( void *pvParameters );

void lora_handler_initialise(UBaseType_t lora_handler_task_priority);

void lora_handler_task( void *pvParameters );


