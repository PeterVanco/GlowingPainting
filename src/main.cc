
#include "main.h"

typedef enum {
	LIGHT_INTENSITY_NORMAL_OR_UNKNOWN,
	LIGHT_INTENSITY_LOW,
	LIGHT_INTENSITY_HIGH
} LightIntensity;

typedef enum {
	LIGHT_MEASUREMENT_NOT_SUPPRESSED,
	LIGHT_MEASUREMENT_SUPPRESSED_FROM_LOW,
	LIGHT_MEASUREMENT_SUPPRESSED_FROM_HIGH
} LightMeasurementSuppression;

__IO uint32_t sysTickActiveDelay;
__IO int32_t buttonDebouncer = -1;
__IO LightIntensity lightIntensity = LIGHT_INTENSITY_NORMAL_OR_UNKNOWN;
__IO bool adcMeasurementSuppressed;
__IO LightMeasurementSuppression lightIntensityMeasurementSuppression = LIGHT_MEASUREMENT_NOT_SUPPRESSED;

#define	SUPPRESS_ADC_MEASUREMENT(x) {										\
										adcMeasurementSuppressed = true;	\
										x									\
										adcMeasurementSuppressed = false;	\
									}

App app;

bool isLightIntensityLowAndNotSuppressedFromLow() {
	return lightIntensity == LIGHT_INTENSITY_LOW && lightIntensityMeasurementSuppression != LIGHT_MEASUREMENT_SUPPRESSED_FROM_LOW;
}

bool isLightIntensityHighOrSuppressedFromLow() {
	return lightIntensity == LIGHT_INTENSITY_HIGH || lightIntensityMeasurementSuppression == LIGHT_MEASUREMENT_SUPPRESSED_FROM_LOW;
}


int main(void) {

	app.init(false);
	// app.init(true);
	// app.selfTest();

	while (1) {

		// (it was not DISABLED at NIGHT && is NIGHT) || it was ENABLED during DAY
		if (isLightIntensityLowAndNotSuppressedFromLow() || lightIntensityMeasurementSuppression == LIGHT_MEASUREMENT_SUPPRESSED_FROM_HIGH) {
			if (app.getSystemState() != READY) {
				app.init(true);
				app.start();
			}
			app.step();
			sleepMs(10);
		} else if (buttonDebouncer == BUTTON_IDLE && isLightIntensityHighOrSuppressedFromLow()) {
			SUPPRESS_ADC_MEASUREMENT({
				app.stop();
				app.sleep();
				lightIntensity = LIGHT_INTENSITY_NORMAL_OR_UNKNOWN;
			})
		}

	}
}

void uartPutc(uint8_t c) {

	USART_SendData(USART2, c);
	while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
}

void USART2_IRQHandler() {

	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
		uint16_t c = USART_ReceiveData(USART2);
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);

		USART_SendData(USART2, c);
		while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
	}
}

void initAppAfterWakeUp() {
	if (app.getSystemState() == NOT_INITIALIZED) {
		app.init(false);
	}
}

void EXTI0_IRQHandler() {

	if (EXTI_GetITStatus(EXTI_Line0) != RESET) {
		EXTI_ClearITPendingBit(EXTI_Line0);

		initAppAfterWakeUp();

		buttonDebouncer = BUTTON_DEBOUNCE_TIME_MS;
	}
}

void disableMeasurementSuppression(LightMeasurementSuppression suppressionType) {
	if (lightIntensityMeasurementSuppression == suppressionType) {
		lightIntensityMeasurementSuppression = LIGHT_MEASUREMENT_NOT_SUPPRESSED;
	}
}

void ADC1_IRQHandler() {

	ADC_ClearITPendingBit(ADC1, ADC_IT_AWD);

	if (adcMeasurementSuppressed) {
		return;
	}

	uint16_t value = app.readLightIntensity();
	if (lightIntensity != LIGHT_INTENSITY_HIGH && value > LIGHT_INTENSITY_HIGH_TRESHOLD) {
		// printf("ABOVE (%i)\n", value);
		lightIntensity = LIGHT_INTENSITY_HIGH;
		disableMeasurementSuppression(LIGHT_MEASUREMENT_SUPPRESSED_FROM_LOW);

	} else if (lightIntensity != LIGHT_INTENSITY_LOW && value < LIGHT_INTENSITY_LOW_TRESHOLD) {
		// printf("BELOW (%i)\n", value);
		lightIntensity = LIGHT_INTENSITY_LOW;
		disableMeasurementSuppression(LIGHT_MEASUREMENT_SUPPRESSED_FROM_HIGH);
	}

}

void RTCAlarm_IRQHandler() {

	if (RTC_GetITStatus(RTC_IT_ALR) != RESET) {

		initAppAfterWakeUp();

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

void handleButton() {

	if (lightIntensityMeasurementSuppression == LIGHT_MEASUREMENT_NOT_SUPPRESSED) {
		lightIntensityMeasurementSuppression = lightIntensity == LIGHT_INTENSITY_LOW ?
																 LIGHT_MEASUREMENT_SUPPRESSED_FROM_LOW :
																 LIGHT_MEASUREMENT_SUPPRESSED_FROM_HIGH;
	} else {
		lightIntensityMeasurementSuppression = LIGHT_MEASUREMENT_NOT_SUPPRESSED;
	}
}

void handleButtonDebouncer() {
	if (buttonDebouncer > 0) {
		buttonDebouncer--;
	} else if (buttonDebouncer == 0) {
		buttonDebouncer = BUTTON_IDLE;
		handleButton();
	}
}

void SysTick_Handler(void) {

	if (sysTickActiveDelay) {
		sysTickActiveDelay--;
	}

	handleButtonDebouncer();
}
