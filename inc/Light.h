#ifndef LIGHT_H_
#define LIGHT_H_

#include "PWM.h"
#include "stdlib.h"
#include "led_linearization.h"

#ifdef __cplusplus
extern "C" {
#endif


#define STEP			31 // must be odd
#define MIN_INTENSITY	400
#define MAX_INTENSITY	800


class Light {

	PWM* pwmInstance;
	Channel pwmChannel;
	int16_t lightId;
	uint16_t min;
	uint16_t max;
	__IO uint16_t value;
	float lightFactor;

	uint16_t linearize(uint16_t value);
	uint16_t adjustToLightIntensity(uint16_t value);

public:

	Light(PWM* pwmInstance,
		  Channel pwmChannel);

	Light(PWM* pwmInstance,
		  Channel pwmChannel,
		  int16_t lightId);

	void setValue(const uint16_t value);
	uint16_t getValue();

	void setMinimumValue(const uint16_t min);
	void setMaximumValue(const uint16_t max);
	void setLightFactor(const float factor);

	uint16_t step();

};

#ifdef __cplusplus
}
#endif
#endif
