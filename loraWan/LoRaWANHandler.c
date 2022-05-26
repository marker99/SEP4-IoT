/*
* loraWANHandler.c
*
* Created: 12/04/2019 10:09:05
*  Author: IHA
*/
#include <stddef.h>
#include <stdio.h>

#include <ATMEGA_FreeRTOS.h>
#include <message_buffer.h>
#include <lora_driver.h>
#include <status_leds.h>

#include "LoRaWANHandler.h"
#include "measurment.h"
#include "settings.h"


// Parameters for OTAA join - You have got these in a mail from IHA
#define LORA_appEUI "E3F46724321C3AFF"
#define LORA_appKEY "70392C359F1F97A044173202961DB664"

//private methodes
static void _lora_setup(void);

void lora_handler_initialize(UBaseType_t lora_handler_task_priority)
{
	
	lorawan_handler_uplink_massage_Buffer = xMessageBufferCreate(sizeof(measurment_t));
	lorawan_handler_downlink_message_buffer = xMessageBufferCreate(sizeof(settings_t));
	
	lora_driver_initialise(lora_handler_task_priority, lorawan_handler_downlink_message_buffer);
	loraWan_uplink_handler_initialize(lora_handler_task_priority, lorawan_handler_uplink_massage_Buffer);


	xTaskCreate(
	lora_handler_task
	,  "LRHand"
	,  configMINIMAL_STACK_SIZE
	,  NULL
	,  lora_handler_task_priority  
	,  NULL );
}

/*-----------------------------------------------------------*/
void lora_handler_task( void *pvParameters )
{
	// Hardware reset of LoRaWAN transceiver
	lora_driver_resetRn2483(1);
	vTaskDelay(2);
	lora_driver_resetRn2483(0);
	// Give it a chance to wakeup
	vTaskDelay(150);
	
	lora_driver_flushBuffers(); // get rid of first version string from module after reset!

	_lora_setup();
	
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
	printf("FactoryReset >%s<\n", lora_driver_mapReturnCodeToText(lora_driver_rn2483FactoryReset()));
	
	// Configure to EU868 LoRaWAN standards
	printf("Configure to EU868 >%s<\n", lora_driver_mapReturnCodeToText(lora_driver_configureToEu868()));

	// Get the transceivers HW EUI
	rc = lora_driver_getRn2483Hweui(_out_buf);
	printf("Get HWEUI >%s<: %s\n",lora_driver_mapReturnCodeToText(rc), _out_buf);

	// Set the HWEUI as DevEUI in the LoRaWAN software stack in the transceiver
	printf("Set DevEUI: %s >%s<\n", _out_buf, lora_driver_mapReturnCodeToText(lora_driver_setDeviceIdentifier(_out_buf)));

	// Set Over The Air Activation parameters to be ready to join the LoRaWAN
	printf("Set OTAA Identity appEUI:%s appKEY:%s devEUI:%s >%s<\n", LORA_appEUI, LORA_appKEY, _out_buf, lora_driver_mapReturnCodeToText(lora_driver_setOtaaIdentity(LORA_appEUI,LORA_appKEY,_out_buf)));

	// Save all the MAC settings in the transceiver
	printf("Save mac >%s<\n",lora_driver_mapReturnCodeToText(lora_driver_saveMac()));

	// Enable Adaptive Data Rate
	printf("Set Adaptive Data Rate: ON >%s<\n", lora_driver_mapReturnCodeToText(lora_driver_setAdaptiveDataRate(LORA_ON)));

	// Set receiver window1 delay to 500 ms - this is needed if down-link messages will be used
	printf("Set Receiver Delay: %d ms >%s<\n", 500, lora_driver_mapReturnCodeToText(lora_driver_setReceiveDelay(500)));

	// Join the LoRaWAN
	uint8_t maxJoinTriesLeft = 10;
	
	do {
		rc = lora_driver_join(LORA_OTAA);
		printf("Join Network Tries Left:%d >%s<\n", maxJoinTriesLeft, lora_driver_mapReturnCodeToText(rc));

		if ( rc != LORA_ACCEPTED)
		{
			// Make the red led pulse to tell something went wrong
			//status_leds_longPuls(led_ST2); // OPTIONAL
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
		printf("Lora wan accepted!\n");
	}
}



