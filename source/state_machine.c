/*
 * state_machine.c
 *
 *  Created on: 13-Dec-2021
 *      Author: Kevin Tom
 */

#include <math.h>
#include <stdio.h>
#include "Accelerometer.h"
#include "i2c.h"
#include "sysclock.h"
#include "test_cbfifo.h"
#include "MKL25Z4.h"
#include "UART.h"
#include "command_processor.h"
#include "PWM.h"
#include "switch.h"
#include "led.h"
#include "test_led.h"
#include "test_accelerometer.h"



typedef enum
{
	INIT,
	TESTING,
	COMMAND_PROCESSING
}state_t;

state_t state = INIT;




void initialize_systems()
{
	sysclock_init();

	Init_UART0(BAUD_RATE);

	test_cbfifo();

    init_switch();

    init_switch_interrupt();

	i2c_init();

	led_init();

	tpm_config(48000);

	if (!init_mma())
	{
		printf("\n\rCouldn't initialize Accelerometer");
		tpm_function(255,0,0);
		while (1)
			;
	}
}


void test_system()
{
	led_test();

	test_accelerometer();

}







void statemachine()
{
	while(1)
	{
		switch(state)
		{
		case INIT:
			{
			initialize_systems();
			state = TESTING;
			break;
			}
		case TESTING:
			{
				test_system();
				state = COMMAND_PROCESSING;
				break;
			}
		case COMMAND_PROCESSING:
			{
				command_processor_start();
				break;
			}

		}

	}

}
