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
#include "test_accelerometer.h"

#include <stdio.h>

/* TODO: insert other include files here. */

typedef enum
{
	INIT,
	TESTING,
	COMMAND_PROCESSING
}state_t;

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

	printf("\n\rStarting state machine");
}


/* TODO: insert other definitions and declarations here. */

/*
 * @brief   Application entry point.
 */
int main(void)
{

	initialize_systems();

	test_system();

	command_processor_start();




    return 0 ;
}
