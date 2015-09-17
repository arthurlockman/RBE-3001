/*
 * motors.c
 *
 *  Created on: Sep 12, 2015
 *      Author: Tucker Haydon
 */

#include "RBELib/RBELib.h"


 void stopMotors()
 {
	 driveLink(1, 0);
	 driveLink(2, 0);
 }

void gotoAngles(int lowerTheta, int upperTheta)
{
	setSetpoint('U', upperTheta + 90);
	setSetpoint('L', lowerTheta + 90);
}

void gotoXY(int x, int y)
{
	float link1 = 6.0;
	float link2 = 9.0; //TODO: Update link lengths
	float x1 = sqrt(1 - (pow(x, 2) + pow(y, 2) - pow(link1, 2) - pow(link2, 2)) / (2 * link1 * link2));
	float x2 = (pow(x, 2) + pow(y, 2) - pow(link1, 2) - pow(link2, 2)) / (2 * link1 * link2);
	float t2pos = atan2(x1, x2);
	float t2neg = atan2(-x1, x2);
	float t1pos = atan2(link2 * sin(t2pos), link1 + link2 * cos(t2pos));
	float t1neg = atan2(link2 * sin(t2neg), link1 + link2 * cos(t2neg));
	if (IN_RANGE(t1pos, 90, -90) && IN_RANGE(t2pos, 90, -90))
	{
		gotoAngles(t1pos, t2pos);
	} else if (IN_RANGE(t1pos, 90, -90) && IN_RANGE(t2pos, 90, -90))
	{
		gotoAngles(t1neg, t2neg);
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
