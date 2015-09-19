#include "RBELib/RBELib.h"
#include "RBELib/pot.h"

unsigned long ms;
short count;
short pidCount;
volatile int upperLinkActual, lowerLinkActual;
volatile long pidOutputUpper, pidOutputLower;
float degToRadConstant = 0.01745329251;

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
	debugUSARTInit(DEFAULT_BAUD);
	initRBELib();
	initSPI();
	initADC(3, ADC_FREE_RUNNING, ADC_REF_VCC);
	potCalibration calUpper =
	{ 250, 625, 975 };
	initPot(0, 3, calUpper);
	potCalibration calLower =
	{ 255, 668, 1100 };
	initPot(1, 2, calLower);
	upperLinkActual = potAngle(0);
	lowerLinkActual = potAngle(1);
	configureMsTimer();
	setPinsDir('A', INPUT, 4, PORT4, PORT5, PORT6, PORT7); //not using ADC channels other than 0 and 1, configure with those
	setConst('U', 690.0, 3.0, 64.0); //set PID gains 690.0, 3.0, 64.0
	setConst('L', 700.0, 6.0, 63.0);
	unsigned long lastMs;
	while (1)
	{
		if (getPinsVal('A', 1, PORT4))
		{
			gotoAngles(-90, -90);
		}
		else if (getPinsVal('A', 1, PORT5))
		{
			gotoAngles(-60, -60);
		}
		else if (getPinsVal('A', 1, PORT6))
		{
			gotoAngles(-30, -30);
		}
		else if (getPinsVal('A', 1, PORT7))
		{
			gotoAngles(0, 0);
		}
		driveLink(2, pidOutputUpper);
		driveLink(1, pidOutputLower);
		upperLinkActual = potAngle(0);
		lowerLinkActual = potAngle(1);

		printf("%d\n\r", getADC(2));
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

	float lowerLinkActualRad = (potAngleFloat(1) - 90.0) * degToRadConstant;
	float upperLinkActualRad = (potAngleFloat(0) - 90.0) * degToRadConstant;

	y = (137.16 + cos(lowerLinkActualRad) * a1 + cos(upperLinkActualRad + lowerLinkActualRad) * a2);
	x = -1*(sin(lowerLinkActualRad) * a1 + sin(upperLinkActualRad + lowerLinkActualRad) * a2);
	// printf("%f, %f, %f, %f\n\r", potAngleFloat(1) - 90.0, potAngleFloat(0) - 90.0, x, y);

	pos[0] = (int)x;
	pos[1] = (int)y;

}

//void drawTriangle()
//{
//	debugUSARTInit(DEFAULT_BAUD);
//	initRBELib();
//	initSPI();
//	initADC(3, ADC_FREE_RUNNING, ADC_REF_VCC);
//	potCalibration calUpper =
//	{ 250, 625, 975 };
//	initPot(0, 3, calUpper);
//	potCalibration calLower =
//	{ 255, 668, 1100 };
//	initPot(1, 2, calLower);
//	upperLinkActual = potAngle(0);
//	lowerLinkActual = potAngle(1);
//	configureMsTimer();
//	setConst('U', 690.0, 3.0, 64.0); //set PID gains 690.0, 3.0, 64.0
//	setConst('L', 700.0, 6.0, 63.0);
//	int posX = 100;
//	int posY = 100;
//	gotoXY(posX, posY);
//	int side = 1;
//	int count = 0;
//	int lastMs = ms;
//	while (1)
//	{
//		gotoXY(posX, posY);
//		driveLink(2, pidOutputUpper);
//		driveLink(1, pidOutputLower);
//		upperLinkActual = potAngle(0);
//		lowerLinkActual = potAngle(1);
//
//		if (ms - lastMs >= 100)
//		{
//			if (count == 100) //100mm, side length
//			{
//				count = 0;
//				side++;
//				if (side > 3)
//					side = 1;
//			}
//			switch (side)
//			{
//			case 1:
//				posX++;
//				break;
//			case 2:
//				posY++;
//				posX--;
//				break;
//			case 3:
//				posY--;
//				posX--;
//				break;
//			}
//			count++;
//			lastMs = ms;
//		}
//	}
//}

int main(void)
{
	debugUSARTInit(DEFAULT_BAUD);
	initRBELib();
	initSPI();
	// pidButtons2();
	while(1)
	{
		int pos[2];
		calcXY(pos);
		printf("%d, %d\n\r", pos[0], pos[1]);
	}
}
