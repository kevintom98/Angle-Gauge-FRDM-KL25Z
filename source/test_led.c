/*
 * test_led.c
 *
 *  Created on: 13-Dec-2021
 *      Author: Kevin Tom
 */


#include <stdio.h>
#include "PWM.h"
#include "Delay.h"



/*
 * This function runs automatic tests to test LEDs
 *
 * Parameters : None
 *
 * Returns : None
 *
 * */
void led_test()
{

	printf("\n\rLED TESTING");
	printf("\n\r***********");

	tpm_function(255,0,0);
	Delay(500);
	tpm_function(0,0,0);
	Delay(100);
	printf("\n\rRed led tested");

	tpm_function(0,255,0);
	Delay(500);
	tpm_function(0,0,0);
	Delay(100);
	printf("\n\rGreen led tested");

	tpm_function(0,0,255);
	Delay(500);
	tpm_function(0,0,0);
	Delay(100);
	printf("\n\rBlue led tested");

	tpm_function(255,255,255);
	Delay(500);
	tpm_function(0,0,0);
	Delay(100);
	printf("\n\rAll LEDs tested for working");

}


