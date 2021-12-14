/*
 * switch.h
 *
 *  Created on: 13-Dec-2021
 *      Author: Kevin Tom
 */

#ifndef SWITCH_H_
#define SWITCH_H_



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
void init_switch();

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
void init_switch_interrupt();

#endif /* SWITCH_H_ */
