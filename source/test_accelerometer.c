/*
 * test_accelerometer.c
 *
 *  Created on: 13-Dec-2021
 *      Author: Kevin Tom
 */

#include <stdio.h>

#include "Delay.h"
#include "Accelerometer.h"
#include "test_accelerometer.h"

void test_angle(float target)
{
	float tilt = 0;

	while(1)
	{
	read_full_xyz();

	tilt = (int)(convert_xyz_to_roll_pitch() * 100 + 0.5) ;

	tilt = (tilt / 100);

	printf("\n\rTilt : %f",tilt);

	if(target == tilt)
	{
		printf("\n\rMeasured %f",target);
		break;
	}

	}
}

void test_accelerometer()
{

	printf("\n\rManual testing of accelerometer");
	printf("\n\r********************************");

	printf("\n\rPlease place the sensor on a flat surface(The program will run until it is placed on flat surface)");
	test_angle(0.2);
	printf("\n\rDevice is on flat surface");
	Delay(1500);

	printf("\n\rTilt till 20 Degree is measured");
	Delay(1500);
	test_angle(20);
	printf("\n\rSuccess");
	Delay(1000);

	printf("\n\rTilt till 45 Degree is measured");
	Delay(1500);
	test_angle(45.0);
	printf("\n\rSuccess");
	Delay(1000);

	printf("\n\rTilt till 90 Degree is measured");
	Delay(1500);
	test_angle(90.0);
	printf("\n\rSuccess");
	Delay(1000);

	printf("\n\n\rAll tests passed!");
	Delay(1000);

}

