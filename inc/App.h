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
	void initPWMs();
	void configureUart();

public:

	App();

	void init();
	void selfTest();
	void step();

};

#ifdef __cplusplus
}
#endif
#endif