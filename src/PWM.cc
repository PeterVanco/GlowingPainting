#include "PWM.h"

#define NO_OF_CHANNELS	sizeof(PWM::CHANNELS) / sizeof(PWM::CHANNELS[0])

const Channel PWM::CHANNELS[] = {CHANNEL_1, CHANNEL_2, CHANNEL_3, CHANNEL_4};
void (*PWM::TIM_OCxInit[]) (TIM_TypeDef* TIMx, TIM_OCInitTypeDef* TIM_OCInitStruct) = {TIM_OC1Init, TIM_OC2Init, TIM_OC3Init, TIM_OC4Init};
void (*PWM::TIM_OCxPreloadConfig[]) (TIM_TypeDef* TIMx, uint16_t TIM_OCPreload) = {TIM_OC1PreloadConfig, TIM_OC2PreloadConfig, TIM_OC3PreloadConfig, TIM_OC4PreloadConfig};

PWM::PWM(TIM_TypeDef* timerInstance) {
	this->timerInstance = timerInstance;

	sOcInit.TIM_OCMode = TIM_OCMode_PWM1;
	sOcInit.TIM_OutputState = TIM_OutputState_Enable;
	sOcInit.TIM_OCPolarity = TIM_OCPolarity_Low;
	sOcInit.TIM_Pulse = TIM_OCPolarity_Low;
}

void PWM::init() {

	printf("Iniatilizing PWM on TIMER %#010x\n", timerInstance);

	sTimeBaseInit.TIM_Period = this->PERIOD;
	sTimeBaseInit.TIM_Prescaler = 0;
	sTimeBaseInit.TIM_ClockDivision = 0;
	sTimeBaseInit.TIM_CounterMode = TIM_CounterMode_CenterAligned3;

	TIM_TimeBaseInit(timerInstance, &sTimeBaseInit);

	setChannel(CHANNEL_1234, 0);

	TIM_ARRPreloadConfig(timerInstance, ENABLE);
	TIM_Cmd(timerInstance, ENABLE);
	TIM_CtrlPWMOutputs(timerInstance, ENABLE);
}

int PWM::isChannelMasked(uint8_t channelIndex, const uint8_t channels) {
	return CHANNELS[channelIndex] & channels;
}

void PWM::setChannel(const uint8_t channels,
					 uint16_t value) {
	sOcInit.TIM_Pulse = value > this->PERIOD ? this->PERIOD : value;
	uint8_t channelIndex;
	for (channelIndex = 0; channelIndex < NO_OF_CHANNELS; channelIndex++) {
		if (isChannelMasked(channelIndex, channels)) {
			(*TIM_OCxInit[channelIndex])(timerInstance, &sOcInit);
			(*TIM_OCxPreloadConfig[channelIndex])(timerInstance, TIM_OCPreload_Enable);
		}
	}
}
