/* Created on: 05-Oct-2021
 * Author: Kevin Tom
 *
 *
 *
 * init.c
 * *******
 * This C file deals with initialization of all components used in this project.
 * The file has initializations for the following,
 *
 * 	1. All 3 LEDs used in this project
 * 	2. TPM functionality to LED pins
 * 	3. Switch (hardware)
 * 	4. Switch interrupt (GPIO interrupt)
 * 	5. Touch sensor
 *
 *
 */

#include "MKL25Z4.h"
#include "init.h"



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






/*
 * This function initializes switch connected to PORT D, Pin 3. The
 * function does the following;
 *  1. Enables clock to PORT (PORT D)
 *  2. Set MUX function to 1 which is GPIO
 *  3. Enabling the Pull-up resistor and clearing PDDR at switch pin
 *  4. Setting interrupt to be triggered when zero is detected
 *
 * Parameters : None
 *
 * Returns : None
 *
 * */
void init_switch()
{
	//Enabling clock to PORTD
	SIM->SCGC5 |= SWITCH_SCGC5_MASK;

	//CLEARING THE MUX VALUES AND SETTING IT TO FUNCTION 1
	SWITCH_PIN_CTRL_REG &= ~PORT_PCR_MUX_MASK;
	SWITCH_PIN_CTRL_REG |= PORT_PCR_MUX(1);

	//Enabling the Pull-up resistor and clearing PDDR at switch pin
	SWITCH_PIN_CTRL_REG |= PORT_PCR_PE(1) | PORT_PCR_PS(1);
	SWITCH_GPIO_PORT->PDDR &= ~(1 << SWITCH_PIN);

	//Setting interrupt to be triggered when zero is detected
	SWITCH_PIN_CTRL_REG	|= PORT_PCR_IRQC(INTERRUPT_WHEN_ZERO);
}





/*
 * This function enables and configures interrupt for switch connected
 * to GPIO PORT D pin 3.
 *
 *
 * Parameters : None
 *
 * Returns : None
 *
 *
 * */
void init_switch_interrupt()
{
	NVIC_SetPriority(PORTD_IRQn,3);
	NVIC_ClearPendingIRQ(PORTD_IRQn);
	NVIC_EnableIRQ(PORTD_IRQn);

	__enable_irq();
}




/*
 * This function will activate clock to PORT used by TSI,
 * then initializes GENCS register with require values.
 *
 * Parameters : None
 *
 * Returns : None
 *
 *
 * This function is leveraged from,
 * https://github.com/alexander-g-dean/ESF/tree/master/NXP/Misc/Touch%20Sense
 *
 */
void touch_init()
{
	//Enabling clock to port B - For Touch sensor inputs
	SIM->SCGC5 |= SIM_SCGC5_TSI_MASK;


	//Setting MUX values
	TSI0->GENCS = TSI_GENCS_MODE(0u) | //operating in non-noise mode
								TSI_GENCS_REFCHRG(0u) | //reference oscillator charge and discharge value 500nA
								TSI_GENCS_DVOLT(0u) | //oscillator voltage rails set to default
								TSI_GENCS_EXTCHRG(0u) | //electrode oscillator charge and discharge value 500nA
								TSI_GENCS_PS(0u) |  // frequency clcok divided by one
								TSI_GENCS_NSCN(31u) | //scanning the electrode 32 times
								TSI_GENCS_TSIEN_MASK | //enabling the TSI module
								TSI_GENCS_EOSF_MASK; // writing one to clear the end of scan flag

}
