#include "App.h"

PWM timer1(TIM1);
PWM timer3(TIM3);
PWM PWMs[] = {timer1, timer3};

#define NO_OF_LIGHTS					sizeof(allLights) / sizeof(allLights[0])

#define FOR_ALL_LIGHTS(closure)			{																	\
											uint8_t lightIndex;												\
											for (lightIndex = 0; lightIndex < NO_OF_LIGHTS; lightIndex++) {	\
												Light *light = &allLights[lightIndex];						\
												closure;													\
											}																\
										}

typedef enum {
	BIG_1,
	BIG_2,
	MIDDLE_1,
	SMALL_1
} LightID;



Light big_1(&timer1, CHANNEL_2, BIG_1);
Light big_2(&timer1, CHANNEL_1, BIG_2);
Light big[] = {big_1, big_2};

Light middle_1(&timer3, CHANNEL_1, MIDDLE_1);
Light middle[] = {middle_1};

Light small_1(&timer3, CHANNEL_2, SMALL_1);
Light small[] = {small_1};

Light allLights[] = {big_1, big_2, middle_1, small_1};

App::App() {
	systemState = NOT_INITIALIZED;
	// small_1.setMinimumValue(MIN_INTENSITY + 200);
}


void App::init(const bool initPWM) {

	configureRcc(initPWM);
	configureGpio(initPWM);
	configureUart();

	printf("\n\nSystem initialized\n");

	initButton();
	initADCWatchdog();
	initRTC();

	if (initPWM) {
		initPWMs();
	}

	systemState = initPWM ? READY : READY_NO_PWM;

}

SystemState App::getSystemState() {
	return systemState;
}

void App::initPWMs() {
	uint8_t i;
	for (i = 0; i < sizeof(PWMs) / sizeof(PWMs[0]); i++) {
		PWMs[i].init();
	}
}

void App::initRTC() {

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_ClearITPendingBit(EXTI_Line17);
	EXTI_InitStructure.EXTI_Line = EXTI_Line17;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	PWR_BackupAccessCmd(ENABLE);
	BKP_DeInit();

	RCC_LSICmd(ENABLE);

	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
	while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);

	RCC_RTCCLKCmd(ENABLE);

	RTC_WaitForSynchro();

	RTC_SetPrescaler(32767);
	RTC_WaitForLastTask();

	RTC_ITConfig(RTC_IT_ALR, ENABLE);
	RTC_WaitForLastTask();

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel = RTCAlarm_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}

void App::initADCWatchdog() {

	ADC_InitTypeDef  ADC_InitStructure;

	RCC_ADCCLKConfig(RCC_PCLK2_Div8);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	ADC_DeInit(ADC1);

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC1, &ADC_InitStructure);

	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1, ADC_SampleTime_13Cycles5);

	ADC_AnalogWatchdogThresholdsConfig(ADC1, LIGHT_INTENSITY_HIGH_TRESHOLD, LIGHT_INTENSITY_LOW_TRESHOLD);
	ADC_AnalogWatchdogSingleChannelConfig(ADC1, ADC_Channel_5);
	ADC_AnalogWatchdogCmd(ADC1, ADC_AnalogWatchdog_SingleRegEnable);

	ADC_ITConfig(ADC1, ADC_IT_AWD, ENABLE);

	ADC_Cmd(ADC1, ENABLE);

	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1));

	ADC_SoftwareStartConvCmd(ADC1, ENABLE);

	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannel = ADC1_2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}

uint16_t App::readLightIntensity() {
	//	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1, ADC_SampleTime_13Cycles5);
	//	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	//	while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
	return ADC_GetConversionValue(ADC1);
}

void App::initButton() {

	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);

    EXTI_InitStructure.EXTI_Line = EXTI_Line0;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

}

void App::selfTest() {

	printf("Testing ... ");

	FOR_ALL_LIGHTS({
		light->setValue(0x00);
	})

	FOR_ALL_LIGHTS({
		light->setValue(MAX_INTENSITY);
		sleepMs(500);
	})

	FOR_ALL_LIGHTS({
		light->setValue(0x00);
		sleepMs(500);
	})

	printf("OK\n");

}

void App::start() {

	big_1.setValue(big_1.step());
	big_2.setValue(big_2.step());
	sleepMs(500);
	middle_1.setValue(middle_1.step());
	sleepMs(500);
	small_1.setValue(small_1.step());
}

void App::step() {

	float lightFactor = (float) readLightIntensity() / (float) LIGHT_INTENSITY_LOW_TRESHOLD;

	FOR_ALL_LIGHTS({
		light->setMultiplicationFactor(lightFactor);
		light->setValue(light->step());
	})
}

void App::stop() {

	small_1.setValue(0);
	sleepMs(500);
	middle_1.setValue(0);
	sleepMs(500);
	big_1.setValue(0);
	big_2.setValue(0);
}

void App::configureGpio(const bool initPWM) {

	GPIO_InitTypeDef GPIO_InitStructure;

	// BUTTON
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// TIM
	if (initPWM) {
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
		GPIO_Init(GPIOB, &GPIO_InitStructure);

		GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
		GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE);
		GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);
	}

	// UART2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// BOARD LED
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

}

void App::configureRcc(const bool initPWM) {
	RCC_ClocksTypeDef RCC_Clocks;
	RCC_GetClocksFreq(&RCC_Clocks);
	SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);

	if (initPWM) {
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 | RCC_APB1Periph_TIM2, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	}

}

void App::configureUart() {

	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART2, &USART_InitStructure);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART2, ENABLE);

	while (USART_GetFlagStatus(USART2, USART_FLAG_TC ) == RESET);

	//NVIC_InitTypeDef NVIC_InitStructure;
	//NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	//NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
	//NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
	//NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	//NVIC_Init(&NVIC_InitStructure);
}

void App::sleep() {

	// printf("Setting RTC alarm\n");

	RTC_SetAlarm(RTC_GetCounter() + LIGHT_INTENSITY_CHECK_PERIOD);
	RTC_WaitForLastTask();

	//printf("Going to sleep ...");

	systemState = NOT_INITIALIZED;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	initButton();

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE, DISABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

	PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFE);

//	if (!isInitialized()) {
//		init();
//		// initADCWatchdog();
//	}

	// initADCWatchdog();

	printf("Woke up\n");

}
