/*
 * main.c
 *
 *  Created on: Aug 6, 2014
 *      Author: ewillcox
 */
#include "RBELib/RBELib.h"

unsigned long ms;
short count;
short pidCount;
volatile int upperLinkActual, lowerLinkActual;
volatile long pidOutputUpper, pidOutputLower;
volatile int x_accel, y_accel, z_accel;
int runPid = 1;
ISR(TIMER0_OVF_vect)
{
	count++;
	if (count == 9)
	{
		ms++;
		count = 1;
		pidCount++;
	}
	if (pidCount >= 9 && runPid)
	{
		pidCount = 1;
		upperLinkActual = potAngle(0);
		lowerLinkActual = potAngle(1);
		pidOutputUpper = calcPID('U', upperLinkActual);
		pidOutputLower = calcPID('L', lowerLinkActual);
		driveLink(2, pidOutputUpper);
		driveLink(1, pidOutputLower);
	}
}

void setup()
{
	debugUSARTInit(DEFAULT_BAUD);					// Setup serial
	initRBELib();									// Setup RBELib
	initSPI();										// Setup SPI/DAC
	initADC(3, ADC_FREE_RUNNING, ADC_REF_VCC);		// Setup ADC
	configureMsTimer();								// Setup ms timer and interrupts
	potCalibration calUpper = { 250, 625, 975 };	// Setup upper arm pot
	initPot(0, 3, calUpper);
	potCalibration calLower = { 255, 668, 1100 };	// Setup lower arm pot
	initPot(1, 2, calLower);
	setConst('U', 300.0, 0.0, 10.0); 				// Setup upper arm gains
	setConst('L', 300.0, 0.0, 10.0);				// Setup lower arm gains
	encInit(0);
	encInit(1);
}

void home()
{
	gotoAngles(0,0);

	while(!IN_RANGE(upperLinkActual,91,89)|| !IN_RANGE(lowerLinkActual,91,89))
	{
		printf("%d, %d\n\r", upperLinkActual, lowerLinkActual);
	}
	_delay_ms(500);
	resetEncCount(0);
	resetEncCount(1);
}

void collectEncoderData(void)
{
	unsigned long accMs = ms;
	unsigned long printMs = ms;
	setPinsDir('B', INPUT, 2, PORT0, PORT1, PORT2, PORT3);
	int speed = 0;
	runPid = 0;
	while(1)
	{
		if (getPinsVal('B', 1, PORT0))
		{
			speed = 0;
		} else if (getPinsVal('B', 1, PORT1))
		{
			speed = -805;
		} else if (getPinsVal('B', 1, PORT2))
		{
			speed = 805;
		} else if (getPinsVal('B', 1, PORT3))
		{
			speed = 1610;
		}
		driveLink(2, speed);
		driveLink(1, 0);
		if (ms - printMs >= 10)
		{
			printf("%ld\n\r", encCount(1));
			printMs = ms;
		}
	}
}
int main(void)
{
	setup();

	collectEncoderData();
	home();
	printf("Homed.\n\r");
	unsigned long accMs = ms;
	unsigned long printMs = ms;
	setPinsDir('B', INPUT, 2, PORT0, PORT1);
	int angle = 0;
	while(1)
	{
//		printf("Stuff!\n\r");
		if (ms - accMs > 1)
		{
			 x_accel = getAccel('x');
			 y_accel = getAccel('y');
			 z_accel = getAccel('z');
			accMs = ms;
		}
		if (getPinsVal('B', 1, PORT0))
		{
			angle = 0;
//			printf("Going to 0\n\r");
		} else if (getPinsVal('B', 1, PORT1))
		{
			angle = -90;
//			printf("Going to 90\n\r");
		}
		gotoAngles(angle, 0);
		if (ms - printMs >= 10)
		{
			printf("%d,%d,%ld,%ld,%f,%f,%f\n\r", lowerLinkActual - 90, upperLinkActual - 90,
					encCount(0), encCount(1), convertAccel(x_accel), convertAccel(y_accel),
					convertAccel(z_accel));
			printMs = ms;
		}
	}
}
