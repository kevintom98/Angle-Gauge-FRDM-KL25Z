/*
 * led.c
 *
 *  Created on: 13-Dec-2021
 *      Author: Kevin Tom
 */

#include "MKL25Z4.h"
#define RED_LED_PIN   (18)
#define GREEN_LED_PIN (19)
#define BLUE_LED_PIN  (1)

#define ALTERNATE_FUNCTION_3 (3)
#define ALTERNATE_FUNCTION_4 (4)

/*
 * This function initializes all the LEDs used in this project. The
 * function does the following;
 *  1. Enables clock to PORTs (PORT B and PORT D)
 *  2. Enables clock to TPM0 and TPM2
 *  3. Set LED Pins to Alternate functions (PWM functions)
 *
 * Parameters : None
 *
 * Returns : None
 *
 * */
void led_init()
{

//ENABLING CLOCK

			//Enabling clock to port B - For RED and GREEN LED's
			SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;;
	    	//Enabling clock to port D - For BLUE LED
	    	SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;;

	    	//Enabling Clock to TPM0 and TPM2
	    	SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK; //For RED and GREEN
	    	SIM->SCGC6 |= SIM_SCGC6_TPM2_MASK; //For BLUE


//SETTING MUX VALUES

	    	//CLEARING THE MUX VALUES AND SETTING IT TO ALTERNATE 3
	    	PORTB->PCR[RED_LED_PIN] &= ~PORT_PCR_MUX_MASK;
	    	PORTB->PCR[RED_LED_PIN] |= PORT_PCR_MUX(ALTERNATE_FUNCTION_3); //Alternate Function 3 is TPM

	    	//CLEARING THE MUX VALUES AND SETTING IT TO ALTERNATE 3
	    	PORTB->PCR[GREEN_LED_PIN] &= ~PORT_PCR_MUX_MASK;
	    	PORTB->PCR[GREEN_LED_PIN] |= PORT_PCR_MUX(ALTERNATE_FUNCTION_3); //Alternate Function 3 is TPM

	    	//CLEARING THE MUX VALUES AND SETTING IT TO ALTERNATE 4
	    	PORTD->PCR[BLUE_LED_PIN] &= ~PORT_PCR_MUX_MASK;
	    	PORTD->PCR[BLUE_LED_PIN] |= PORT_PCR_MUX(ALTERNATE_FUNCTION_4); //Alternate Function 4 is TPM
}
