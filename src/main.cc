
#include "main.h"

typedef enum {
	LIGHT_INTENSITY_NORMAL,
	LIGHT_INTENSITY_LOW,
	LIGHT_INTENSITY_HIGH
} LightIntensity;

__IO uint32_t sysTickActiveDelay;
__IO LightIntensity lightIntensity = LIGHT_INTENSITY_NORMAL;

App app;

int main(void) {

	app.init();

	app.selfTest();

//	RTC_ClearFlag(RTC_FLAG_SEC);
//	while(RTC_GetFlagStatus(RTC_FLAG_SEC) == RESET);

//	/* Alarm in 3 second */
//	RTC_SetAlarm(RTC_GetCounter()+ 3);
//	/* Wait until last write operation on RTC registers has finished */
//	RTC_WaitForLastTask();

	while (1) {

		// printf("Loop\n");

		// printf("%i\n", app.readLightIntensity());

		if (lightIntensity == LIGHT_INTENSITY_LOW) {
			app.step();
			sleepMs(10);
		} else if (lightIntensity == LIGHT_INTENSITY_HIGH) {
			app.sleep();
			lightIntensity = LIGHT_INTENSITY_NORMAL;
		}

	}

	// app.sleep();

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

void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line0);
		printf("INT\n");
	}
}

void ADC1_IRQHandler() {

	/* Clear ADC1 AWD pending interrupt bit */
	ADC_ClearITPendingBit(ADC1, ADC_IT_AWD);

	if (lightIntensity == LIGHT_INTENSITY_NORMAL) {
		printf("ADC watchdog\n");
	}

	uint16_t value = ADC_GetConversionValue(ADC1);
	if (lightIntensity != LIGHT_INTENSITY_HIGH && value > LIGHT_INTENSITY_HIGH_TRESHOLD) {
		printf("Light intensity is above upper treshold (%i)\n", value);
		lightIntensity = LIGHT_INTENSITY_HIGH;
	} else if (lightIntensity != LIGHT_INTENSITY_LOW && value < LIGHT_INTENSITY_LOW_TRESHOLD) {
		printf("Light intensity is below lower treshold (%i)\n", value);
		lightIntensity = LIGHT_INTENSITY_LOW;
	}

}

void RTCAlarm_IRQHandler(void) {

	if(RTC_GetITStatus(RTC_IT_ALR) != RESET) {

		printf("RTC\n");

		/* Clear EXTI line17 pending bit */
		EXTI_ClearITPendingBit(EXTI_Line17);

		/* Check if the Wake-Up flag is set */
		if(PWR_GetFlagStatus(PWR_FLAG_WU) != RESET) {
			/* Clear Wake Up flag */
			PWR_ClearFlag(PWR_FLAG_WU);
		}

		/* Wait until last write operation on RTC registers has finished */
		RTC_WaitForLastTask();
		/* Clear RTC Alarm interrupt pending bit */
		RTC_ClearITPendingBit(RTC_IT_ALR);
		/* Wait until last write operation on RTC registers has finished */
		RTC_WaitForLastTask();
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

