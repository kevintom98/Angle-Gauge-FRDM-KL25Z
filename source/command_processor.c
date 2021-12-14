
/*
 * command_processor.c
 *
 *  Created on: 08-Nov-2021
 *      Author: Kevin Tom, Kevin.Tom@colorado.edu
 *
 *
 *  This file has the function implementation of command processor
 */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#include "command_processor.h"
#include "Accelerometer.h"
#include "UART.h"
#include "PWM.h"
#include "state_machine.h"
#include "Delay.h"

static float calibration_value = 0;

/* This function is the handler for author command.
 * This function prints the authors name
 *
 * Parameters:
 * None
 *
 * Returns:
 * None
 *
 *
 * */
void author_handler()
{
	//Printing author name
	printf("\n\rKevin Tom\n\r");
}





/*
 * This function is the interrupt handler for hardware button press.
 *
 * This function sets a global variable and clears interrupt after it
 * is encountered.
 *
 * That is why it is kept in this file, this file updates variable inside
 * this file
 *
 * Parameters : None
 *
 * Returns : None
 *
 *
 * */
void PORTD_IRQHandler(void)
{
	/*Checking if ISFR  triggered*/
	if( (PORTD->ISFR & (1<<3)) == 0)
		return;

	read_full_xyz();
	calibration_value = convert_xyz_to_tilt();
	printf("\n\rCalibration value set to : %f\n\r",calibration_value);
	printf("# ");

	/*Clears the Interrupt*/
	PORTD->ISFR &= (1 << 3);
}







/* This function is the handler for help command
 * it prints the help menu
 *
 * Parameters:
 * 	argc - Number of arguments
 * 	argv - Array of arguments ending with '\0'
 *
 * Returns:
 * 	None
 * */
void help_handler()
{
	for (int i=0; i < num_commands; i++)
	  {
		//Prints until help
	    if (strcasecmp("help", commands[i].name) != 0)
	    {
	    	//Printing the string
	    	for(const char *j=commands[i].help_string; *j != '\0'; j++)
	    		printf("%c",*j);
	    }
	  }
}




/* This function is the handler for measure command.
 * This function runs a while loop and exists it when
 * angle is reached
 *
 * Parameters:
 * None
 *
 * Returns:
 * None
 *
 * */
void measure_handler(int argc, char *argv[])
{
	float target = 0,tilt=0, percentage = 0, calibration = 0;

	//Checking if input is valid
	if((target > 180) || (target < 0))
	{
		printf("\n\rInvalid target angle");
		return;
	}

	//Converting value to 2 point precision
	target = (int)(atof(argv[1]) * 100 + 0.5);
	target = target / 100;

	printf("\n\rTarget angle %f",target);


	while(1)
	{
		read_full_xyz();

		//Setting the calibrated value
		calibration = convert_xyz_to_tilt() - calibration_value;

		if(calibration < 0)
			calibration = fabs(calibration);

		//Converting value to 2 point precision
		tilt = (int)((calibration) * 100 + 0.5) ;
		tilt = (tilt / 100);

		//Calculating percentage for PWM
		percentage = tilt/target;

		//PWM transition
		transition(0,percentage * 255, 0);

		printf("\n\rTilt : %f",tilt);



		//If target angle is reached blink LED and print result
		if(tilt == target)
		{
			printf("\n\rReached target angle %f\n\r",target);

			//Angle reached blinking pattern
			tpm_function(0,0,255);
			Delay(200);
			tpm_function(0,0,0);
			Delay(200);
			tpm_function(0,0,255);
			Delay(200);
			tpm_function(0,0,0);

			break;
		}
	}
}





/* This function is the handler for set0 command.
 * Thi command sets the current angle as 0
 *
 * Parameters:
 * None
 *
 * Returns:
 * None
 *
 * */
void set0_handler()
{
	//Measuring current value
	read_full_xyz();
	calibration_value = convert_xyz_to_tilt();
	printf("\n\rCalibration value set to : %f\n\r",calibration_value);
}



/* This function is the handler for test command
 * Runs tests on the device
 *
 * Parameters:
 * None
 *
 * Returns:
 * None
 *
 * */
void test_handler()
{
	printf("\n\n\r\rRunning tests\n\n\r");
	test_system();
}







/* This function starts the command processor and handles the commands recevied
 *
 * Parameters:
 * 	None
 *
 * Returns:
 * None
 *
 *
 * */
void command_processor_start()
{
	char command[100];
	int i=-1;
	printf("\rDigital Angle Gauge\n\r");
	printf("**********************\n\r");
	printf("Commands\n\r");
	printf("--------\n\r");
	printf("author  - This command will print the name of the author who wrote the command line\n\r");
	printf("dump    - This command will print Hexdump of memory(eg: dump start_addr end_addr ; dump 0 0x64)\n\r");
	printf("measure - This command will start measuring the angle\n\r");
	printf("set0    - This command will set the current angle as 0\n\r");
	printf("help    - prints the above anytime\n\r");
	printf("-----------------------------------------------------------------------------------------------\n\r");
	int a;

	while(1)
	{
		printf("# ");
		i= -1;


		/*******************Accumulator**********************/
		while(command[i] != '\r')
		{
			i++;
			//Getting character
			a = getchar();
			if(a==0)
				break;

			command[i]= a;

			//Handling backspace
			if((command[i] == '\b') && (i > 1))
			{
				command[i] =' ';
				printf(" \b \b");
			}
			//else if(commsnd[i] == )

		}
		command[i++] = '\0';
		/****************************************************/


		//Calling process command function
		process_command(command);
	}


}





/* This function splits the received command into
 * argc and argv vectors and calls appropriate handling functions
 *
 * Parameters:
 * 	char *input - Input string from accumulator
 *
 * Returns:
 * 	None
 * */
void process_command(char *input)
{
  char *p = input;
  char *end;


  // find end of string
  for (end = input; *end != '\0'; end++)
    ;


  //Bool for printing error message
  bool found = false;
  char *argv[10];
  int argc = 0;


  //Setting argv array to '0'
  __builtin_memset(argv, 0, sizeof(argv));


  for (p = input; p < end; p++)
  {
	  //If a character is recognized
	  if((*p >= 48))
	  {
		  //if previous character is ' ' or '\0' or it is starting character
		  if( (*(p-1) == ' ') || (p == input) || (*(p-1) == '\0'))
		  {
			  //Write the address to argv[argc]
			  argv[argc] = p;
			  //Incrementing argc
			  argc++;
		  }
		  //If trailing character is space make it as '\0'
		  if(*(p+1) == ' ')
			  	*(p+1) = '\0';
	  }
  }


  //If no command received
  argv[argc] = NULL;
  if (argc == 0)   // no command
    return;


  //Checking which handler to call using argv[0] string
  for (int i=0; i < num_commands; i++)
  {
    if (strcasecmp(argv[0], commands[i].name) == 0)
    {
      commands[i].handler(argc, argv);
      found = true;
      break;
    }
  }


  //If no handler is found print error message
  if(found == false)
  {
	  printf("\n\rUnknown Command: ");
	  for(char *i= argv[0];*i != '\0' ;i++)
		  printf("%c",*i);
	  printf("\n\r");
  }


}
