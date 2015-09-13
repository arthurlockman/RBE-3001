#include "RBELib/RBELib.h"
#include "RBELib/pot.h"

unsigned long ms;
short count;
short pidCount;
volatile int upperLinkSetpoint, upperLinkActual;
volatile long pidOutput;

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
		pidOutput = calcPID('U', upperLinkSetpoint, upperLinkActual);
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
		} else if (dac_0_count <= 0)
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
		} else if (dac_1_count <= 0)
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

void tunePID()
{
	upperLinkSetpoint = 90;
	debugUSARTInit(DEFAULT_BAUD);
	initRBELib();
	initSPI();
	initADC(3, ADC_FREE_RUNNING, ADC_REF_VCC);
	potCalibration cal =
	{ 250, 625, 975 };
	initPot(0, 3, cal);
	upperLinkActual = potAngle(0);
	configureMsTimer();
	setPinsDir('D', INPUT, 1, PORT0);
//	setPinsDir('B', INPUT, 4, PORT1, PORT2, PORT7, PORT4);
	float kP, kI, kD;
	unsigned long lastMs = ms;
	while (1)
	{
		if (ms - lastMs >= 100)
		{
			kP = getADC(5); // 690
			kI = getADC(6); // 3
			kD = getADC(7); // 64
			// printf("kP: %f, kI: %f, kD: %f, read: %d, out: %ld\n\r", kP, kI, kD, upperLinkActual, pidOutput);
			// printf("%d\n\r", upperLinkActual);

			// Calculate motor input voltage
			float motorVoltageMv = pidOutput;
			if (motorVoltageMv > 4095)
				motorVoltageMv = 4095;
			if (motorVoltageMv < -4095)
				motorVoltageMv = -4095;
			motorVoltageMv = motorVoltageMv * 8000.0 / 4096.0;

			// Print statistics
			printf("Command Pos: %d, Arm Pos: %d, Voltage (mV): %f, Current: %d\n\r", upperLinkSetpoint, upperLinkActual, motorVoltageMv, readCurrentMilliamps(2));
			lastMs = ms;
		}
		if (getPinsVal('D', 1, PORT0))
		{
			printf("Setting pins val...\n\r");
			setConst('U', kP, kI, kD);
		}
		upperLinkActual = potAngle(0);
		driveLink(2, pidOutput);
	}
}

int main(void)
{
	tunePID();
}
