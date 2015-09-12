/*
 * DAC.c
 *
 *  Created on: Sep 12, 2015
 *      Author: Tucker Haydon
 */

#include <RBELib/RBELib.h>

void setDAC(int DACn, int SPIVal)
{
	setPinsVal('D', LOW, 1, PORT4); //pulls CS low
	int word;
	switch (DACn)
	{
	case 0:
		word = 0b00110000;
		break;
	case 1:
		word = 0b00110001;
		break;
	case 2:
		word = 0b00110010;
		break;
	case 3:
		word = 0b00110011;
		break;
	}
	spiTransceive(word); //send address and command
	spiTransceive((int)(SPIVal >> 4)); //send first 8 bits
	spiTransceive((int)((SPIVal << 4) & 0b0000000011110000)); //send last bits of 0s
	setPinsVal('D', HIGH, 1, PORT4); //set CS to high
}
