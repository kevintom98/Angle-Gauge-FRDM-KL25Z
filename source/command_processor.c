
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
#include "hexdump.h"
#include "Accelerometer.h"
#include "UART.h"



static float calibration_value = 0;

/*This structure holds all the necessary variables for state machine*/
static struct
{
	double led_red;
	double led_green;
	double led_blue;
}current =
{
	.led_red = 255,
	.led_green = 00,
	.led_blue = 00
};



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
void tpm_function(int red, int green, int blue)
{
	TPM2->CONTROLS[0].CnV = (red  <<8);   //Red
	TPM2->CONTROLS[1].CnV = (green<<8); //Green
	TPM0->CONTROLS[1].CnV = (blue <<8);  //Blue
}


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
void transition_to_any_state(int RED,int GREEN, int BLUE)
{
	current.led_red   = ((RED -current.led_red)   *transition_factor)+current.led_red;
	current.led_green = ((GREEN-current.led_green)*transition_factor)+current.led_green;
	current.led_blue  = ((BLUE -current.led_blue) *transition_factor)+current.led_blue;

	tpm_function(current.led_red,current.led_green,current.led_blue);
}





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







/* This function is the handler for author command.
 * This function prints the authors name
 *
 * Parameters:
 * 	argc - Number of arguments
 * 	argv - Array of arguments ending with '\0'
 *
 * Returns:
 * None
 *
 *
 * */
void dump_handler(int argc, char *argv[])
{

	int start = 0, len = 0;

	//Converting start address to from hex to decimal
	start = (int)strtol(argv[1],NULL,16);

	/*If given address is in hex convert it into decimal
	 * Else convert it into decimal from string
	 */
	if((*(argv[2]) == '0') && (*(argv[2]+1) == 'x'))
		len = (int)strtol(argv[2],NULL,16);
	else
		len = atoi(argv[2]);


	//Calling the hexdump function
	hexdump((int *)start,len);
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




void measure_handler(int argc, char *argv[])
{
	float target = atof(argv[1]),tilt=0;

	target = (int)(atof(argv[1]) * 100 + 0.5);

	target = target / 100;

	printf("\n\rTarget angle %f",target);

	Delay(500);

	while(1)
	{
		read_full_xyz();

		tilt = (int)((convert_xyz_to_roll_pitch() - calibration_value) * 100 + 0.5) ;

		tilt = (tilt / 100);

		//tilt = tilt - calibration_value;

		printf("\n\rTilt : %f",tilt);

		if(tilt == target)
		{
			printf("\n\rReached target angle %f\n\r",target);
			break;
		}
	}
}



void set0()
{
	read_full_xyz();
	calibration_value = convert_xyz_to_roll_pitch();
	printf("\n\rCalibration value set to : %f\n\r",calibration_value);
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
