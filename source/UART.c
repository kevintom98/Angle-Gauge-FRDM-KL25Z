/*
 *  File name : UART.c
 *
 *  Created on: 04-Nov-2021
 *      Author: Kevin Tom
 *
 * Description
 * -----------
 * This file handles the UART0 configuration, UART0 Interrupt handler and __sys_readc,__sys_write
 *
 *
 */


#include <stdio.h>

#include "UART.h"
#include "cbfifo.h"



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
void Init_UART0(uint32_t baud_rate)
{
	uint16_t sbr;
	uint8_t temp;

	// Enable clock gating for UART0 and Port A
	SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;  //0100 0000 0000 11th bit
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;  //UART0 is in PORTA -> PTA1,PTA2 , Alternate Function 1

	// Make sure transmitter and receiver are disabled before init
	UART0->C2 &= ~UART0_C2_TE_MASK & ~UART0_C2_RE_MASK; // Masking Transmit Enable and Receive Enable

	// Set UART clock to 48 MHz clock
	SIM->SOPT2 |= SIM_SOPT2_UART0SRC(1); //MCGFLLCLK or MCGPLLCLK/2 (We selected FLL clock in sysclock)


	// Set pins to UART0 Rx and Tx
	//Interrupt status flag (ISF) on  Pin-1 , interrupt is detected, Alternate 2 (UART)
	PORTA->PCR[1] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(2); // Rx
	 //Interrupt status flag (ISF) on  Pin-2 , interrupt is detected, Alternate 2 (UART)
	PORTA->PCR[2] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(2); // Tx

	// Set baud rate and oversampling ratio
	sbr = (uint16_t)((SYS_CLOCK)/(baud_rate * UART_OVERSAMPLE_RATE));
	UART0->BDH &= ~UART0_BDH_SBR_MASK; //Clearing 4 bit BDH
	UART0->BDH |= UART0_BDH_SBR(sbr>>8); //Putting the 9th bit in BDH, since other 3 bits are 0
	UART0->BDL = UART0_BDL_SBR(sbr); //Putting rest of 8bits in BDL
	UART0->C4 |= UART0_C4_OSR(UART_OVERSAMPLE_RATE-1); //Over-sampling 15

	// Disable interrupts for RX active edge and LIN break detect, select one stop bit
	UART0->BDH |= UART0_BDH_RXEDGIE(0) | UART0_BDH_SBNS(STOP_BITS) | UART0_BDH_LBKDIE(0);

	// Don't enable loopback mode, use 8 data bit mode, don't use parity
	UART0->C1 = UART0_C1_LOOPS(0) | UART0_C1_M(DATA_SIZE) | UART0_C1_PE(PARITY);
	// Don't invert transmit data, don't enable interrupts for errors
	UART0->C3 = UART0_C3_TXINV(0) | UART0_C3_ORIE(0)| UART0_C3_NEIE(0)
			| UART0_C3_FEIE(0) | UART0_C3_PEIE(0);

	// Clear error flags
	UART0->S1 = UART0_S1_OR(1) | UART0_S1_NF(1) | UART0_S1_FE(1) | UART0_S1_PF(1);

	// Send LSB first, do not invert received data
	UART0->S2 = UART0_S2_MSBF(0) | UART0_S2_RXINV(0);


	/******************Interrupts Configuration part*********************/
	NVIC_SetPriority(UART0_IRQn, 2); // 0, 1, 2, or 3
	NVIC_ClearPendingIRQ(UART0_IRQn);
	NVIC_EnableIRQ(UART0_IRQn);

	// Enable receive interrupts but not transmit interrupts yet
	UART0->C2 |= UART_C2_RIE(1);
	/********************************************************************/

	// Enable UART receiver and transmitter
	UART0->C2 |= UART0_C2_RE(1) | UART0_C2_TE(1);

	// Clear the UART RDRF flag
	temp = UART0->D;
	UART0->S1 &= ~UART0_S1_RDRF_MASK;

}





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
void UART0_IRQHandler(void)
{
	uint8_t ch;


	if (UART0->S1 & (UART_S1_OR_MASK |UART_S1_NF_MASK |
		UART_S1_FE_MASK | UART_S1_PF_MASK))
	{
			// clear the error flags
			UART0->S1 |= UART0_S1_OR_MASK | UART0_S1_NF_MASK |
									UART0_S1_FE_MASK | UART0_S1_PF_MASK;

			// read the data register to clear RDRF
			ch = UART0->D;
	}

	if (UART0->S1 & UART0_S1_RDRF_MASK) //RDRF is '1': Receive data buffer is full
	{
		ch = UART0->D; //Reading the received character

		UART0->D = ch; //Echoing back the character

		if(cbfifo_enqueue(&ch,1,RX_Buffer) == 1)
		{
			;
		}
		else
		{
			//If not discard
		}
	}

	if ( (UART0->C2 & UART0_C2_TIE_MASK) &&    // transmitter interrupt enabled
			(UART0->S1 & UART0_S1_TDRE_MASK) ) //If transmit data buffer is empty
	{
		// checking if tx buffer empty
		if(cbfifo_length(TX_Buffer) > 0)
		{
			cbfifo_dequeue(&ch, 1, TX_Buffer);

			UART0->D = ch; //Put the data across UART line
		}
		else
		{
			// queue is empty so disable transmitter interrupt
			UART0->C2 &= ~UART0_C2_TIE_MASK;
		}
	}
}



/* overwriting the sys_write function which will help in using printf, when we
 * overwrite this function we will direct printf to UART0.
 *
 *  This code is inspired from,
 * 	https://github.com/alexander-g-dean/ESF/blob/master/NXP/Code/Chapter_8
 *
 * Parameters:
 * 				  int handle -  stdout - (handle = 1)
 * 	  	  	  	  	  	  	    stderr - (handle = 2)
 * 	  	  	  	  	  	  	    In this case we are directing both to serial, so no need to
 * 	  	  	  	  	  	  	    take care of this
 *                char *buf  -  start address of buffer to be written
 *                int size   -  number of characters to be written
 *
 * Returns:
 * 		-1 in case of error
 * 	  	 0 in case of success
 * */
int __sys_write(int handle, char *buf, int size)
{
	//In case of error return -1
	if(buf == NULL || size <= 0)
		return -1;


	//Checking if buffer is full
	while( size > (MAX_SIZE - cbfifo_length(TX_Buffer)) );


	//Enqueue to transmit buffer
	if(cbfifo_enqueue(buf,size,TX_Buffer) != size)
		return -1;


	//Generating a transmit signal
	//start transmitter if it isn't already running
	if (!(UART0->C2 & UART0_C2_TIE_MASK))
	{
		UART0->C2 |= UART0_C2_TIE(1);
	}


	//If success return 0
	return 0;
}








/* overwriting the__sys_readc function which will help in using getchar(), when we
 * overwrite this function we will direct pgetchar to UART0.
 *  This code is inspired from,
 * 	https://github.com/alexander-g-dean/ESF/blob/master/NXP/Code/Chapter_8
 *
 * Parameters:
 * 			None
 *
 * Returns:
 *         int - ASCII code of character written/ 0 in case of error
 * */
int __sys_readc(void)
{
	char c;

	//Wait till something is written into RX_Buffer, i.e, wait till use writes something
	while((cbfifo_length(RX_Buffer)==0));

	//After RX_Buffer has value dequeue it and return
	if(cbfifo_dequeue(&c, 1, RX_Buffer) == 1)
		return c;
	else
		return 0;
}



/*int my_nonblocking_getchar()
{
	char c;

	//Wait till something is written into RX_Buffer, i.e, wait till use writes something
	if((cbfifo_length(RX_Buffer)==0))
		return 0;

	//After RX_Buffer has value dequeue it and return
	if(cbfifo_dequeue(&c, 1, RX_Buffer) == 1)
		return c;
	else
		return -1;
}*/





