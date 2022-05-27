/*
 * loraWan_up_link_handler.h
 *
 * Created: 23-05-2022 12:12:51
 *  Author: Sander
 */ 


#ifndef LORAWAN_UP_LINK_HANDLER_H_
#define LORAWAN_UP_LINK_HANDLER_H_
#include <ATMEGA_FreeRTOS.h>
#include <message_buffer.h>
#include "measurment.h"

void loraWan_up_link_handler_initialize(UBaseType_t task_priority, MessageBufferHandle_t uplink_message_buffer);

void loraWan_up_link_handler_task_init();

void loraWan_up_link_handler_task_run();

void loraWan_up_link_handler_task(void *pvParameters);

#endif /* LORAWAN_UP_LINK_HANDLER_H_ */