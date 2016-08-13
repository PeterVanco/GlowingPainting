#ifndef PWM_H_
#define PWM_H_

#include "stdio.h"
#include "stm32f10x.h"

typedef enum _Channel {
	CHANNEL_1 = 1 << 0,
	CHANNEL_2 = 1 << 1,
	CHANNEL_3 = 1 << 2,
	CHANNEL_4 = 1 << 3,
	CHANNEL_1234 = CHANNEL_1 | CHANNEL_2 | CHANNEL_3 | CHANNEL_4
} Channel;

#ifdef __cplusplus
extern "C" {
#endif

class PWM {

	static const uint16_t PERIOD = 1000;
	static const Channel CHANNELS[];
	static void (*TIM_OCxInit[]) (TIM_TypeDef* TIMx, TIM_OCInitTypeDef* TIM_OCInitStruct);
	static void (*TIM_OCxPreloadConfig[]) (TIM_TypeDef* TIMx, uint16_t TIM_OCPreload);
	int isChannelMasked(uint8_t channelIndex, const uint8_t channels);

public:

	TIM_TypeDef* timerInstance;
	TIM_TimeBaseInitTypeDef sTimeBaseInit;
	TIM_OCInitTypeDef sOcInit;

	PWM(TIM_TypeDef* timerInstance);

	void init();
	void setChannel(const uint8_t channels,
					const uint16_t value);

};

#ifdef __cplusplus
}
#endif
#endif
