# PES-Final_Project
Code forPES-Final_Project, ECEN-5813,  Fall 2021
****************************************************************************************************************   
**Author**            - Kevin Tom  
**Development Board** - FRDM KL25Z (MKL25Z128VLK4)  
**Tools used**        - MCUXpresso IDE v11.2.0 [Build 4120] [2020-07-09]      
**Operating System**  - Windows 10 Home  
****************************************************************************************************************

## Demo Video Link : https://drive.google.com/file/d/1oIr_fNSRlbOBn8hfRs_yd6o9VZI9CJ8w/view?usp=sharing

## How to compile and run the file    
    1. Open MCUXpresso IDE and export the project 
    2. Press "Build" to build the code 
    3. Connect your FRDM KL25Z to your computer
    4. Press "Debug" to debug the code on FRDM KL25Z
    
# User Guide

## Initial setting up steps
    1. When starting the program intially device will be in test mode and will be testing peripherals.  
    You will be able to see RED,GREEN,BLUE,LED blinking in same order wtih 1sec interval.
    2. After blinking of LEDs, the device must be kept on a flat surface for calibration.
    3. After flat surface is detected, the user have to tilt  the sensor to 20 degrees to test the working.
    4. When 20 degree is detected, the user have to tilt to 45 degeress for testing.
    5. After 45 degree is detected the user has to tilt the sensor to 90 degeress.
    6. If all these tests are passed "All test passed" message will be printed on the GUI.
    7. After this the command processor will be displayed with list of commands in the processor.
    
## To set an angle to zero
    1. To set a specific angle to zero, the device should be kept in that angle.
    2. The user have to type 'set0'command on the command line for setting the current orientation as zero.
    3. Otherwise the user can press the switch on the device to set the current orientation as zero.
    4. All the measurements will be made with respect to this orientation after setting zero.
    
## To measure an angle
    1. To measure an angle , the user have to type "measure [space] [angle]" in the command processor.
    2. After this command is send, the user can see real time angle feedback depending on device orientation on the screen.
    3. The LED on the device will also give visual feedback (Will be bright green when it is near target angle)
    4. When the target angle is reached the GUI will show that angle have reached and LED will blink in Blue color twice.
    
## Test the device at any time
    1. To test the device at anytime the user can type "test" command on the processor.
    2. This will take the device to test mode and will be running tests similar to inital setting up steps section
    
    
## Terminal Parameters    
 ![Terminal_Parametrs](/Screenshots/Connection_Parameters.JPG)   
  
  
## CMD GUI
  ![CMD_GUI](/Screenshots/CMD_GUI.JPG)
  
  
  
