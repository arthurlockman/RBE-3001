/*
 * pot.c
 *
 *  Created on: Sep 10, 2015
 *      Author: arthurlockman
 */

#include "RBELib/pot.h"

void initPot(potCalibration calib)
{
	calibration = calib;
	analogRange = calibration.DEG_180 - calibration.DEG_0;
}

int potAngle(long pot)
{
	return (int)(180.0 / analogRange * (pot - calibration.DEG_0));
}

int potVolts(long pot)
{
	return (pot / 1023.0) * 5000;
}
