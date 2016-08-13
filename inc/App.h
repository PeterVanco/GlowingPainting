#ifndef APP_H_
#define APP_H_


#include "main.h"
#include "PWM.h"
#include "Light.h"


const uint16_t LIGHT_INTENSITY_LOW_TRESHOLD = 1200;
const uint16_t LIGHT_INTENSITY_HIGH_TRESHOLD = 2000;


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
	void initADCWatchdog();
	void initRTC();

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
