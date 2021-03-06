#include "RBELib/RBELib.h"

unsigned long ms;
short count;
short pidCount;
volatile int upperLinkActual, lowerLinkActual;
volatile long pidOutputUpper, pidOutputLower;


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
		pidOutputUpper = calcPID('U', upperLinkActual);
		pidOutputLower = calcPID('L', lowerLinkActual);
	}
}

void armDataCapture()
{
	debugUSARTInit(DEFAULT_BAUD);
	initRBELib();
	initADC(3, ADC_FREE_RUNNING, ADC_REF_VCC);
	potCalibration cal =
	{ 250, 625, 975 };
	initPot(0, 3, cal);

	while (1)
	{
		printf("%d, %d, %d\n\r", potAngle(0), potVolts(0), getADCValue());
		_delay_ms(10);
	}
}

void outputTriangleWave()
{
	debugUSARTInit(DEFAULT_BAUD);
	initRBELib();
	initSPI();
	int dac_0_count = 0;
	int dac_0_dir = 1;
	int dac_1_count = 4095;
	int dac_1_dir = 0;
	while (1)
	{
		setDAC(0, dac_0_count);
		setDAC(1, dac_1_count);
		if (dac_0_dir == 1)
			dac_0_count += 5;
		else
			dac_0_count -= 5;
		if (dac_0_count >= 4095)
		{
			dac_0_count = 4095;
			dac_0_dir = 0;
		}
		else if (dac_0_count <= 0)
		{
			dac_0_count = 0;
			dac_0_dir = 1;
		}

		if (dac_1_dir == 1)
			dac_1_count += 5;
		else
			dac_1_count -= 5;
		if (dac_1_count >= 4095)
		{
			dac_1_count = 4095;
			dac_1_dir = 0;
		}
		else if (dac_1_count <= 0)
		{
			dac_1_count = 0;
			dac_1_dir = 1;
		}
	}
}

void readCurrent()
{
	debugUSARTInit(DEFAULT_BAUD);
	initRBELib();
	initSPI();
	initADC(1, ADC_FREE_RUNNING, ADC_REF_VCC);
	while (1)
	{
		driveLink(2, 4095);
		printf("%d\n\r", readCurrentMilliamps(2));
		_delay_ms(10);
	}
}

//void tunePID()
//{
//	lowerLinkSetpoint = 90;
//	debugUSARTInit(DEFAULT_BAUD);
//	initRBELib();
//	initSPI();
//	initADC(2, ADC_FREE_RUNNING, ADC_REF_VCC);
//	potCalibration cal =
//	{ 250, 685, 1100 };
//	initPot(1, 2, cal);
//	lowerLinkActual = potAngle(1);
//	configureMsTimer();
//	setPinsDir('D', INPUT, 1, PORT0);
////	setPinsDir('B', INPUT, 4, PORT1, PORT2, PORT7, PORT4);
//	float kP, kI, kD;
//	unsigned long lastMs = ms;
//	while (1)
//	{
//		if (ms - lastMs >= 100)
//		{
//			kP = getADC(5); // 690
//			kI = getADC(6); // 3
//			kD = getADC(7); // 64
//			 printf("kP: %f, kI: %f, kD: %f, read: %d, out: %ld\n\r", kP, kI, kD, lowerLinkActual, pidOutputLower);
//			// printf("%d\n\r", upperLinkActual);
//
//			// Calculate motor input voltage
////			float motorVoltageMv = pidOutputUpper;
////			if (motorVoltageMv > 4095)
////				motorVoltageMv = 4095;
////			if (motorVoltageMv < -4095)
////				motorVoltageMv = -4095;
////			motorVoltageMv = motorVoltageMv * 8000.0 / 4096.0;
////
////			// Print statistics
////			printf("Command Pos: %d, Arm Pos: %d, Voltage (mV): %f, Current: %d\n\r", upperLinkSetpoint, upperLinkActual, motorVoltageMv, readCurrentMilliamps(2));
//			lastMs = ms;
//		}
//		if (getPinsVal('D', 1, PORT0))
//		{
//			printf("Setting pins val...\n\r");
//			setConst('L', kP, kI, kD);
//		}
//		lowerLinkActual = potAngle(1);
//		driveLink(1, pidOutputLower);
//	}
//}

//void pidButtons()
//{
//	upperLinkSetpoint = 90;
//	debugUSARTInit(DEFAULT_BAUD);
//	initRBELib();
//	initSPI();
//	initADC(3, ADC_FREE_RUNNING, ADC_REF_VCC);
//	potCalibration cal =
//	{ 250, 625, 975 };
//	initPot(0, 3, cal);
//	upperLinkActual = potAngle(0);
//	configureMsTimer();
//	setPinsDir('A', INPUT, 4, PORT4, PORT5, PORT6, PORT7); //not using ADC channels other than 0 and 1, configure with those
//	setConst('U', 690.0, 3.0, 64.0); //set PID gains 690.0, 3.0, 64.0
//	unsigned long lastMs;
//	while (1)
//	{
//		if (getPinsVal('A', 1, PORT4))
//		{
//			upperLinkSetpoint = 0;
//		}
//		else if (getPinsVal('A', 1, PORT5))
//		{
//			upperLinkSetpoint = 30;
//		}
//		else if (getPinsVal('A', 1, PORT6))
//		{
//			upperLinkSetpoint = 60;
//		}
//		else if (getPinsVal('A', 1, PORT7))
//		{
//			upperLinkSetpoint = 90;
//		}
//		driveLink(2, pidOutputUpper);
//		upperLinkActual = potAngle(0);
//
//		if (ms - lastMs >= 10)
//		{
//			// Calculate motor input voltage
//			float motorVoltageMv = pidOutputUpper;
//			if (motorVoltageMv > 4095)
//				motorVoltageMv = 4095;
//			if (motorVoltageMv < -4095)
//				motorVoltageMv = -4095;
//			motorVoltageMv = motorVoltageMv * 8000.0 / 4096.0;
//
//			printf("%d, %d, %f, %d\n\r", upperLinkSetpoint, upperLinkActual, motorVoltageMv, readCurrentMilliamps(2));
//			lastMs = ms;
//		}
//
//	}
//}

void pidButtons2()
{
	gotoAngles(0, 0);
	setPinsDir('A', INPUT, 4, PORT4, PORT5, PORT6, PORT7); //not using ADC channels other than 0 and 1, configure with those
	unsigned long lastMs;
	while (1)
	{
		if (getPinsVal('A', 1, PORT4))
		{
			//gotoAngles(-90, -90);
			gotoXY(260, 0);
//			printf("1\n\r");
		}
		else if (getPinsVal('A', 1, PORT5))
		{
			//gotoAngles(-60, -60);
			gotoXY(0, -260);
//			printf("2\n\r");
		}
		else if (getPinsVal('A', 1, PORT6))
		{
			//gotoAngles(-30, -30);
			gotoXY(200, 0);
//			printf("3\n\r");
		}
		else if (getPinsVal('A', 1, PORT7))
		{
			//gotoAngles(0, 0);
			gotoXY(0, -200);
//			printf("4\n\r");
		}
		upperLinkActual = potAngle(0);
		lowerLinkActual = potAngle(1);
		driveLink(2, pidOutputUpper);
		driveLink(1, pidOutputLower);

		if (ms - lastMs >= 10)
		{
			int pos[2];
			calcXY(pos);
			printf("%d, %d, %d, %d\n\r", lowerLinkActual, upperLinkActual, pos[0], pos[1]);
			lastMs = ms;
		}

//		printf("%d\n\r", getADC(2));
//		if (ms - lastMs >= 10)
//		{
//			// Calculate motor input voltage
//			float motorVoltageMv = pidOutputUpper;
//			if (motorVoltageMv > 4095)
//				motorVoltageMv = 4095;
//			if (motorVoltageMv < -4095)
//				motorVoltageMv = -4095;
//			motorVoltageMv = motorVoltageMv * 8000.0 / 4096.0;
//
//			printf("%d, %d, %f, %d\n\r", upperLinkSetpoint, upperLinkActual, motorVoltageMv, readCurrentMilliamps(2));
//			lastMs = ms;
//		}
	}
}

void calcXY(int* pos)
{
	float a1 = 152.4; // in mm
	float a2 = 111.76; // in mm

	float x, y;

	initADC(3, ADC_FREE_RUNNING, ADC_REF_VCC);
	potCalibration calUpper =
	{ 250, 625, 975 };
	initPot(0, 3, calUpper);
	potCalibration calLower =
	{ 255, 668, 1100 };
	initPot(1, 2, calLower);

	float lowerLinkActualRad = (potAngleFloat(1) - 90.0) * 0.01745329251; // degToRad constant
	float upperLinkActualRad = (potAngleFloat(0) - 90.0) * 0.01745329251;

	x = (cos(lowerLinkActualRad) * a1 + cos(upperLinkActualRad + lowerLinkActualRad) * a2);
	y = (sin(lowerLinkActualRad) * a1 + sin(upperLinkActualRad + lowerLinkActualRad) * a2);
	// printf("%f, %f, %f, %f\n\r", potAngleFloat(1) - 90.0, potAngleFloat(0) - 90.0, x, y);

	pos[0] = (int)x;
	pos[1] = (int)y;

}

void testGotoXY()
{
	int state = 0;
	int x = 260;
	int y = 0;
	long lastms = ms;
	int counter = 0;
	while(1)
	{
		upperLinkActual = potAngle(0);
		lowerLinkActual = potAngle(1);
		// Two pots to control the XY position
//		int x = (int)(getADC(4)*264.0/1024.0);
//		int y = -(int)(getADC(5)*264.0/1024.0);

		gotoXY(x, y);
		driveLink(2, pidOutputUpper);
		driveLink(1, pidOutputLower);

//		printf("X: %d, Y: %d\n\r", x, y);
		if (ms - lastms > 20)
		{
			lastms = ms;
			switch (state)
			{
			case 0:
				x-= 1;
				y = 0;
				if (x <= 200)
					state++;
				break;
			case 1:
				x = 200;
				y -= 1;
				if (y <= -60)
					state++;
				break;
			case 2:
				x += 1;
				y += 1;
				if (x >= 260)
					state = 0;
				break;
			}
//			switch (state)
//			{
//			case 0:
//				x = 260;
//				y = 0;
//				counter++;
//				if (counter >= 200)
//				{
//					state++;
//					counter = 0;
//				}
//				break;
//			case 1:
//				x = 200;
//				y = 0;
//				counter++;
//				if (counter >= 200)
//				{
//					state++;
//					counter = 0;
//				}
//				break;
//			case 2:
//				x = 200;
//				y = -60;
//				counter++;
//				if (counter >= 200)
//				{
//					state = 0;
//					counter = 0;
//				}
//				break;
//			}
			int pos[2];
			calcXY(pos);
			printf("%d, %d, %d, %d\n\r", lowerLinkActual, upperLinkActual, pos[0], pos[1]);
		}
	}
}

void testForwardKinematics()
{
	while(1)
	{
		int lowerTheta = (int)(getADC(4)*180.0/1024.0 - 90);
		int upperTheta = (int)(getADC(5)*180.0/1024.0 - 90);
		upperLinkActual = potAngle(0);
		lowerLinkActual = potAngle(1);
		gotoAngles(lowerTheta, upperTheta);
		driveLink(1, pidOutputLower);
		driveLink(2, pidOutputUpper);
		int pos[2];
		calcXY(pos);
		// printf("%ld, %ld\n\r", pidOutputLower, pidOutputUpper);
		printf("%d, %d, %d, %d, %d, %d\n\r", lowerTheta, upperTheta, lowerLinkActual, upperLinkActual, pos[0], pos[1]);
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
}

int main(void)
{
	setup();
//	pidButtons2();
//	testForwardKinematics();
	testGotoXY();


}
