/*
 * PWM.h
 *
 *  Created on: 13-Dec-2021
 *      Author: Kevin Tom
 */

#ifndef PWM_H_
#define PWM_H_

#include <stdint.h>


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
void transition(int RED,int GREEN, int BLUE);

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
void tpm_function(int red, int green, int blue);


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
void tpm_config(uint16_t period);

#endif /* PWM_H_ */
