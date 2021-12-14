/*
 * Accelerometer.h
 *
 *  Created on: 08-Dec-2021
 *      Author: Kevin Tom
 */

#ifndef ACCELEROMETER_H_
#define ACCELEROMETER_H_

#include <stdint.h>

/*MACROS for Interfacing*/
#define MMA_ADDR 0x3A

#define REG_XHI 0x01
#define REG_XLO 0x02
#define REG_YHI 0x03
#define REG_YLO 0x04
#define REG_ZHI	0x05
#define REG_ZLO 0x06

#define REG_WHOAMI 0x0D
#define REG_CTRL1  0x2A
#define REG_CTRL4  0x2D

#define WHOAMI 0x1A

#define COUNTS_PER_G (4096.0)
#define M_PI (3.14159265)


/* This function initializes accelerometer
 *
 * Parameters: None
 *
 * Returns: int  - if initialization is successful or not
 *
 * */
int init_mma(void);

/* This function reads all 3-axis of the accelerometer and
 * update global variables.
 *
 * Parameters: None
 *
 * Returns: None
 *  But this function updates global variables of acceleration in all 3-axis
 *
 * */
void read_full_xyz(void);

/* This function reads all 3-axis of the accelerometer and
 * update global variables.
 *
 * Parameters: None
 *
 * Returns: None
 *  But this function updates global variables of acceleration in all 3-axis
 *
 * */
void read_xyz(void);

/* This function converts the readings to tilt value.
 *
 * Parameters: None
 *
 * Returns: float - Calculated tilt value
 *
 * */
float convert_xyz_to_tilt();


//These are used by functions outside of this file
extern int16_t acc_X, acc_Y, acc_Z;


#endif /* ACCELEROMETER_H_ */
