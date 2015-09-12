/*
 * SPI.c
 *
 *  Created on: Sep 12, 2015
 *      Author: Tucker Haydon
 */

#include "RBELib/RBELib.h"

void initSPI()
{
	SPI_MOSI_DDR = OUTPUT;
	SPI_MISO_DDR = INPUT;
	SPI_SCK_DDR  = OUTPUT;
	SPI_MASTER_SS = OUTPUT;
	DAC_SS_ddr = OUTPUT;
	DAC_SS = HIGH;
	// No interrupts, enable, MSB first, master mode, rising then falling, sample then setup, Fosc/4
	SPCR &= 0b00000000;
	SPCR |= 0b01010000;
}

unsigned char spiTransceive(BYTE data)
{
	SPDR = data;
	while (!(SPSR & (1<<SPIF)))
		;
	return SPDR;
}
