/*
 * main.c
 *
 *  Created on: Aug 6, 2014
 *      Author: ewillcox
 */
#include "RBELib/RBELib.h"
#include "RBELib/ADC.h"

void readADCValues()
{
	debugUSARTInit(DEFAULT_BAUD);
	initRBELib();

	initADC(7, ADC_FREE_RUNNING, ADC_REF_VCC);
	while (1)
	{
	    long val = getADC(7);
		printf("%ld\n\r", val);
		_delay_ms(10);
	}
}

int main(void)
{
	readADCValues();
	return 0;
}

