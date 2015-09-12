/*
 * pot.c
 *
 *  Created on: Sep 10, 2015
 *      Author: arthurlockman
 */

#include "RBELib/pot.h"

void initPot(int pot, int channel, potCalibration calib)
{
	pots[pot].adcChannel = channel;
	pots[pot].min = calib.DEG_0;
	pots[pot].scaler = 180.0 / (calib.DEG_180 - calib.DEG_0);
}

int potAngle(int pot)
{
	int value = getADC(pots[pot].adcChannel);
	return (int)(pots[pot].scaler * (value - pots[pot].min));
}

int potVolts(int pot)
{
	int value = getADC(pots[pot].adcChannel);
	return (value / 1023.0) * 5000;
}
