/*
 * main.c
 *
 *  Created on: Aug 6, 2014
 *      Author: ewillcox
 */
#include "RBELib/RBELib.h"
#include "RBELib/pot.h"

int main(void)
{
	debugUSARTInit(DEFAULT_BAUD);
	initRBELib();
	initADC(3, ADC_FREE_RUNNING, ADC_REF_VCC);
	initPot(DEFAULT_POT_CALIBRATION);

	while (1)
	{
		long adcReading = getADC(3);
		printf("Raw: %ld, Angle: %d, mV: %d\n\r", adcReading, potAngle(adcReading), potVolts(adcReading));
		_delay_ms(10);
	}

	return 0;
}
