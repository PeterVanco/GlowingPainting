#include "App.h"

PWM timer1(TIM1);
PWM timer2(TIM2);
PWM timer3(TIM3);
PWM PWMs[] = {timer1, timer2, timer3};

#define NO_OF_LIGHTS					sizeof(allLights) / sizeof(allLights[0])

#define FOR_ALL_LIGHTS(closure)			{																	\
											uint8_t lightIndex;												\
											for (lightIndex = 0; lightIndex < NO_OF_LIGHTS; lightIndex++) {	\
												Light *light = &allLights[lightIndex];						\
												closure;													\
											}																\
										}

typedef enum _LightID {
	BIG_1,
	BIG_2,
	BIG_3,

	MIDDLE_1,
	MIDDLE_2,

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
	// small_1.setMinimumValue(MIN_INTENSITY + 200);
}


void App::init() {

	configureRcc();
	configureGpio();
	configureUart();

	printf("\n\nSystem initialized\n");

	initPWMs();
	initButton();
	initADC();

}

void App::initPWMs() {
	uint8_t i;
	for (i = 0; i < sizeof(PWMs) / sizeof(PWMs[0]); i++) {
		PWMs[i].init();
	}
}

void App::initADC() {

	ADC_InitTypeDef  ADC_InitStructure;
	/* PCLK2 is the APB2 clock */
	/* ADCCLK = PCLK2/6 = 72/6 = 12MHz*/
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);

	/* Enable ADC1 clock so that we can talk to it */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	/* Put everything back to power-on defaults */
	ADC_DeInit(ADC1);

	/* ADC1 Configuration ------------------------------------------------------*/
	/* ADC1 and ADC2 operate independently */
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	/* Disable the scan conversion so we do one at a time */
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	/* Don't do contimuous conversions - do them on demand */
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	/* Start conversin by software, not an external trigger */
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	/* Conversions are 12 bit - put them in the lower 12 bits of the result */
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	/* Say how many channels would be used by the sequencer */
	ADC_InitStructure.ADC_NbrOfChannel = 1;

	/* Now do the setup */
	ADC_Init(ADC1, &ADC_InitStructure);
	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);

	/* Enable ADC1 reset calibaration register */
	ADC_ResetCalibration(ADC1);
	/* Check the end of ADC1 reset calibration register */
	while(ADC_GetResetCalibrationStatus(ADC1));
	/* Start ADC1 calibaration */
	ADC_StartCalibration(ADC1);
	/* Check the end of ADC1 calibration */
	while(ADC_GetCalibrationStatus(ADC1));
}

uint16_t App::readLightIntensity() {
  ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1, ADC_SampleTime_1Cycles5);
  // Start the conversion
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);
  // Wait until conversion completion
  while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
  // Get the conversion value
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

	// NVIC_EnableIRQ(EXTI0_IRQn);
}

void App::selfTest() {

	printf("Self testing ...\n");

	// printf("Found %d groups\n", sizeof(allLights) / sizeof(allLights[0]));

	FOR_ALL_LIGHTS({
		light->setValue(0x00);
	})

	FOR_ALL_LIGHTS({
		light->setValue(MAX_INTENSITY);
		sleepMs(500);
	})

//	uint8_t i;
//	for (i = 0; i < 1; i++) {
//		Light* group = *allLights[i];
//		// printf("Group %d has %d lights\n", groupIndex, sizeof(*group));
//		for (groupIndex = 0; groupIndex < sizeof(group) / sizeof(group[0]); groupIndex++) {
//			printf("Turning ON light %d-%d\n", i, groupIndex);
//			// allLights[i][groupIndex].setValue(0xff);
//			sleepMs(500);
//		}
//	}

	printf("All lights ON\n");

	FOR_ALL_LIGHTS({
		light->setValue(0x00);
		sleepMs(500);
	})

//	for (i = 0; i < sizeof(allLights) / sizeof(allLights[0]); i++) {
//		uint8_t groupIndex;
//		for (groupIndex = 0; groupIndex < sizeof(allLights[i]) / sizeof(allLights[i][0]); groupIndex++) {
//			printf("Turning OFF light %d-%d\n", i, groupIndex);
//			// allLights[i][groupIndex].setValue(0x00);
//			sleepMs(500);
//		}
//	}
	printf("All lights OFF\n");

}

void App::step() {

	FOR_ALL_LIGHTS({
		light->setValue(light->step());
	})

//	uint8_t i;
//	for (i = 0; i < sizeof(allLights) / sizeof(allLights[0]); i++) {
//		uint8_t groupIndex;
//		for (groupIndex = 0; groupIndex < sizeof(allLights[i]) / sizeof(allLights[i][0]); groupIndex++) {
//			// allLights[i][groupIndex].setValue(allLights[i][groupIndex].step());
//		}
//	}
}

void App::configureGpio() {

	GPIO_InitTypeDef GPIO_InitStructure;

	// BUTTON
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// TIM

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE);
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);

	// UART2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

}

void App::configureRcc() {
	RCC_ClocksTypeDef RCC_Clocks;
	RCC_GetClocksFreq(&RCC_Clocks);
	SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 | RCC_APB1Periph_TIM2 | RCC_APB1Periph_USART2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO | RCC_APB2Periph_TIM1, ENABLE);
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
	NVIC_EnableIRQ(USART2_IRQn);
}

void App::sleep() {

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

	PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);

	init();

}
