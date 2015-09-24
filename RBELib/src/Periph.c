/*
 * Periph.c
 *
 *  Created on: Sep 24, 2015
 *      Author: Tucker Haydon
 */

#include "RBELib/RBELib.h"

// signed int getAccel(int axis);

// int IRDist(int chan);

void encInit(int chan)
{
	ENCODER_SS_0_ddr = OUTPUT;
	ENCODER_SS_1_ddr = OUTPUT;

	switch(chan)
	{
	case 0:
		// Clearing count
		ENCODER_SS_0 = LOW;
		spiTransceive(0b00100000);
		ENCODER_SS_0 = HIGH;
		// Setting MDR0
		ENCODER_SS_0 = LOW;
		spiTransceive(0b10001000);
		spiTransceive(0b00000011);
		ENCODER_SS_0 = HIGH;
		// Setting MDR1
		ENCODER_SS_0 = LOW;
		spiTransceive(0b10010000);
		spiTransceive(0b00000000);
		ENCODER_SS_0 = HIGH;
		break;
	case 1:
		// Clearing count
		ENCODER_SS_1 = LOW;
		spiTransceive(0b00100000);
		ENCODER_SS_1 = HIGH;
		// Setting MDR0
		ENCODER_SS_1 = LOW;
		spiTransceive(0b10001000);
		spiTransceive(0b00000011);
		ENCODER_SS_1 = HIGH;
		// Setting MDR1
		ENCODER_SS_1 = LOW;
		spiTransceive(0b10010000);
		spiTransceive(0b00000000);
		ENCODER_SS_1 = HIGH;
		break;
		break;
	default:
		break;
	}
}

void resetEncCount(int chan)
{
	switch(chan)
	{
	case 0:
		// Set count to 0
		ENCODER_SS_0 = LOW;
		spiTransceive(0b00100000);
		ENCODER_SS_0 = HIGH;
		break;
	case 1:
		// Set count to 0
		ENCODER_SS_1 = LOW;
		spiTransceive(0b00100000);
		ENCODER_SS_1 = HIGH;
		break;
	default:
		break;
	}
}

signed long encCount(int chan)
{
	long encValue = 0;
	switch(chan)
		{
		case 0:
			// Send the command over MOSI
			ENCODER_SS_0 = LOW;
			spiTransceive(0b01100000);
			// Read over MISO 4 times = 4 bytes = 32 bits
			encValue |= (long)spiTransceive(0) << 24;
			encValue |= (long)spiTransceive(0) << 16;
			encValue |= (long)spiTransceive(0) << 8;
			encValue |= (long)spiTransceive(0) << 0;
			ENCODER_SS_0 = HIGH;
			break;
		case 1:
			// Send the command over MOSI
			ENCODER_SS_1 = LOW;
			spiTransceive(0b01100000);
			// Read over MISO 4 times = 4 bytes = 32 bits
			encValue |= (long)spiTransceive(0) << 24;
			encValue |= (long)spiTransceive(0) << 16;
			encValue |= (long)spiTransceive(0) << 8;
			encValue |= (long)spiTransceive(0) << 0;
			ENCODER_SS_1 = HIGH;
			encValue = -encValue;
			break;
		default:
			printf("Ya fucked up.");
			break;
		}
	return encValue;
}
