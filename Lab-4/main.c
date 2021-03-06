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
int slow = 0;
int blockType = 3;
int distance = -1;
int pickedUp = 0;

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
		if(slow == 0)
		{
			driveLink(2, pidOutputUpper);
			driveLink(1, pidOutputLower);
		}
		else
		{
//			driveLinkSlow(2, pidOutputUpper);
//			driveLinkSlow(1, pidOutputLower);
			stopMotors();
		}
	}
}


void setup()
{
	debugUSARTInit(DEFAULT_BAUD);					// Setup serial
	initRBELib();									// Setup RBELib
	initSPI();										// Setup SPI/DAC
	initADC(3, ADC_FREE_RUNNING, ADC_REF_VCC);		// Setup ADC
	configureMsTimer();								// Setup ms timer and interrupts
	potCalibration calUpper = { 259, 645, 1014 };	// Setup upper arm pot
	initPot(0, 3, calUpper);
	potCalibration calLower = { 260, 680, 1070 };	// Setup lower arm pot
	initPot(1, 2, calLower);
	setConst('U', 300.0, 0.0, 10.0); 				// Setup upper arm gains
	setConst('L', 300.0, 0.0, 10.0);				// Setup lower arm gains
	encInit(0);
	encInit(1);
	blockType = 3;
	distance = -1;
	pickedUp = 0;
}

void home()
{
	gotoAngles(0,0);

	while(!IN_RANGE(upperLinkActual,91,89)|| !IN_RANGE(lowerLinkActual,91,89))
	{
		// printf("%d,%d,%d,%d,%d\n\r", lowerLinkActual, upperLinkActual, blockType, distance, pickedUp);
	}
	_delay_ms(500);
	resetEncCount(0);
	resetEncCount(1);
}

int stableValue(int channel)
{

	int hasTriggered = 0;
	int numVals = 7;
	float sum = 0;
	int min = 100;
	float average;
	int vals[numVals];

	for(int i = 0; i < numVals; i++)
	{
		vals[i] = IRDist(channel);
		_delay_ms(5);
	}
	sum = 0;
	for(int p = 0; p < numVals; p++)
	{
		sum += vals[p];
	}
	average = sum / numVals;
	int startValue = (int)average;

	int i = 0;

	while((hasTriggered == 0) || (int)average != startValue)
	{
		// Moving average
		vals[i%numVals] = IRDist(channel);

		sum = 0;
		for(int p = 0; p < numVals; p++)
		{
			sum += vals[p];
		}
		average = sum / numVals;

		// If less than the minimum, make minimum
		if(average < min) {min = (int)average;/*printf("New min: %d\n\r", min);*/}

		// Check triggered
		if(min < 13) {hasTriggered = 1;}


		i++;
		_delay_ms(5);
	}
	distance = min;
	blockType = 0;
	// printf("%d,%d,%d,%d,%d\n\r", lowerLinkActual, upperLinkActual, blockType, distance, pickedUp);
	return min;
}

pickupInfo calcPickupInfo()
{
	float deltaX = 7.75;
	int deltaT;
	float distanceToArmCenter = 11.25;

	int startIRVal_1 = -1;
	int startIRVal_2 = -1;

	startIRVal_1 = stableValue(6);
	long passTime1 = ms;

//	int tmp_y = - 175 - ((startIRVal_1) / 2.0 - 6.0) * 10;
//	int tmp_x = - 20.0 + (tmp_y + 175) * (-30.0/65.0) + 30;
	gotoXY(30, -220);

	startIRVal_2 = stableValue(7);
	long passTime2 = ms;

	// printf("Start values: %d, %d\n\r", startIRVal_1, startIRVal_2);

	deltaT = (int)(passTime2 - passTime1);
	// printf("Delta time: %d\n\r", deltaT);

	float velocity = deltaX / (float)deltaT;
	// printf("Velocity: %f\n\r", velocity);

	float timeToPickup = distanceToArmCenter/velocity;
	// printf("Time: %f\n\r", timeToPickup);

	pickupInfo info;
	info.y = - 182 - ((startIRVal_1 + startIRVal_2) / 2.0 - 5.5) * 11;
	info.x = - 15 + (info.y + 182) * (-27.0/66.0);
	info.time = timeToPickup - 1100;// + -180*(info.y + 180.0)/62.0;
	return info;
	// -182, -7 to -248, 20
}

void waitForArmMovement()
{
	long startTime = ms;
	while (!IN_RANGE(upperLinkActual, upperLinkSetpoint + 2, upperLinkSetpoint - 2) ||
			!IN_RANGE(lowerLinkActual, lowerLinkSetpoint + 2, lowerLinkSetpoint - 2))
	{
		// printf("%d,%d,%d,%d,%d\n\r", lowerLinkActual, upperLinkActual, blockType, distance, pickedUp);
		if(ms - startTime > 5000)
			break;
	}


}

void timerWeighingRoutine()
{
	gotoAngles(-70,0);
	waitForArmMovement();
	_delay_ms(500);
	slow = 1;
	long startTime = ms;
	gotoAngles(-90,90);
	int lowerLinkInit = lowerLinkActual;
	int upperLinkInit = upperLinkActual;
	while (!IN_RANGE(upperLinkActual, upperLinkSetpoint + 2, upperLinkSetpoint - 2) ||
				!IN_RANGE(lowerLinkActual, lowerLinkSetpoint + 2, lowerLinkSetpoint - 2))
	{
		// printf("%d,%d,%d,%d,%d\n\r", lowerLinkActual, upperLinkActual, blockType, distance, pickedUp);
		if(ms - startTime > 5000)
			break;
	}
	int lowerLinkDiff = lowerLinkActual - lowerLinkInit;
	int upperLinkDiff = upperLinkActual - upperLinkInit;
	//printf("DTheta: %d\n\r", lowerLinkDiff + upperLinkDiff);
	_delay_ms(1000);
	slow = 0;

	if(lowerLinkDiff + upperLinkDiff > -10)// Light weight
	{
		//printf("Light weight.\n\r");
		blockType = 1;
		gotoXY(0,-300);
		waitForArmMovement();
		_delay_ms(500);
		openGripper(1);
		pickedUp = 0;
		_delay_ms(500);
	}
	else // Heavy weight
	{
		//printf("Heavy weight.\n\r");
		blockType = 2;
		gotoAngles(-30,0);
		waitForArmMovement();
		gotoXY(30, -200);
		waitForArmMovement();
		_delay_ms(500);
		openGripper(1);
		pickedUp = 0;
		_delay_ms(500);
	}
}

int main(void)
{
	setup();
	while(1)
	{
		setup();
		home();
		openGripper(1);
		gotoXY(30, -220);
		waitForArmMovement();
		_delay_ms(50);
		setServo(0, 0);
		pickupInfo info = calcPickupInfo();
		gotoXYMatlab((int)info.x, (int)info.y);
		_delay_ms((int)info.time);
		//	printf("Pickup pos: %d, %d\n\r\n\r", (int)info.x, (int)info.y);
		gotoXYMatlab((int)info.x, (int)info.y);
		waitForArmMovement();
		_delay_ms(100);
		closeGripper(1);
		pickedUp = 1;
		_delay_ms(500);
		gotoXY(30, -220);
		waitForArmMovement();

//		Weighing routine
		timerWeighingRoutine();
	}

//	while(1)
//	{
//		int pos[2];
//		calcXY(pos);
//		printf("%d, %d", pos[0], pos[1]);
//		printf("\t\t%d, %d\n\r", IRDist(6), IRDist(7));
//		_delay_ms(100);
//	}
}
