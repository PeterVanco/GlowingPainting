#ifndef APP_H_
#define APP_H_


#include "main.h"
#include "PWM.h"
#include "Light.h"


#define LIGHT_INTENSITY_LOW_TRESHOLD	3800
#define LIGHT_INTENSITY_HIGH_TRESHOLD	4000
#define LIGHT_INTENSITY_CHECK_PERIOD	3


#ifdef __cplusplus
extern "C" {
#endif

class App {

	bool initialized;

	void configureRcc();
	void configureGpio();
	void configureUart();

	void initPWMs();
	void initButton();
	void initADCWatchdog();
	void initRTC();

public:

	App();

	void init();
	bool isInitialized();
	void selfTest();
	void sleep();

	void step();

	uint16_t readLightIntensity();

};

#ifdef __cplusplus
}
#endif
#endif
