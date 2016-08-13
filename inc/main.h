#ifndef MAIN_H_
#define MAIN_H_

#include <stddef.h>
#include <stdlib.h>
#include "stdio.h"
#include "stm32f10x.h"
#include "App.h"

#ifdef __cplusplus
extern "C" {
#endif



void SysTick_Handler(void);
void USART2_IRQHandler(void);

void uartPutc(uint8_t c);
void sleepMs(uint32_t ms);

#ifdef __cplusplus
}
#endif

#endif
