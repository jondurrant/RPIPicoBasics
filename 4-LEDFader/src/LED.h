/*
 * LED.h
 *
 * Object to manage an LED with brightness and
 * fade animation
 *
 *  Created on: 9 Jun 2022
 *      Author: jondurrant
 */

#ifndef LED_LEDBRIGHTNESS_SRC_LED_H_
#define LED_LEDBRIGHTNESS_SRC_LED_H_

#include <stdlib.h>
#include "pico/stdlib.h"
#include "pico/time.h"

//Enumeration for LED modes
enum LEDMode {LEDOff, LEDOn, LEDFade, LEDFadeTo};

class LED {
public:
	/***
	 * Constructor without initialising LED
	 */
	LED();

	/***
	 * Setup LED on the GPIO pad
	 * @param gp - GPIO pad number
	 */
	LED(uint8_t gp);

	/***
	 * Destructor
	 */
	virtual ~LED();

	/***
	 * Set the GPIO Pad and initialised it
	 * @param gp - GPIO pad number
	 */
	void setGP(uint8_t gp);

	/***
	 * Set brightness
	 * @param b - brightness 0 to 0xff
	 */
	void setBrightness(uint8_t b);


	/***
	 * Needs to be called at frequent interval to trigger fade animation
	 * Uses time internally so will handle irregulat calls
	 */
	void poll();

	/***
	 * Set the target mode
	 * @param mode - LEDMode
	 * @param target - target brightness for mode. Not used for OFF or LEDFade
	 */
	void setMode(LEDMode mode, uint8_t target = 0);

	/***
	 * Set time in milliseconds of each step in the fade cycle.
	 * 0xff steps to go from off to full on
	 * @param step
	 */
	void setStepTimeMS(uint16_t step);


private:
	// GPIO Pad number
	uint8_t xGP = 0;

	//Current brightness
	uint8_t xBrightness = 0;

	//Target brightness
	uint8_t xTarget = 0;

	//Current mode
	LEDMode xMode = LEDOff;

	//See setStepTimeMS
	uint16_t xStepMS = 3;

	//Time since boot of the last step in MS
	uint32_t xLastStep = 0;

};

#endif /* LED_LEDBRIGHTNESS_SRC_LED_H_ */
