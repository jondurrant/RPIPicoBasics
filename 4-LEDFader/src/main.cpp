/*
 * main.cpp
 *
 *  Created on: 9 Jun 2022
 *      Author: jondurrant
 */

#include "LED.h"
#include <stdio.h>

#define DELAY 500 // in microseconds

#define LED_PAD 2 // GP2


int main(){
	uint32_t start = to_ms_since_boot (get_absolute_time ());
	uint32_t sinceStart;
	uint32_t now;
	uint8_t demo = 0;

	stdio_init_all();

	LED led(LED_PAD);

	led.setMode(LEDOn, 0xff);


	while (true){
		led.poll();

		now = to_ms_since_boot (get_absolute_time ());
		sinceStart = now - start;

		if (sinceStart > 5000){
			start = now;
			demo++;
			switch(demo){
			case 1:{
				printf("LEDOn\n");
				led.setMode(LEDOn);
				break;
			}
			case 2:{
				printf("LEDOff\n");
				led.setMode(LEDOff);
				break;
			}
			case 3:{
				printf("LEDFadeTo Full On\n");
				led.setMode(LEDFadeTo, 0xff);
				break;
			}
			case 4:{
				printf("LEDFadeTo 50%\n");
				led.setMode(LEDFadeTo, 0x80);
				break;
			}
			case 6:{
				printf("LEDFadeTo Off\n");
				led.setMode(LEDFadeTo, 0x00);
				break;
			}
			case 8:{
				printf("LEDFade\n");
				led.setMode(LEDFade);
			}
			case 20:{
				demo = 0;
			}
			}

		}
	}
}


