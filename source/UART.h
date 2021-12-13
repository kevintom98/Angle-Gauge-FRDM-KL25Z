/*
 *  File name : UART.h
 *
 *  Created on: 04-Nov-2021
 *      Author: Kevin Tom
 *
 *  Description
 *  -----------
 *  This file has the #defines function definitions
 *
 *
 */


#ifndef UART_H
#define UART_H

#include <stdint.h>
#include <MKL25Z4.H>



#define UART_OVERSAMPLE_RATE 	(16)    //Oversampling rate for the UART
#define BUS_CLOCK 				(24e6)  //Bus Clock selection
#define SYS_CLOCK				(24e6)  //System Clock selection


//Parameters according to assignment
#define STOP_BITS               (1)     // [0] : One Stop bit       | [1] : Two stop bits
#define DATA_SIZE				(0)     // [0] : 8-bit mode         | [1] : for 9-bit mode
#define PARITY					(0)     // [0] : Parity is disabled | [1] : Parity is enabled
#define BAUD_RATE               (38400) // Baud rate needed



/* This function initializes UART0 with sysclock.c configuration and enables
 * interrupts on it.
 *
 *  This code is inspired from,
 * 	https://github.com/alexander-g-dean/ESF/blob/master/NXP/Code/Chapter_8
 *
 * Parameters:
 * 		uint32_t baud_rate - The baud rate to which it should be configured to
 *
 * Returns:
 * 		None
 * */
void Init_UART0(uint32_t baud_rate);


/* overwriting the__sys_readc function which will help in using getchar(), when we
 * overwrite this function we will direct pgetchar to UART0.
 *  This code is inspired from,
 * 	https://github.com/alexander-g-dean/ESF/blob/master/NXP/Code/Chapter_8
 *
 * Parameters:
 * 			None
 *
 * Returns:
 *         int - ASCII code of character written/0 in case of error
 * */
int __sys_readc(void);


/* overwriting the sys_write function which will help in using printf, when we
 * overwrite this function we will direct printf to UART0.
 *
 *  This code is inspired from,
 * 	https://github.com/alexander-g-dean/ESF/blob/master/NXP/Code/Chapter_8
 *
 * Parameters:
 * 				 int handle -  stdout - (handle = 1)
 * 	  	  	  	  	  	  	   stderr - (handle = 2)
 * 	  	  	  	  	  	  	   In this case we are directing both to serial, so no need to
 * 	  	  	  	  	  	  	   take care of this
 *                char *buf  -  start address of buffer to be written
 *                int size   -  number of characters to be written
 *
 * Returns:
 * 		-1 in case of error
 * 	  	 0 in case of success
 * */
int __sys_write(int handle, char *buf, int size);



/* Interrupt handler for UART0, This function will enqueue and dequeue from
 * cbfifo on interrupt triggers.
 *
 *  This code is inspired from,
 * 	https://github.com/alexander-g-dean/ESF/blob/master/NXP/Code/Chapter_8
 *
 * Parameters:
 * 		None
 *
 * Returns:
 * 		None
 * */
void UART0_IRQHandler(void);

int my_nonblocking_getchar();



#endif //_UART_H_
