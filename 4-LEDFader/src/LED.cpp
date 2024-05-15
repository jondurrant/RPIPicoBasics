/*
 * LED.cpp
 *
 * Simple LED drive class. Drives LED brightness using PWM
 *
 *  Created on: 9 Jun 2022
 *      Author: jondurrant
 */

#include "LED.h"
#include "hardware/pwm.h"
#include <stdio.h>

/***
 * Setup LED on the GPIO pad
 * @param gp - GPIO pad number
 */
LED::LED(uint8_t gp) {
	setGP(gp);
}

/***
 * Construct without initialising GPIO

 */
LED::LED() {
	//NOP
}

LED::~LED() {
	// NOP
}

/***
 * Set the GPIO Pad and initialised it
 * @param gp - GPIO pad number
 */
void LED::setGP(uint8_t gp){
	xGP = gp;

	gpio_init(gp);

	//Setup up PWM to fade LED
	gpio_set_function(gp, GPIO_FUNC_PWM);
	pwm_set_gpio_level(gp, 0);
	uint slice_num = pwm_gpio_to_slice_num(gp);
	pwm_set_enabled(slice_num, true);
}

/***
 * Set brightness
 * @param b - brightness 0 to 0xff
 */
void LED::setBrightness(uint8_t b){
	xBrightness = b;
	uint16_t duty = b * b;
	pwm_set_gpio_level(xGP, duty);
}

void LED::setStepTimeMS(uint16_t step){
	xStepMS = step;
}

/***
 * Set the target mode
 * @param mode - LEDMode
 * @param target - target brightness for mode. Not used for OFF or LEDFade
 */
void LED::setMode(LEDMode mode, uint8_t target){
	xMode = mode;
	switch(mode){
	case LEDOff: {
		setBrightness(0);
		break;
	}
	case LEDOn:{
		if(target == 0){
			setBrightness(0xff);
		} else {
			setBrightness(target);
		}
		break;
	}
	case LEDFade:{
		xTarget = target;
		break;
	}
	case LEDFadeTo:{
		xTarget = target;
		break;
	}
	}

	//Reset laststep time
	xLastStep = to_ms_since_boot (get_absolute_time ());

}

/***
 * Needs to be called at frequent interval to trigger fade animation
 * Uses time internally so will handle irregulat calls
 */
void LED::poll(){
	//Only used in LEDFade and LEDFadeTo
	if ((xMode == LEDFade) || (xMode == LEDFadeTo)){

		//Handle if we have reached target
		if (xBrightness == xTarget){
			//In fade mode we reset target on being reached.
			if (xMode == LEDFade){
				if (xTarget == 0){
					xTarget = 0xFF;
				} else {
					xTarget = 0;
				}
			} else {
				return;
			}
		}

		//Calculate time since last action
		uint32_t now = to_ms_since_boot (get_absolute_time ());
		uint32_t sinceStep = now - xLastStep;
		uint32_t stepsDue = sinceStep / xStepMS;

		//IF we need to move ahead a number of steps in animation
		if (stepsDue > 0){
			xLastStep = now;

			if (xTarget > xBrightness){

				if (stepsDue > (xTarget - xBrightness)){
					setBrightness(xTarget);
				} else {
					setBrightness(xBrightness + stepsDue);
				}

			} else {
				if (stepsDue > (xBrightness - xTarget)){
					setBrightness(xTarget);
				} else {
					setBrightness(xBrightness - stepsDue);
				}
			}
		}
	}
}



