/*
 * motors.c
 *
 *  Created on: Sep 12, 2015
 *      Author: Tucker Haydon
 */

#include "RBELib/RBELib.h"

const float k_degToRad = 0.01745329251;
const float k_radToDeg = 57.2957795;

 void stopMotors()
 {
	 driveLink(1, 0);
	 driveLink(2, 0);
 }

void gotoAngles(int lowerTheta, int upperTheta)
{
	setSetpoint('U', upperTheta + 90);
	setSetpoint('L', lowerTheta + 90);
//	 printf("Lower: %d, Upper: %d, diff: %d\n\r", lowerTheta, upperTheta, lowerTheta - upperTheta);
}

void gotoXY(int x, int y)
{
	float link1 = 152.4; // mm
	float link2 = 111.76; // mm
	float x1 = sqrt(1 - pow((pow(x, 2) + pow(y, 2) - pow(link1, 2) - pow(link2, 2)) / (2 * link1 * link2), 2));
	float x2 = (pow(x, 2) + pow(y, 2) - pow(link1, 2) - pow(link2, 2)) / (2 * link1 * link2);
	float t2pos = atan2(x1, x2);
	float t2neg = atan2(-x1, x2);
	float t1pos = atan2((float)y, (float)x) - atan2(link2 * sin(t2pos), link1 + link2 * cos(t2pos));
	float t1neg = atan2((float)y, (float)x) - atan2(link2 * sin(t2neg), link1 + link2 * cos(t2neg));

	// MOTHER FUCKING RADIANS. CONVERTING TO DEGREES.
	float theta1pos = t1pos * k_radToDeg;
	float theta2pos = t2pos * k_radToDeg;
	float theta1neg = t1neg * k_radToDeg;
	float theta2neg = t2neg * k_radToDeg;

	// At the point, we must determine which of the 4 angles are valid.
	// Once we determine which of them are valid, we must contruct pairs of angles.
	// Once we have the pairs of angles, we must determine if the pairs of angles are valid.
	// If at this point we have 2 valid pairs of angles, we must determine which pair to choose.

	if (IN_RANGE(theta1pos, 90, -90) && IN_RANGE(theta2pos, 90, -90) && IN_RANGE(theta1neg, 90, -90) && IN_RANGE(theta2neg, 90, -90))
	{
		//TODO: Decide which to go to if both are valid
		if (y <= 0)
		{
			if (theta1pos - theta2pos >= 0)
				gotoAngles(theta1pos, theta2pos);
			else
				gotoAngles(theta1neg, theta2neg);
		} else
		{
			if (theta1pos - theta2pos <= 0)
				gotoAngles(theta1pos, theta2pos);
			else
				gotoAngles(theta1neg, theta2neg);
		}
	} else if (IN_RANGE(theta1pos, 90, -90) && IN_RANGE(theta2pos, 90, -90))
	{
		gotoAngles(theta1pos, theta2pos);
	} else if (IN_RANGE(theta1neg, 90, -90) && IN_RANGE(theta2neg, 90, -90))
	{
		gotoAngles(theta1neg, theta2neg);
	}
	else
	{
		// printf("Not in range. \n\r");
	}
}

// TODO Implement different links
void driveLink(int link, long dir)
{
	if (dir > 4095)
		dir = 4095;
	if (dir < -4095)
		dir = -4095;
	switch(link)
	{
	case 1:
		if (dir >= 0)
		{
			setDAC(0, dir);
			setDAC(1, 0);
		} else
		{
			setDAC(0, 0);
			setDAC(1, -dir);
		}
		break;
	case 2:
		if (dir >= 0)
		{
			setDAC(3, dir);
			setDAC(2, 0);
		} else
		{
			setDAC(3, 0);
			setDAC(2, -dir);
		}
		break;
	}
}

int readCurrentMilliamps(int link)
{
	int value;
	switch(link)
	{
	case 1: //read link 1
		value = getADC(0);
		break;
	case 2: //read link 2
		value = getADC(1);
		break;
	}
	return -(int)(((value - 542) / 1024.0) * 5000.0);
}

// void homePos();
