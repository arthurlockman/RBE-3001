/*
 * main.c
 *
 *  Created on: Aug 6, 2014
 *      Author: ewillcox
 */
#include "RBELib/RBELib.h"
#include "RBELib/ADC.h"

int main(void)
{
	debugUSARTInit(DEFAULT_BAUD);
	initRBELib();
	setPinsDir('A', INPUT, 1, PORT7);
	initADC(7, ADC_FREE_RUNNING, ADC_REF_VCC);
	while (1)
	{
		printf("Reading... ");
		long val = getADC(7);
		printf("Value: %ld\n\r", val);
		_delay_ms(1000);
	}
	return 0;
}
