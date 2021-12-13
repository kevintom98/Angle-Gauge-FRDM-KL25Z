/*
 * switch.c
 *
 *  Created on: 13-Dec-2021
 *      Author: Kevin Tom
 */
#include "MKL25Z4.h"

// SWITCH is connected to PORT D, pin 3
#define SWITCH_GPIO_PORT GPIOD
#define SWITCH_PIN 3
#define SWITCH_PIN_CTRL_REG PORTD->PCR[SWITCH_PIN]
#define SWITCH_SCGC5_MASK SIM_SCGC5_PORTD_MASK


#define INTERRUPT_WHEN_ZERO 8

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
