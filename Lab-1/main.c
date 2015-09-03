/*
 * main.c
 *
 *  Created on: Aug 6, 2014
 *      Author: ewillcox
 */
#include "RBELib/RBELib.h"
int main(void)
{
	DDRBbits._P4 = OUTPUT;
	DDRBbits._P4 = OUTPUT;
	while (1)
	{
		PINBbits._P4 = 0;
		_delay_ms(100);
		PINBbits._P4 = 1;
		_delay_ms(100);
	}
	return 0;
}
