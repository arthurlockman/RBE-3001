#include "RBELib/RBELib.h"
#include "RBELib/pot.h"

int main(void)
{
	debugUSARTInit(DEFAULT_BAUD);
	initRBELib();
	initADC(3, ADC_FREE_RUNNING, ADC_REF_VCC);
	potCalibration cal = {250, 625, 975};
	initPot(0, 3, cal);

	while (1)
	{
		printf("Angle: %d, mV: %d\n\r", potAngle(0), potVolts(0));
		_delay_ms(10);
	}

	return 0;
}
