/*
* thread_safe_printf.c
*
* Created: 30-05-2022 11:48:58
*  Author: Sander
*/

#include <ATMEGA_FreeRTOS.h>
#include <semphr.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdio_driver.h>
#include <avr/interrupt.h>
#include <serial.h>
#include <task.h>
#include "headers/thread_safe_printf.h"

static SemaphoreHandle_t _semaphore;

void thread_safe_printf_initialize(){
	_semaphore = xSemaphoreCreateMutex();
	xSemaphoreGive(_semaphore);
	// Make it possible to use STDIO on COM port 0 (USB) on Arduino board - Setting 57600, 8, N, 1
	stdio_initialise(ser_USART0);
	sei();
}

// can only be used after the freeRTOS vTaskStartScheduler is called, use printf, from stdIo before.
void thread_safe_printf(const char *fmt, ...){
	if (xSemaphoreTake(_semaphore, portMAX_DELAY)){
		va_list args;
		va_start(args, fmt);
		vprintf(fmt, args);
		va_end(args);
		vTaskDelay(pdMS_TO_TICKS(50));
		xSemaphoreGive(_semaphore);
	}
	
}