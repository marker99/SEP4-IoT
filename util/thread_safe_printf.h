/*
 * thread_safe_printf.h
 *
 * Created: 30-05-2022 11:39:16
 *  Author: Sander
 */ 


#ifndef THREAD_SAFE_PRINTF_H_
#define THREAD_SAFE_PRINTF_H_

void thread_safe_printf_initialize();

void thread_safe_printf(const char *fmt, ...);

#endif /* THREAD_SAFE_PRINTF_H_ */