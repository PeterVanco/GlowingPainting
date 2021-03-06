#ifndef APP_H_
#define APP_H_


#include "main.h"
#include "PWM.h"
#include "Light.h"


#define LIGHT_INTENSITY_LOW_TRESHOLD	1500
#define LIGHT_INTENSITY_HIGH_TRESHOLD	2500
#define LIGHT_INTENSITY_CHECK_PERIOD	3

#define BUTTON_DEBOUNCE_TIME_MS			200
#define BUTTON_IDLE						-1

typedef enum {
	NOT_INITIALIZED,
	READY_NO_PWM,
	READY
} SystemState;


#ifdef __cplusplus
extern "C" {
#endif

class App {

	__IO SystemState systemState;

	void configureRcc(const bool initPWM);
	void configureGpio(const bool initPWM);
	void configureUart();

	void initPWMs();
	void initButton();
	void initADCWatchdog();
	void initRTC();

public:

	App();

	void init(const bool initPWM);
	SystemState getSystemState();
	void selfTest();
	void sleep();

	void start();
	void step();
	void stop();

	uint16_t readLightIntensity();

};

#ifdef __cplusplus
}
#endif
#endif
