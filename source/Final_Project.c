/**
 * @file    Final_Project.c
 * @brief   Application entry point.
 */
#include <math.h>
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

#include <stdio.h>

/* TODO: insert other include files here. */









/* TODO: insert other definitions and declarations here. */

/*
 * @brief   Application entry point.
 */
int main(void)
{

	sysclock_init();

	Init_UART0(BAUD_RATE);

	test_cbfifo();


    init_switch();

    init_switch_interrupt();

	i2c_init();										/* init i2c	*/

	led_init();

	tpm_config(48000);

	startup_test();





	//Init_RGB_LEDs();

	if (!init_mma())
	{												/* init mma peripheral */
		//Control_RGB_LEDs(1, 0, 0);
		printf("\n\rCouldn't initialize Accelerometer");
		tpm_function(255,0,0);
		while (1)
			;
	}

	command_processor_start();

    return 0 ;
}
