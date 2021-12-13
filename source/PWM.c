/*
 * PWM.c
 *
 *  Created on: 13-Dec-2021
 *      Author: Kevin Tom
 */


#include "PWM.h"
#include "MKL25Z4.h"
#include "command_processor.h"

#define TPM0_CHANNEL_1 (1)
#define TPM2_CHANNEL_0 (0)
#define TPM2_CHANNEL_1 (1)

#define transition_factor 0.01

//For interrupt handler and switch recognition
#define SWITCH_GPIO_PORT GPIOD
#define SWITCH_PIN 3
#define SWITCH_PIN_CTRL_REG PORTD->PCR[SWITCH_PIN]



/*This structure holds all the necessary variables for state machine*/
static struct
{
	double led_red;
	double led_green;
	double led_blue;
}current =
{
	.led_red = 00,
	.led_green = 00,
	.led_blue = 00
};



/*
 * This function sets PWM for specific pins according to value given
 *
 * The value is multiplied by 256 to map it to period of 48000 given as
 * PWN period while initializing, we are getting wide range of values to
 * control the led.
 *
 *
 * Parameters :
 *	red   - red   led tpm value
 *	green - green led tpm value
 *	blue  - blue  led tpm value
 *
 * Returns : None
 *
 * */
void tpm_function(int red, int green, int blue)
{
	TPM2->CONTROLS[0].CnV = (red  <<8);   //Red
	TPM2->CONTROLS[1].CnV = (green<<8); //Green
	TPM0->CONTROLS[1].CnV = (blue <<8);  //Blue
}


/*
 * This function helps in transition from any led color to any state color.
 *
 * The function calls tpm_function for setting the led colors.
 * The function will also set the state to final state after transition.
 *
 * Parameters :
 *		RED   : Final state RED LED color
 *		GREEN : Final state GREEN LED color
 *		BLUE  : Final state BLUE LED color
 *		state : State to which transition should happen
 *
 * Returns : None
 *
 * */
void transition_to_any_state(int RED,int GREEN, int BLUE)
{
	current.led_red   = ((RED -current.led_red)   *transition_factor)+current.led_red;
	current.led_green = ((GREEN-current.led_green)*transition_factor)+current.led_green;
	current.led_blue  = ((BLUE -current.led_blue) *transition_factor)+current.led_blue;

	tpm_function(current.led_red,current.led_green,current.led_blue);
}


/*
 * This function configures TPMs used in this project. This
 * function does the following;
 *  1. Select the clock for TPM
 *  2. Load the MOD register with value
 *  3. Configure DEBUG Mode behavior
 *  4. Set initial duty cycle
 *  5. Setting LPTPM counter to increment on every LPTPM counter clock
 *
 * Parameters :
 * 		uint16_t - Period to be loaded to TPM MOD.
 *
 * Returns : None
 *
 * */

void tpm_config(uint16_t period)
{
//TPM CONFIRGURATION

	//Select (MCGFLLCLK)/(MCGFLLCLK/2) and then select MCGFLLCLK/2
	SIM->SOPT2 |= (SIM_SOPT2_TPMSRC(1)); // | SIM_SOPT2_PLLFLLSEL_MASK);

	//Loading counter and MOD
	TPM0->MOD = period -1;
	TPM2->MOD = period -1;

	//CPWMS is set to 0 for up counting, no prescalar
	TPM0->SC = TPM_SC_PS(1);
	TPM2->SC = TPM_SC_PS(1);

	//Configuring behavior in DEBUG mode, 11 is continue normal operation
	TPM0->CONF |= TPM_CONF_DBGMODE(3);
	TPM2->CONF |= TPM_CONF_DBGMODE(3);

	//Configuring TPM0 channel 1 to edge aligned low-true PWM
	TPM0->CONTROLS[TPM0_CHANNEL_1].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK;
	TPM2->CONTROLS[TPM2_CHANNEL_0].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK;
	TPM2->CONTROLS[TPM2_CHANNEL_1].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK;

	//Set initial duty cycle
	TPM0->CONTROLS[TPM0_CHANNEL_1].CnV = 0;
	TPM2->CONTROLS[TPM2_CHANNEL_0].CnV = 0;
	TPM2->CONTROLS[TPM2_CHANNEL_1].CnV = 0;

	//Start TPM (LPTPM counter increments on every LPTPM counter clock)
	TPM0->SC |= TPM_SC_CMOD(1);
	TPM2->SC |= TPM_SC_CMOD(1);
}


