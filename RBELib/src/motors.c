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

// void gotoAngles(int lowerTheta, int upperTheta);

// void gotoXY(int x, int y);

// TODO Implement different links
void driveLink(int link, int dir)
{
	switch(link)
	{
	case 1:
		if (dir >= 0)
		{
			setDAC(1, dir);
			setDAC(0, 0);
		} else
		{
			setDAC(1, 0);
			setDAC(0, -dir);
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
