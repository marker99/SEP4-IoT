/*
 * time_interval_config.h
 *
 * Created: 30-05-2022 17:25:19
 *  Author: Sander
 */ 


#ifndef TIME_INTERVAL_CONFIG_H_
#define TIME_INTERVAL_CONFIG_H_
#include <ATMEGA_FreeRTOS.h>

//Application

#define APPLICATION_MAX_DELAY  pdMS_TO_TICKS(30000) // application will wait for a measurement for maximum 30 sec
#define APPLICATION_MEASURMENT_FREQUENCY pdMS_TO_TICKS(60000) // every 1 min

//lora wan
#define LORA_WAN_UPLINK_FREQUENCY pdMS_TO_TICKS(300000) // every 5 min
#define LORA_WAN_UP_LINK_TASK_START_OFFSET pdMS_TO_TICKS(10000) // 10 sec




#endif /* TIME_INTERVAL_CONFIG_H_ */