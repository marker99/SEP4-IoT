/*
* loraWan_down_link_handler.h
*
* Created: 26-05-2022 13:20:05
*  Author: Frederik
*/


#ifndef LORAWAN_DOWN_LINK_HANDLER_H_
#define LORAWAN_DOWN_LINK_HANDLER_H_

void loraWan_down_link_handler_initialize(UBaseType_t task_priority);

void loraWan_down_link_handler_task_run(void);

void loraWan_down_link_handler_task_handler(void *pvParameters);

#endif /* LORAWAN_DOWN_LINK_HANDLER_H_ */