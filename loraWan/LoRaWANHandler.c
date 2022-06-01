/*
* loraWANHandler.c
*
* Created: 12/04/2019 10:09:05
*  Author: IHA
*/

// FreeRTOS Includes
#include <ATMEGA_FreeRTOS.h>
#include <message_buffer.h>
#include <lora_driver.h>
#include <status_leds.h>

// Includes
#include "LoRaWANHandler.h"
#include "measurment.h"
#include "settings.h"
#include "util/headers/thread_safe_printf.h"
#include <stddef.h>
#include <stdio.h>


// Parameters for OTAA join - Received in a mail from IHA
#define LORA_APP_EUI "E3F46724321C3AFF"
#define LORA_APP_KEY "70392C359F1F97A044173202961DB664"

static MessageBufferHandle_t lorawan_handler_downlink_message_buffer;

// Declaration of private functions
static void _lora_setup(void);

void lora_handler_initialize(UBaseType_t lora_handler_task_priority)
{
    // Initialize Message Buffers
  
    lorawan_handler_downlink_message_buffer = xMessageBufferCreate((3 * sizeof(lora_driver_payload_t)));
    
    // Initialize Driver, UpLink Handler and downlink handler
    lora_driver_initialise(lora_handler_task_priority, lorawan_handler_downlink_message_buffer);
	
    loraWan_up_link_handler_initialize(lora_handler_task_priority);
	loraWan_down_link_handler_initialize(lora_handler_task_priority, lorawan_handler_downlink_message_buffer);

    xTaskCreate(
    lora_handler_task
    ,  "LRHand"
    ,  configMINIMAL_STACK_SIZE+30
    ,  NULL
    ,  lora_handler_task_priority
    ,  NULL );
}

/*-----------------------------------------------------------*/
void lora_handler_task( void *pvParameters )
{
    // Hardware reset of LoRaWAN transceiver
    lora_driver_resetRn2483(1);
    
    // Small Delay
    vTaskDelay(2);
    
    // Release the Reset
    lora_driver_resetRn2483(0);
    
    // Give it a chance to wakeup
    vTaskDelay(150);
    
    // Clears the LoRaWAN Driver Buffers
    lora_driver_flushBuffers();

    // Sets up the LoRaWAN
    _lora_setup();

    // Yield to all other Tasks
	

    for(;;)
    {
        taskYIELD();
    }
}

static void _lora_setup(void)
{
    char _out_buf[20];
    lora_driver_returnCode_t rc;
    
    status_leds_slowBlink(led_ST2); // OPTIONAL: Led the green led blink slowly while we are setting up LoRa
    
    // Factory reset the transceiver
    thread_safe_printf("FactoryReset >%s<\n", lora_driver_mapReturnCodeToText(lora_driver_rn2483FactoryReset()));
    
	lora_driver_configureToEu868();

    // Get the transceivers HW EUI
    rc = lora_driver_getRn2483Hweui(_out_buf);
	
    lora_driver_mapReturnCodeToText(rc);

    lora_driver_setDeviceIdentifier(_out_buf);

    lora_driver_setOtaaIdentity(LORA_APP_EUI,LORA_APP_KEY,_out_buf);

	lora_driver_saveMac();

    lora_driver_setAdaptiveDataRate(LORA_ON);

	lora_driver_setReceiveDelay(500);

    // Join the LoRaWAN
    uint8_t maxJoinTriesLeft = 10;
    
    do {
        rc = lora_driver_join(LORA_OTAA);
        thread_safe_printf("Join Network Tries Left:%d >%s<\n", maxJoinTriesLeft, lora_driver_mapReturnCodeToText(rc));

        if ( rc != LORA_ACCEPTED)
        {
            // Wait 5 sec and lets try again
            vTaskDelay(pdMS_TO_TICKS(5000UL));
        }
        else
        {
            break;
        }
    } while (--maxJoinTriesLeft );

    if (rc == LORA_ACCEPTED)
    {
        // Connected to LoRaWAN :-)
        // Make the green led steady
        status_leds_ledOn(led_ST2); // OPTIONAL
    }
}



