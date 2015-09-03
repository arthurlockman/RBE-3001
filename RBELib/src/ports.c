/*
 * ports.c
 *
 *  Created on: Sep 3, 2015
 *      Author: Tucker Haydon
 */

#include <RBELib/RBELib.h>

// Takes a number of inputs as integers representing the pins
// MAKE SURE THE PORTS ARE DEFINED IN RBELib.h
// MAKE SURE CHAR IS CHANGED TO INT IN HEADER
void setPinsDir(char port, int dir, int numPins, ...)
{
	va_list listPointer;								// pointer to the ...
	va_start(listPointer, numPins);					// Initializing the pointer

	// Find the correct port struct
	__8bitreg_t* portPointer;
	switch (port)
	{
	case 'A':
		portPointer = &DDRAbits;
		break;
	case 'B':
		portPointer = &DDRBbits;
		break;
	case 'C':
		portPointer = &DDRCbits;
		break;
	case 'D':
		portPointer = &DDRDbits;
		break;
	}

	for (int i = 0; i < numPins; i++)
	{
		int pin = va_arg(listPointer, int);
		// Set specified pins to specified dir
		switch (pin)
		{
		case PORT0:
			portPointer->_P0 = dir;
			break;
		case PORT1:
			portPointer->_P1 = dir;
			break;
		case PORT2:
			portPointer->_P2 = dir;
			break;
		case PORT3:
			portPointer->_P3 = dir;
			break;
		case PORT4:
			portPointer->_P4 = dir;
			break;
		case PORT5:
			portPointer->_P5 = dir;
			break;
		case PORT6:
			portPointer->_P6 = dir;
			break;
		case PORT7:
			portPointer->_P7 = dir;
			break;
		}
	}
}

void setPinsVal(char port, int val, int numPins, ...)
{
	va_list listPointer;								// pointer to the ...
	va_start(listPointer, numPins);					// Initializing the pointer

	// Find the correct port struct
	__8bitreg_t* portPointer;
	switch (port)
	{
	case 'A':
		portPointer = &PINAbits;
		break;
	case 'B':
		portPointer = &PINBbits;
		break;
	case 'C':
		portPointer = &PINCbits;
		break;
	case 'D':
		portPointer = &PINDbits;
		break;
	}

	for (int i = 0; i < numPins; i++)
	{
		int pin = va_arg(listPointer, int);
		// Set specified pins to specified val
		switch (pin)
		{
		case PORT0:
			portPointer->_P0 = val;
			break;
		case PORT1:
			portPointer->_P1 = val;
			break;
		case PORT2:
			portPointer->_P2 = val;
			break;
		case PORT3:
			portPointer->_P3 = val;
			break;
		case PORT4:
			portPointer->_P4 = val;
			break;
		case PORT5:
			portPointer->_P5 = val;
			break;
		case PORT6:
			portPointer->_P6 = val;
			break;
		case PORT7:
			portPointer->_P7 = val;
			break;
		}
	}
}

// Gets the value from the specified. Any error will return a value of 0b1111;
unsigned char getPinsVal(char port, int numPins, ...)
{
	va_list listPointer;								// pointer to the ...
	va_start(listPointer, numPins);					// Initializing the pointer

	// Find the correct port struct
	__8bitreg_t* portPointer;
	switch (port)
	{
	case 'A':
		portPointer = &PINAbits;
		break;
	case 'B':
		portPointer = &PINBbits;
		break;
	case 'C':
		portPointer = &PINCbits;
		break;
	case 'D':
		portPointer = &PINDbits;
		break;
	default:
		return 0b1111;
		break;
	}

	for (int i = 0; i < numPins; i++)
	{
		int pin = va_arg(listPointer, int);
		// Set specified pins to specified val
		switch (pin)
		{
		case PORT0:
			return portPointer->_P0;
			break;
		case PORT1:
			return portPointer->_P1;
			break;
		case PORT2:
			return portPointer->_P2;
			break;
		case PORT3:
			return portPointer->_P3;
			break;
		case PORT4:
			return portPointer->_P4;
			break;
		case PORT5:
			return portPointer->_P5;
			break;
		case PORT6:
			return portPointer->_P6;
			break;
		case PORT7:
			return portPointer->_P7;
			break;
		default:
			return 0b1111;
			break;
		}
	}
	return 0b1111;

}

