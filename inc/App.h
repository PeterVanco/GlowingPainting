#ifndef APP_H_
#define APP_H_


#include "main.h"
#include "PWM.h"
#include "Light.h"


#ifdef __cplusplus
extern "C" {
#endif

class App {

	void configureRcc();
	void configureGpio();
	void configureUart();

	void initPWMs();
	void initButton();
	void initADC();

public:

	App();

	void init();
	void selfTest();
	void step();
	void sleep();

	uint16_t readLightIntensity();

};

#ifdef __cplusplus
}
#endif
#endif
