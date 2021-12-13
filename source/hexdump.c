/*
 * hexdump.c
 *
 *  Created on: 09-Nov-2021
 *      Author: Kevin Tom, Kevin.Tom@colorado.edu
 *
 *
 *      This file has the hexdump function
 */

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "hexdump.h"







/* This converts integer input to hexadecimal character
 *
 *
 * Parameters:
 * 		uint32_t x - Number to be converted
 *
 * Returns:
 * 		char       - Hexadecimal equivalent
 * */
char int_to_hexchar(uint32_t x)
{
	if (x < 10)
		return ('0' + x);
	else
		return ('A' + (x - 10));
}





/* This function prints the hexdump starting from an address till the
 * given length.
 *
 * Leveraged code from,
 *  PES - ECEN: 5813 - Lecture 5 - Testing and Assignment 1 Review
 *
 *
 * Parameters:
 * 		int *start - Start address
 * 		size_t len - Total number of locations to be printed
 *
 * Returns:
 * 		None
 * */
void hexdump(int *start, size_t len)
{
	uint8_t *buf = (uint8_t*) start;
	int start_addr = (int)start;


	//Truncating i the len is greater than MAX Size (640)
	if(len > DUMP_MAX_SIZE)
	{
		len = DUMP_MAX_SIZE;
	}

	//Printing in a new line
	printf("\n\r");

	for(int i =0;i<len;i+=STRIDE)
	{
		//Printing the address
		printf("%04x_%04x",(start_addr & (0xFFFF0000)),(start_addr & (0x0000FFFF)));
	    printf("  ");

	    //Printing the memory content in hex
	    for (int j=0; (j < STRIDE) && (i+j < len); j++)
		{
	    	printf("%c",int_to_hexchar((buf[i+j]) >> 4));
			printf("%c",int_to_hexchar((buf[i+j]) & 0x0f));
			printf(" ");
		}

	    //Incrementing the address by STRIDE
		start_addr += STRIDE;

		//Going to new line
		printf("\r");
		printf("\n");
	 }
}




