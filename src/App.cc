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

Light big_1(&timer2, CHANNEL_2, BIG_1);
Light big_2(&timer3, CHANNEL_1, BIG_2);
Light big_3(&timer3, CHANNEL_2, BIG_3);
Light big[] = {big_1, big_2, big_3};

Light middle_1(&timer1, CHANNEL_1, MIDDLE_1);
Light middle_2(&timer1, CHANNEL_2, MIDDLE_1);
Light middle[] = {middle_1, middle_2};

Light small_1(&timer1, CHANNEL_3, SMALL_1);
Light small[] = {small_1};

// Light* allLights[] = {big, middle, small};
Light allLights[] = {big_1, big_2, big_3, middle_1, middle_2, small_1};

App::App() {
	small_1.setMinimumValue(MIN_INTENSITY + 200);
}


void App::init() {

	configureRcc();
	configureGpio();
	configureUart();

	printf("\n\nSystem initialized\n");

	initPWMs();

}

void App::initPWMs() {
	uint8_t i;
	for (i = 0; i < sizeof(PWMs) / sizeof(PWMs[0]); i++) {
		PWMs[i].init();
	}
}

void App::selfTest() {

	printf("Self testing ...\n");

	// printf("Found %d groups\n", sizeof(allLights) / sizeof(allLights[0]));

	FOR_ALL_LIGHTS({
		light->setValue(0x00);
	})

	FOR_ALL_LIGHTS({
		light->setValue(0xff);
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
