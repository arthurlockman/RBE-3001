#include "RBELib/RBELib.h"
#include "RBELib/pot.h"

long ms;
short count;
short pidCount;
int upperLinkSetpoint, upperLinkActual;

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
		calcPID('U', upperLinkSetpoint, upperLinkActual);
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
	upperLinkSetpoint = 0;
	debugUSARTInit(DEFAULT_BAUD);
	initRBELib();
	initSPI();
	initADC(1, ADC_FREE_RUNNING, ADC_REF_VCC);
	potCalibration cal =
	{ 250, 625, 975 };
	initPot(0, 3, cal);
	upperLinkActual = potAngle(0);
	configureMsTimer();

	int kPraw = getADC(7); //TODO: FIX THESE
	int kIraw = getADC(8);
	int kDraw = getADC(9);
	float kP, kI, kD;
	while (1)
	{
		char changed = 0;
		if (getADC(7) != kPraw)
		{
			changed = 1;
			kP = getADCValue(); //TODO: Scale values
			kPraw = getADCValue();
		}
		if (getADC(7) != kIraw)
		{
			changed = 1;
			kI = getADCValue();
			kIraw = getADCValue();
		}
		if (getADC(7) != kDraw)
		{
			changed = 1;
			kD = getADCValue();
			kDraw = getADCValue();
		}
		if (changed)
		{
			setConst('U', kP, kI, kD);
			printf("kP: %f, kI: %f, kD: %f\n\r", kP, kI, kD);
		}
	}
	upperLinkActual = potAngle(0);
}

int main(void)
{
	tunePID();
}
