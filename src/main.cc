
#include "main.h"

typedef enum {
	LIGHT_INTENSITY_NORMAL_OR_UNKNOWN,
	LIGHT_INTENSITY_LOW,
	LIGHT_INTENSITY_HIGH
} LightIntensity;

__IO uint32_t sysTickActiveDelay;
__IO LightIntensity lightIntensity = LIGHT_INTENSITY_NORMAL_OR_UNKNOWN;
__IO bool buttonPressed;

App app;

int main(void) {

	app.init();
	// app.selfTest();

	while (1) {

		if (buttonPressed) {
			if (app.isInitialized()) {
				app.sleep();
				lightIntensity = LIGHT_INTENSITY_NORMAL_OR_UNKNOWN;
			} else {
				// TODO suppress Light measurement
			}
			buttonPressed = false;
		}

		if (lightIntensity == LIGHT_INTENSITY_LOW) {
			if (!app.isInitialized()) {
				app.init();
			}
			app.step();
			sleepMs(10);
		} else if (lightIntensity == LIGHT_INTENSITY_HIGH) {
			app.sleep();
			lightIntensity = LIGHT_INTENSITY_NORMAL_OR_UNKNOWN;
		}

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

void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line0);
		printf("BUTTON\n");
		buttonPressed = true;
	}
}

void ADC1_IRQHandler() {

	ADC_ClearITPendingBit(ADC1, ADC_IT_AWD);

	if (lightIntensity == LIGHT_INTENSITY_NORMAL_OR_UNKNOWN) {
		printf("Light level unknown - measuring\n");
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

	if (RTC_GetITStatus(RTC_IT_ALR) != RESET) {

		printf("RTC Alarm\n");

		EXTI_ClearITPendingBit(EXTI_Line17);

		if(PWR_GetFlagStatus(PWR_FLAG_WU) != RESET) {
			PWR_ClearFlag(PWR_FLAG_WU);
		}

		RTC_WaitForLastTask();
		RTC_ClearITPendingBit(RTC_IT_ALR);
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
