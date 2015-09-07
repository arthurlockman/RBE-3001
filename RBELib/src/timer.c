/*
 * timer.c
 *
 *  Created on: Sep 5, 2015
 *      Author: Rachael Putnam
 */

#include <RBELib/RBELib.h>
#include <RBELib/ADC.h>

void initTimer(int timer, int mode, unsigned int comp)
{
	switch (timer)
	{
	case 0:	//Timer 0
		//Clock source, no prescaling for now
		//to change the prescaling, see page 100
		TCCR0B &= 0b11111001;
		TCCR0B |= 0b00000001;

		switch (mode)
		{
		case 0: //Normal mode
			TCCR0A &= 0b11111100;
			TCCR0B &= 0b11111011;
			break;
		case 1:
			break;
		case 2: //CTC mode
			TCCR1A &= 0b11111110;
			TCCR1A |= 0b00000010;
			TCCR1B &= 0b11111011;
			break;
		}
		setCompValue(0,comp);
		break;
	case 1: //Timer 1
		switch (mode)
		{
		case 0: //Normal mode
			TCCR1A &= 0b11111100;
			TCCR1B &= 0b11111011;
			break;
		case 1:
			break;
		case 2: //CTC mode
			TCCR1A |= 0b00000001;
			TCCR1A |= 0b00000010;
			TCCR1B &= 0b11111011;
			break;
		}
		break;
	case 2:	//Timer 2
		switch (mode)
		{
		case 0: //Normal mode
			TCCR2A &= 0b11111100;
			TCCR2B &= 0b11111011;
			break;
		case 1:
			break;
		case 2: //CTC mode
			TCCR2A &= 0b11111110;
			TCCR2A |= 0b00000010;
			TCCR2B &= 0b11111011;
			break;
		}
		setCompValue(2,comp);
		break;

	}

}

void setCompValue(unsigned char timer, unsigned short int comp)
{
	switch (timer)
	{
	case 0:
		OCR0A = comp;
		TIMSK0 = 0b00000010;
		break;
	case 1:
		OCR1A = comp;
		TIMSK1 = 0b00000010;
		break;
	case 2:
		OCR2A = comp;
		TIMSK2 = 0b00000010;
		break;
	}
}

