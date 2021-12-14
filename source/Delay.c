/*
 * Delay.c
 *
 *  Created on: 13-Dec-2021
 *      Author: Kevin Tom
 *
 *      Brief : This file has the hardspin delay function
 */


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
void Delay (uint32_t dly)
{
  volatile uint32_t t;

	for (t=dly*10000; t>0; t--)
		;
}
