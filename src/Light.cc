#include "Light.h"

Light::Light(PWM* pwmInstance,
		  	 Channel pwmChannel) {
	Light(pwmInstance, pwmChannel, -1);
}

Light::Light(PWM* pwmInstance,
		  	 Channel pwmChannel,
		  	 int16_t lightId) {
	this->pwmInstance = pwmInstance;
	this->pwmChannel = pwmChannel;
	this->lightId = lightId;
	min = MIN_INTENSITY;
	max = MAX_INTENSITY;
	value = min;
}

void Light::setValue(const uint16_t value) {
	printf("Setting %d to Light %i\n", value, lightId);
	this->value = value;
	pwmInstance->setChannel(pwmChannel, led_linearization[value]);
}

uint16_t Light::getValue() {
	return value;
}

void Light::setMinimumValue(const uint16_t min) {
	this->min = min;
}

void Light::setMaximumValue(const uint16_t max) {
	this->max = max;
}

uint16_t Light::step() {
	uint16_t result = value;
	result += (rand() % STEP) - (STEP / 2);
	result = result > max ? max : result;
	result = result < min ? min : result;
	return result;
}
