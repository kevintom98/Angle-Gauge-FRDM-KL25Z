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
#include "init.h"

#include <stdio.h>

/* TODO: insert other include files here. */







/*#define LED1_POS (1)	// on port A
#define LED2_POS (2)	// on port A
#define SW1_POS (5)		// on port A

#define MASK(x) (1UL << (x))

// Speaker output
#define SPKR_POS (0) 	// on port C

// Freedom KL25Z LEDs
#define RED_LED_POS (18)		// on port B
#define GREEN_LED_POS (19)	// on port B
#define BLUE_LED_POS (1)*/





//LED Pin Macros
#define RED_LED_PIN   (18)
#define GREEN_LED_PIN (19)
#define BLUE_LED_PIN   (1)

//Defines ports used by LED pins
#define RED_LED_GPIO_PORT   GPIOB
#define GREEN_LED_GPIO_PORT GPIOB
#define BLUE_LED_GPIO_PORT  GPIOD

//Ports for LEDs
#define RED_LED_PORT   PORTB
#define GREEN_LED_PORT PORTB
#define BLUE_LED_PORT  PORTD


/*void Init_RGB_LEDs(void)
{
	// Enable clock to ports B and D
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTD_MASK;;

	// Make 3 pins GPIO
	PORTB->PCR[RED_LED_POS] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[RED_LED_POS] |= PORT_PCR_MUX(1);
	PORTB->PCR[GREEN_LED_POS] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[GREEN_LED_POS] |= PORT_PCR_MUX(1);
	PORTD->PCR[BLUE_LED_POS] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[BLUE_LED_POS] |= PORT_PCR_MUX(1);

	// Set ports to outputs
	PTB->PDDR |= MASK(RED_LED_POS) | MASK(GREEN_LED_POS);
	PTD->PDDR |= MASK(BLUE_LED_POS);
}

void Control_RGB_LEDs(unsigned int red_on, unsigned int green_on, unsigned int blue_on) {
	if (red_on)
	{
			PTB->PCOR = MASK(RED_LED_POS);
	}
	else
	{
			PTB->PSOR = MASK(RED_LED_POS);
	}
	if (green_on)
	{
			PTB->PCOR = MASK(GREEN_LED_POS);
	}
	else
	{
			PTB->PSOR = MASK(GREEN_LED_POS);
	}
	if (blue_on)
	{
			PTD->PCOR = MASK(BLUE_LED_POS);
	}
	else
	{
			PTD->PSOR = MASK(BLUE_LED_POS);
	}
}*/


void led_init_1()
{

//ENABLING CLOCK

		//Enabling clock to port B - For RED and GREEN LED's
		SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
    	//Enabling clock to port D - For BLUE LED
    	SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;


//SETTING MUX VALUES

    	//CLEARING THE MUX VALUES
    	RED_LED_PORT->PCR[RED_LED_PIN] &= ~PORT_PCR_MUX_MASK;
    	//THEN SETTING IT TO GPIO - ALT1
    	RED_LED_PORT->PCR[RED_LED_PIN] |= PORT_PCR_MUX(1);
    	//CLEARING THE MUX VALUES - ALT1
    	GREEN_LED_PORT->PCR[GREEN_LED_PIN] &= ~PORT_PCR_MUX_MASK;
    	//THEN SETTING IT TO GPIO - ALT1
    	GREEN_LED_PORT->PCR[GREEN_LED_PIN] |= PORT_PCR_MUX(1);
    	//CLEARING THE MUX VALUES - ALT1
    	BLUE_LED_PORT->PCR[BLUE_LED_PIN] &= ~PORT_PCR_MUX_MASK;
    	//THEN SETTING IT TO GPIO - ALT1
    	BLUE_LED_PORT->PCR[BLUE_LED_PIN] |= PORT_PCR_MUX(1);


//DECLARING PINS AS OUTPUTS

    	//Declaring GPIO pins are output
    	//Clearing LEDs so that it wont turn on while declaring GPIO as output
    	//Using RED_LED_GPIO_PORT for GREEN and RED as PORTS are the same (PORTB)
    	RED_LED_GPIO_PORT->PSOR =  (1 << GREEN_LED_PIN) | (1 << RED_LED_PIN);
    	BLUE_LED_GPIO_PORT->PSOR = (1 << BLUE_LED_PIN);

    	//Setting them as output
    	//Using RED_LED_GPIO_PORT for GREEN and RED as PORTS are the same (PORTB)
    	RED_LED_GPIO_PORT->PDDR = (1<<RED_LED_PIN) | (1<<GREEN_LED_PIN);
    	BLUE_LED_GPIO_PORT->PDDR |= (1<<BLUE_LED_PIN);

}

void startup_test()
{

	//Turning RED LED ON for 500ms
	RED_LED_GPIO_PORT->PCOR |= (1 << RED_LED_PIN);
	Delay(500);
	//Turning RED LED OFF for 100ms
	RED_LED_GPIO_PORT->PSOR |= (1 << RED_LED_PIN);
	Delay(100);


	//Turning GREEN ON for 500ms
	GREEN_LED_GPIO_PORT->PCOR |= (1 << GREEN_LED_PIN);
  	Delay(500);
  	//Turning RED GREEN OFF for 100ms
  	GREEN_LED_GPIO_PORT->PSOR |= (1 << GREEN_LED_PIN);
    Delay(100);


    //Turning BLUE ON for 500ms
    BLUE_LED_GPIO_PORT->PCOR |= (1 << BLUE_LED_PIN);
    Delay(500);
    //Turning BLUE OFF for 100ms
    BLUE_LED_GPIO_PORT->PSOR |= (1 << BLUE_LED_PIN);
    Delay(100);


	//Turning ALL LEDs ON for 100ms
	RED_LED_GPIO_PORT->PCOR =  (1 << RED_LED_PIN) | (1 << GREEN_LED_PIN);
	BLUE_LED_GPIO_PORT->PCOR = (1 << BLUE_LED_PIN);
	Delay(100);
	//Turning ALL LEDs OFF for 100ms
	RED_LED_GPIO_PORT->PSOR =  (1 << GREEN_LED_PIN) | (1 << RED_LED_PIN);
	BLUE_LED_GPIO_PORT->PSOR = (1 << BLUE_LED_PIN);
	Delay(100);


	//Turning ALL LEDs ON for 100ms
	RED_LED_GPIO_PORT->PCOR =  (1 << GREEN_LED_PIN) | (1 << RED_LED_PIN);
	BLUE_LED_GPIO_PORT->PCOR = (1 << BLUE_LED_PIN);
	Delay(100);
	//Turning ALL LEDs OFF for 100ms
	RED_LED_GPIO_PORT->PSOR = (1 << GREEN_LED_PIN) | (1 << RED_LED_PIN);
	BLUE_LED_GPIO_PORT->PSOR = (1 << BLUE_LED_PIN);
	Delay(100);
}



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


	led_init_1();

	printf("\n\rTesting LEDs");
	startup_test();

	led_init();

	tpm_config(48000);

	//Init_RGB_LEDs();

	if (!init_mma())
	{												/* init mma peripheral */
		//Control_RGB_LEDs(1, 0, 0);
		printf("\n\rCouldn't initialize Accelerometer");
		RED_LED_GPIO_PORT->PCOR |= (1 << RED_LED_PIN);
		while (1)
			;
	}

	command_processor_start();

	//int a = 1;



	//Delay(1000);

	/*while(1)
	{
		printf("\n\rPlease enter a character");

		a = my_nonblocking_getchar();

		//if(a == 48)
		//	printf("\n\rNothing received");


		if(a!= 0)
			printf("\n\rValue : %d",a);

		read_full_xyz();
		convert_xyz_to_roll_pitch();

		Delay(250);
	}*/

	/*while(1)
	{
		read_full_xyz();
		convert_xyz_to_roll_pitch();
		// Light green LED if pitch > 10 degrees
		// Light blue LED if roll > 10 degrees
		Control_RGB_LEDs(0, (fabs(roll) > 10)? 1:0, (fabs(pitch) > 10)? 1:0);
		Delay(500);

	    //Starting the command processor
	    //command_processor_start();
	}*/



    return 0 ;
}
