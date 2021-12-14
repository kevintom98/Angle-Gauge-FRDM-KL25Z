/*
 * led.h
 *
 *  Created on: 13-Dec-2021
 *      Author: Kevin Tom
 */

#ifndef LED_H_
#define LED_H_

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
void led_init();

#endif /* LED_H_ */
