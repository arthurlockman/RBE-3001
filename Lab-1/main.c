/*
 * main.c
 *
 *  Created on: Aug 6, 2014
 *      Author: ewillcox
 */
#include "RBELib/RBELib.h"

volatile char readADCFlag = 0;
volatile unsigned long ms = 0;
volatile unsigned long count = 0;
volatile unsigned tenthMS = 0;


ISR(TIMER0_OVF_vect)
{
	count++;
	tenthMS++;
	if (count == 9)
	{
		ms++;
		count = 1;
	}
	if(tenthMS == 40) // 10000 / hz;
	{
		readADCFlag = 1;	// Resets the flag every 4 ms - 225 Hz
		tenthMS = 0;
	}
}

void readADCValues()
{
	debugUSARTInit(DEFAULT_BAUD);
	initRBELib();
	sei();
	TCCR0A = 0b00000000;
	TCCR0B = 0b00000010;
	TIMSK0 = 0b00000001;
	initADC(7, ADC_FREE_RUNNING, ADC_REF_VCC);
	while (1)
	{
	    long val = getADC(7);
	    double potVal = (5000.0 * val) / 1024.0;
	    double potAngle = (300.0 * val) / 1024.0;
		printf("%ld, %ld, %f, %f\n\r", ms, val, potVal, potAngle);
		long start = ms;
		while(ms - start < 10)
			;
	}
}


void readADCValues2()
{
	debugUSARTInit(DEFAULT_BAUD);
	initRBELib();
	initADC(6, ADC_FREE_RUNNING, ADC_REF_VCC);

	setPinsDir('B', INPUT, 1, PORT4);				// Set B4 to input
	setPinsVal('B', 1, 1, PORT4);

	sei();											// Set global interrupt enable
	// Begin the timer.
	TCCR0A = 0b00000000;
	TCCR0B = 0b00000010;
	TIMSK0 = 0b00000001;

	long buff[225];
	int i = 0;
	while(1)
	{
		if(getPinsVal('B',1,PORT4) == 0b00010000)	// If the button is pressed
		{
			printf("Button has been pressed.\n\r");
			unsigned long start = ms;
			while(ms - start <= 1000)				// Scan inputs for 1 second
			{
				if(readADCFlag == 1)				// Read the ADC at 225 Hz
				{
					readADCFlag = 0;
					long val = getADC(6);
					// double potVal = (5000.0 * val) / 1023.0;
					// double potAngle = (300.0 * val) / 1023.0;
					// long timestamp = ms;
					// printf("%ld, %ld, %f, %f\n\r", timestamp, val, potVal, potAngle);
					buff[i] = val;
					i++;
				}
			}
			break;
		}
	}
	int p;
	for(p = 0; p < 225; p++)
	{
		printf("%ld,\t", buff[p]);
	}

}

void outputWave()
{
	debugUSARTInit(DEFAULT_BAUD);
	initRBELib();
	sei();
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
	readADCValues();
//	readADCValues2();
//	outputWave();
	return 0;
}

