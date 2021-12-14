/*
 * Delay.c
 *
 *  Created on: 13-Dec-2021
 *      Author: Kevin Tom
 *
 *      Brief : Header file for Delay.h
 */

#ifndef DELAY_H_
#define DELAY_H_

#include <stdint.h>

/* This function creats a hardspin delay
 *
 * Parameters:
 * 	uint32_t - delay needed in ~millisceonds
 *
 * Returns:
 * None
 *
 *
 * */
void Delay (uint32_t dly);

#endif /* DELAY_H_ */
