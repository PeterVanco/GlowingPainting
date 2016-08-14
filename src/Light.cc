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
	multiplicationFactor = 1.0;
}

void Light::setValue(const uint16_t value) {
	// printf("Setting %d to Light %i\n", value, lightId);
	this->value = value;
	pwmInstance->setChannel(pwmChannel, applyMultiplicationFactor(linearize(value)));
}

uint16_t Light::linearize(uint16_t value) {
	return led_linearization[value];
}

uint16_t Light::applyMultiplicationFactor(uint16_t value) {
	return value * multiplicationFactor;
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

void Light::setMultiplicationFactor(const float factor) {
	this->multiplicationFactor = factor;
}

uint16_t Light::step() {
	uint16_t result = value;
	result += (rand() % STEP) - (STEP / 2);
	result = result > max ? max : result;
	result = result < min ? min : result;
	return result;
}
