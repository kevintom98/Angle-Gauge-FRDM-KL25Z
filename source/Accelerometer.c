/*
 * Accelerometer.c
 *
 *  Created on: 07-Dec-2021
 *      Author: Kevin Tom
 */

#include <MKL25Z4.H>
#include "Accelerometer.h"
#include "i2c.h"
#include <math.h>
#include "fsl_debug_console.h"
#include <stdio.h>
#include "Delay.h"

int16_t acc_X=0, acc_Y=0, acc_Z=0;
float roll=0.0, pitch=0.0;

//mma data ready
extern uint32_t DATA_READY;






/* This function initializes accelerometer
 *
 * Parameters: None
 *
 * Returns: int  - if initialization is successful or not
 *
 * */
int init_mma()
{
	//set active mode, 14 bit samples and 800 Hz ODR
	i2c_write_byte(MMA_ADDR, REG_CTRL1, 0x01);
	return 1;
}



/* This function reads all 3-axis of the accelerometer and
 * update global variables.
 *
 * Parameters: None
 *
 * Returns: None
 *  But this function updates global variables of acceleration in all 3-axis
 *
 * */
void read_full_xyz()
{
	int i;
	uint8_t data[6];
	int16_t temp[3];

	i2c_start();
	i2c_read_setup(MMA_ADDR , REG_XHI);

	// Read five bytes in repeated mode
	for( i=0; i<5; i++)
	{
		data[i] = i2c_repeated_read(0);
	}
	// Read last byte ending repeated mode
	data[i] = i2c_repeated_read(1);

	for ( i=0; i<3; i++ )
	{
		temp[i] = (int16_t) ((data[2*i]<<8) | data[2*i+1]);
	}

	// Align for 14 bits
	acc_X = temp[0]/4;
	acc_Y = temp[1]/4;
	acc_Z = temp[2]/4;
}



/* This function reads all 3-axis of the accelerometer and
 * update global variables.
 *
 * Parameters: None
 *
 * Returns: None
 *  But this function updates global variables of acceleration in all 3-axis
 *
 * */
void read_xyz(void)
{
	// sign extend byte to 16 bits - need to cast to signed since function
	// returns uint8_t which is unsigned
	acc_X = (int8_t) i2c_read_byte(MMA_ADDR, REG_XHI);
	Delay(100);
	acc_Y = (int8_t) i2c_read_byte(MMA_ADDR, REG_YHI);
	Delay(100);
	acc_Z = (int8_t) i2c_read_byte(MMA_ADDR, REG_ZHI);

}



float convert_xyz_to_roll_pitch()
{
	float ax = acc_X/COUNTS_PER_G,
				ay = acc_Y/COUNTS_PER_G,
				az = acc_Z/COUNTS_PER_G;

	//roll = atan2(ay, az)*180/M_PI;
	//pitch = atan2(ax, sqrt(ay*ay + az*az))*180/M_PI;


	float tilt_angle = 0,axx = ax * ax,ayy = ay*ay,azz = az*az;

	float acosvalue = (az)/(sqrt(axx+ayy+azz));

	tilt_angle = acos(acosvalue) * 180/M_PI;

	return tilt_angle;
}
