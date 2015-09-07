/*
 * main.c
 *
 *  Created on: Aug 6, 2014
 *      Author: ewillcox
 */
#include "RBELib/RBELib.h"
#include "RBELib/ADC.h"

volatile unsigned long count = 0;

ISR(TIMER1_COMPA_vect)
{
	count++;
}

void readADCValues()
{
	debugUSARTInit(DEFAULT_BAUD);
	initRBELib();

	initADC(7, ADC_FREE_RUNNING, ADC_REF_VCC);
	int counter = 0;
	while (1)
	{
	    long val = getADC(7);
	    double potVal = (5000.0 * val) / 1024.0;
	    double potAngle = (300.0 * val) / 1024.0;
	    double timestamp = ((double)counter * 10);
		printf("%f, %ld, %f, %f\n\r", timestamp, val, potVal, potAngle);
		_delay_ms(10);
		counter++;
	}
}

int main(void)
{
	sei();
	initTimer(1, CTC, 0xFF);
	readADCValues();
	return 0;
}

