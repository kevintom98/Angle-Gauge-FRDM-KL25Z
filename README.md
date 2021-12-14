# PES-Final_Project
Code forPES-Final_Project, ECEN-5813,  Fall 2021
****************************************************************************************************************   
**Author**            - Kevin Tom  
**Development Board** - FRDM KL25Z (MKL25Z128VLK4)  
**Tools used**        - MCUXpresso IDE v11.2.0 [Build 4120] [2020-07-09]      
**Operating System**  - Windows 10 Home  
****************************************************************************************************************

## Important files in the build   
    1. hexdump.c - This file has the hexdump function
    2. cbfifo.c  - Cbfifo implementation    
    3. command_processor.c - This file has the function implementation of command processor
    4. UART.c -  This file handles the UART0 configuration, UART0 Interrupt handler and __sys_readc,__sys_write

 


## How to compile and run the file    
    1. Open MCUXpresso IDE and export the project 
    2. Press "Build" to build the code 
    3. Connect your FRDM KL25Z to your computer
    4. Press "Debug" to debug the code on FRDM KL25Z
    
    
    
## Terminal Parameters    
 ![Terminal_Parametrs](/Screenshots/Connection_Parameters.JPG)   
