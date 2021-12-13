/*
 * test_led.c
 *
 *  Created on: 13-Dec-2021
 *      Author: Kevin Tom
 */
#include "PWM.h"
#include "Delay.h"


void startup_test()
{

	tpm_function(255,0,0);
	Delay(500);
	tpm_function(0,0,0);
	Delay(100);

	tpm_function(0,255,0);
	Delay(500);
	tpm_function(0,0,0);
	Delay(100);

	tpm_function(0,0,255);
	Delay(500);
	tpm_function(0,0,0);
	Delay(100);

	tpm_function(255,255,255);
	Delay(500);
	tpm_function(0,0,0);
	Delay(100);

}


