#include <stddef.h>
#include <stdlib.h>
#include "stm32f10x.h"

TIM_OCInitTypeDef TIM_OCInitStructure;
TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

void RCC_Configuration(void);
void NVIC_Configuration(void);
void GPIO_Configuration(void);
void PERIPH_Configuration(void);

void Delay(vu32 nCount) {
	while (nCount--)
		;
}

void SetColor(uint32_t R, uint32_t G, uint32_t B) {

	TIM_OCInitStructure.TIM_Pulse = 0xFF - R;
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable );

	TIM_OCInitStructure.TIM_Pulse = 0xFF - G;
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable );

	TIM_OCInitStructure.TIM_Pulse = 0xFF - B;
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable );

}

class Trieda {

public:
	int x;

	void test(int a) {
		this->x = a + 10;
	}
};

int main(void) {

	Trieda t;
	t.test(2);

	RCC_Configuration();
	GPIO_Configuration();
	NVIC_Configuration();
	PERIPH_Configuration();

	/*
	SetColor(0xFF, 0, 0);
	Delay(0x3FFFFF);
	SetColor(0, 0xFF, 0);
	Delay(0x3FFFFF);
	SetColor(0, 0, 0xFF);
	Delay(0x3FFFFF);
	*/

	uint32_t r;

	#define _DELAY	0x5FFF

	#define STEP			0x1f
	#define MIN_INTENSITY	0x2f
	#define MAX_INTENSITY	0x5f

	uint16_t last1 = rand() % 0xff;
	uint16_t last2 = rand() % 0xff;
	uint16_t last3 = rand() % 0xff;

	while (1) {
		last1 += (rand() % STEP) - (STEP / 2);
		last2 += (rand() % STEP) - (STEP / 2);
		last3 += (rand() % STEP) - (STEP / 2);

		last1 = last1 > MAX_INTENSITY ? MAX_INTENSITY : last1;
		last2 = last2 > MAX_INTENSITY ? MAX_INTENSITY : last2;
		last3 = last3 > MAX_INTENSITY ? MAX_INTENSITY : last3;

		last1 = last1 < MIN_INTENSITY ? MIN_INTENSITY : last1;
		last2 = last2 < MIN_INTENSITY ? MIN_INTENSITY : last2;
		last3 = last3 < MIN_INTENSITY ? MIN_INTENSITY : last3;

		SetColor(last1, last2, last3);
		Delay(0x8fff);
	}

	while (1) {
		for (r = 0; r < 0xFF; r++) {
			SetColor(0xFF, r, 0);
			Delay(_DELAY);
		}

		for (r = 0xFF; r > 0; r--) {
			SetColor(r, 0xFF, 0);
			Delay(_DELAY);
		}

		for (r = 0; r < 0xFF; r++) {
			SetColor(0, 0xFF, r);
			Delay(_DELAY);
		}

		for (r = 0xFF; r > 0; r--) {
			SetColor(0, r, 0xFF);
			Delay(_DELAY);
		}

		for (r = 0; r < 0xFF; r++) {
			SetColor(r, 0, 0xFF);
			Delay(_DELAY);
		}

		for (r = 0xFF; r > 0; r--) {
			SetColor(0xFF, 0, r);
			Delay(_DELAY);
		}
	}
}

void NVIC_Configuration(void) {
	//NVIC_InitTypeDef NVIC_InitStructure;

}

void RCC_Configuration(void) {

	RCC_APB2PeriphClockCmd(
			RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO | RCC_APB2Periph_TIM1,
			ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 | RCC_APB1Periph_TIM2, ENABLE);
}

void GPIO_Configuration(void) {

	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0 ) == Bit_SET) {
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);

		GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
		GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE);
		GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);
	}
}

// TIM3-3, TIM3-4, TIM1-1

void PERIPH_Configuration(void) {

	TIM_TimeBaseStructure.TIM_Period = 0xFF;
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_CenterAligned3;

	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_Pulse = TIM_OCPolarity_High;

	TIM_ARRPreloadConfig(TIM1, ENABLE);
	TIM_ARRPreloadConfig(TIM2, ENABLE);
	TIM_ARRPreloadConfig(TIM3, ENABLE);

	TIM_Cmd(TIM1, ENABLE);
	TIM_Cmd(TIM2, ENABLE);
	TIM_Cmd(TIM3, ENABLE);

	TIM_CtrlPWMOutputs(TIM1, ENABLE);
	TIM_CtrlPWMOutputs(TIM2, ENABLE);
	TIM_CtrlPWMOutputs(TIM3, ENABLE);
}

void __assert_func(const char *file, int line, const char *func,
		const char *failedexpr) {
	while (1) {
	}
}

void __assert(const char *file, int line, const char *failedexpr) {
	__assert_func(file, line, NULL, failedexpr);
}
