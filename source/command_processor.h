/*
 * command_processor.h
 *
 *  Created on: 04-Nov-2021
 *      Author: Kevin Tom, Kevin.Tom@colorado.edu
 */
#ifndef _COMMAND_PROCESSOR_H_
#define _COMMAND_PROCESSOR_H_


//Function pointer for each function
typedef void (*command_handler_t)(int, char *argv[]);



//Structure which holds all the handler details
typedef struct
{
  const char *name;
  command_handler_t handler;
  const char *help_string;
} command_table_t;




/* This function is the handler for author command.
 * This function prints the authors name
 *
 * Parameters:
 * None
 *
 * Returns:
 * None
 * */
void author_handler();








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
void help_handler();



void measure_handler();


void set0();



/********************************************************COMMAND TABLE********************************************************************/
static const command_table_t commands[] =
{
		{"author", author_handler,"\n\rauthor  - This command will print the name of the author who wrote the command line\n\r"},
		{"measure",measure_handler,"measure - This command will start measuring the angle\n\r"},
		{"set0",set0,"set0    - This command will set the current angle as 0\n\r"},
		{"help", help_handler," "}
};
/*****************************************************************************************************************************************/



//Calculating number of commands
static const int num_commands = sizeof(commands) / sizeof(command_table_t);



/* This function starts the command processor and handles the commands recevied
 *
 * Parameters:
 * 	None
 *
 * Returns:
 * None
 * */
void command_processor_start();



/* This function splits the received command into
 * argc and argv vectors and calls appropriate handling functions
 *
 * Parameters:
 * 	char *input - Input string from accumualtor
 *
 * Returns:
 * 	None
 *
 *
 * */
void process_command(char *input);



#endif // _COMMAND_PROCESSOR_H_

