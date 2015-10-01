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

typedef struct
{
	float x;
	float y;
	float time;

} pickupInfo;

ISR(TIMER0_OVF_vect)
{
	count++;
	if (count == 9)
	{
		ms++;
		count = 1;
		pidCount++;
	}
	if (pidCount == 9)
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
		// printf("%d, %d\n\r", upperLinkActual, lowerLinkActual);
	}
	_delay_ms(500);
	resetEncCount(0);
	resetEncCount(1);
}

pickupInfo calcPickupInfo()
{
	// Once we reach the threshhol (12 or 13) AND the value is stable (5-6 readings of the same value), log that time

	float deltaX = 16;
	int deltaT;
	float distanceToArmCenter = 3;

	long startTime = ms;

	int startIRVal_1 = IRDist(6);
	int startIRVal_2 = IRDist(7);

	printf("Start values: %d, %d\n\r", startIRVal_1, startIRVal_2);

	while(startIRVal_1 - IRDist(6) < 2)
		printf("Current IR1: %d\n\r", IRDist(6));
	long passTime1 = ms;

	while(startIRVal_2 - IRDist(7) < 2)
		printf("Current IR2: %d\n\r", IRDist(7));
	long passTime2 = ms;

	deltaT = (int)(passTime2 - passTime1);

	float velocity = deltaX / (float)deltaT;

	float timeToPickup = distanceToArmCenter/velocity;

	pickupInfo info;
	info.time = timeToPickup;

	return info;

}

int main(void)
{
	setup();
//	home();
//	setServo(0, 0);
//	pickupInfo info = calcPickupInfo();
//	printf("Time to interception: %f\n\r", info.time);
	while(1)
	{
		printf("IR Values: %d, %d\n\r", IRDist(6), IRDist(7));
	}

}
