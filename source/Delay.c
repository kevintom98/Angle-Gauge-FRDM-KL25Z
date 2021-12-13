/*
 * Delay.c
 *
 *  Created on: 13-Dec-2021
 *      Author: Kevin Tom
 */


#include <stdint.h>

void Delay (uint32_t dly)
{
  volatile uint32_t t;

	for (t=dly*10000; t>0; t--)
		;
}
