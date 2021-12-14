/*
 * PWM.h
 *
 *  Created on: 13-Dec-2021
 *      Author: Kevin Tom
 */

#ifndef PWM_H_
#define PWM_H_

#include <stdint.h>

void transition(int RED,int GREEN, int BLUE);
void tpm_function(int red, int green, int blue);
void tpm_config(uint16_t period);

#endif /* PWM_H_ */
