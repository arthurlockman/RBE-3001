/*
 * main.c
 *
 *  Created on: Aug 6, 2014
 *      Author: ewillcox
 */
#include "RBELib/RBELib.h"
#include "RBELib/ADC.h"

int main(void)
{
	initADC(7, ADC_FREE_RUNNING, ADC_VREF_VCC);
	while (1)
	{
	}
	return 0;
}
