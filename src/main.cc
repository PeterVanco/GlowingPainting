
#include "main.h"

__IO uint32_t sysTickActiveDelay;

int main(void) {

	App app;

	app.init();
	// app.selfTest();

	while (1) {
		app.step();
		sleepMs(10);
	}

}

void uartPutc(uint8_t c) {
	USART_SendData(USART2, c);
	while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
}

void USART2_IRQHandler(void)
{
	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
		uint16_t c = USART_ReceiveData(USART2);
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);

		USART_SendData(USART2, c);
		while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
	}
}

void sleepMs(uint32_t ms) {
	sysTickActiveDelay = ms;
	while (sysTickActiveDelay);
}

void SysTick_Handler(void) {
	if (sysTickActiveDelay)
		sysTickActiveDelay--;
}

#ifdef USE_FULL_ASSERT

	void __assert_func(const char *file, int line, const char *func, const char *failedexpr) {
		while (1) {
		}
	}

	void __assert(const char *file, int line, const char *failedexpr) {
		__assert_func(file, line, NULL, failedexpr);
	}

	void assert_failed(uint8_t *file, uint32_t line) {
		printf("FATAL ERROR occured in file %s on line %lu\n", file, line);
		while (1) { }
	}

#endif

