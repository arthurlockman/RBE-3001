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

	setPinsDir('A', OUTPUT, 2, PORT3, PORT5);
	setPinsVal('A', LOW, 1, PORT3);
	setPinsVal('A', HIGH, 1, PORT5);

	initADC(7, ADC_FREE_RUNNING, ADC_REF_VCC);
	while (1)
	{
		printf("Reading... ");
	    long val = getADC(7);
		printf("%ld\n\r", val);
		_delay_ms(10);
	}

	return 0;
}
