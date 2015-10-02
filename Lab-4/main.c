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

int normalVariance(int* readings, int numReadings, float average)
{
	float variance = 0;
	for(int i = 0; i < numReadings; i++)
	{
		variance += pow(readings[i] - average, 2);
	}
	variance /= numReadings;

	float stdDev = sqrt(variance);

	if(stdDev <= 1) return 1;
	else return 0;


}

int stableValue(int channel)
{
	int numReadings = 7;
	int readings[numReadings];
	float sum = 0;
	for(int i = 0; i < numReadings; i++)
	{
		readings[i] = IRDist(channel);
		sum += readings[i];
		_delay_ms(1);
	}

	float average = sum / numReadings;

	if((abs(readings[0] - readings[numReadings - 1]) <= 2) && normalVariance(readings, numReadings, average))
	{
		return (int)average;
	}
	else
	{
		return -1;
	}
}

int average(int channel)
{
	int numReadings = 7;
	int readings[numReadings];
	float sum = 0;
	for(int i = 0; i < numReadings; i++)
	{
		readings[i] = IRDist(channel);
		sum += readings[i];
		_delay_ms(1);
	}

	float average = sum / numReadings;
	return (int)(average);
}

pickupInfo calcPickupInfo()
{
	// Once we reach the threshhold (12 or 13) AND the value is stable (5-6 readings of the same value), log that time

	float deltaX = 16;
	int deltaT;
	float distanceToArmCenter = 3;

	int startIRVal_1 = IRDist(6);
	int startIRVal_2 = IRDist(7);

	while(1)
	{
		startIRVal_1 = stableValue(6);
		if(startIRVal_1 != -1 &&  average(6) < 12)
		{
			break;
		}
	}
	long passTime1 = ms;

	while(1)
	{
		startIRVal_2 = stableValue(7);
		if(startIRVal_2 != -1 &&  average(7) < 12)
		{
			break;
		}
	}
	long passTime2 = ms;

	printf("Start values: %d, %d\n\r", startIRVal_1, startIRVal_2);

	deltaT = (int)(passTime2 - passTime1);
	printf("Delta time: %d\n\r", deltaT);

	float velocity = deltaX / (float)deltaT;
	printf("Velocity: %f\n\r", velocity);

	float timeToPickup = distanceToArmCenter/velocity;

	pickupInfo info;
	info.time = timeToPickup;
	info.x = 0.0;
	info.y = - 200 - ((startIRVal_1 + startIRVal_2) / 2.0 - 6) *10;
	return info;

}

int main(void)
{
	setup();
	home();
	openGripper(1);
	gotoXY(60, -200);
	setServo(0, 0);
	pickupInfo info = calcPickupInfo();
	_delay_ms((int)info.time);
	printf("%d, %d", (int)info.x, (int)info.y);
	gotoXY(0, (int)info.y);
	while (!IN_RANGE(upperLinkActual, upperLinkSetpoint - 1, upperLinkSetpoint + 1) &&
			!IN_RANGE(lowerLinkActual, lowerLinkSetpoint - 1, lowerLinkSetpoint + 1))
		;
	closeGripper(1);
	_delay_ms(1000);
	gotoXY(60, -200);
//	while(1)
//	{
//		int pos[2];
//		calcXY(pos);
//		printf("%d \t %d\n\r", pos[0], pos[1]);
//		_delay_ms(10);
////	printf("IR Values: %d, %d\n\r", IRDist(6), IRDist(7));
//	}
	while(1)
		;


}
