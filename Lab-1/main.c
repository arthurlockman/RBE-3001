/*
 * main.c
 *
 *  Created on: Aug 6, 2014
 *      Author: ewillcox
 */
#include "RBELib/RBELib.h"
#include "RBELib/ADC.h"

volatile unsigned int count = 0;
volatile unsigned long ms = 0;

ISR(TIMER0_OVF_vect)
{
	count++;
	if (count == 9)
	{
		ms++;
		count = 0;
	}
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

void outputWave()
{
	debugUSARTInit(DEFAULT_BAUD);
	initRBELib();
	//TODO: Move to function
	TCCR0A = 0b00000000;
	TCCR0B = 0b00000010;
	TIMSK0 = 0b00000001;

	unsigned long start;
	double duty;
	int period = 100;

    initADC(7, ADC_FREE_RUNNING, ADC_REF_VCC);

	setPinsDir('C', OUTPUT, 1, PORT6);
	setPinsDir('B', INPUT, 3, PORT0, PORT1, PORT2);
	while (1)
	{
		unsigned long potValue = getADC(7);
		duty = potValue / 1023.0;
		start = ms;
		while (ms - start < period * duty)
		{
			setPinsVal('C', HIGH, 1, PORT6);
		}

		start = ms;
		while (ms - start < period * (1 - duty))
		{
		    setPinsVal('C', LOW, 1, PORT6);
		}

		//B0, B1, B2
		if (getPinsVal('B',1,PORT0) == 0b00000001) {
			period = 1000;
		} else if (getPinsVal('B',1,PORT1) == 0b00000010) {
			period = 50;
		} else if (getPinsVal('B',1,PORT2) == 0b00000100) {
			period = 10;
		}
		double frequency = 1000.0 / period;
		printf("Duty Cycle: %f\tFrequency: %f\tState: \tPot Value: %ld\n\r", duty, frequency, potValue);
	}
}

int main(void)
{
	sei();
	outputWave();
	return 0;
}

