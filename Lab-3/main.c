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
ISR(TIMER0_OVF_vect)
{
	count++;
	if (count == 9)
	{
		ms++;
		count = 1;
		pidCount++;
		x_accel = getAccel('x');
		y_accel = getAccel('y');
		z_accel = getAccel('z');
	}
	if (pidCount == 9)
	{
		pidCount = 1;
		pidOutputUpper = calcPID('U', upperLinkActual);
		pidOutputLower = calcPID('L', lowerLinkActual);
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
	upperLinkActual = potAngle(0);
	lowerLinkActual = potAngle(1);

	while(!IN_RANGE(upperLinkActual,91,89)|| !IN_RANGE(lowerLinkActual,91,89))
	{
		upperLinkActual = potAngle(0);
		lowerLinkActual = potAngle(1);

		driveLink(2, pidOutputUpper);
		driveLink(1, pidOutputLower);

		printf("%d, %d\n\r", upperLinkActual, lowerLinkActual);
	}
	stopMotors();
	_delay_ms(500);
	resetEncCount(0);
	resetEncCount(1);
}

int main(void)
{
	setup();

//	DDRBbits._P4 = OUTPUT;
//	DDRBbits._P4 = OUTPUT;
//	while (1)
//	{
//		PINBbits._P4 = 0;
//		_delay_ms(100);
//		PINBbits._P4 = 1;
//		_delay_ms(100);
//	}

//	home();
//	printf("Homed.\n\r");
	while(1)
	{
//		long count0 = encCount(0);
//		long count1 = encCount(1);
		printf("%d, %d, %d\n\r", x_accel, y_accel, z_accel);
	}



}
