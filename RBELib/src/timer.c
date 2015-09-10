/*
 * timer.c
 *
 *  Created on: Sep 5, 2015
 *      Author: Rachael Putnam
 */

#include <RBELib/RBELib.h>
#include <RBELib/ADC.h>

void configureMsTimer()
{
	sei();
	TCCR0A = 0b00000000;
	TCCR0B = 0b00000010;
	TIMSK0 = 0b00000001;
}

void setTimerPeriod(int timer, double freq)
{
	int prescaler = 0;
	int scale = 0;
	if ((timer == 0) || (timer == 2))
	{
		//decide prescaler depending of desired frequency
		if ((freq >= 35.2) && (freq <= 9000.0))
		{
			prescaler = SCALE1024;
			scale = 1024;
		}
		else if ((freq >= 140.625) && (freq <= 36000.0))
		{
			prescaler = SCALE256;
			scale = 256;
		}
		else if ((freq >= 562.5) && (freq <= 144000.0))
		{
			prescaler = SCALE64;
			scale = 64;
		}
		else if ((freq >= 4500.0) && (freq <= 1152000.0))
		{
			prescaler = SCALE8;
			scale = 8;
		}
		else if ((freq >= 36000.0) && (freq <= (double) (F_CPU / 2)))
		{
			prescaler = SCALE1;
			scale = 1;
		}
	}
	else
	{
		//decide prescaler depending of desired frequency
		if ((freq >= .253) && (freq <= 9000.0))
		{
			prescaler = SCALE1024;
			scale = 1024;
		}
		else if ((freq >= 1.01) && (freq <= 36000.0))
		{
			prescaler = SCALE256;
			scale = 256;
		}
		else if ((freq >= 4.04) && (freq <= 144000.0))
		{
			prescaler = SCALE64;
			scale = 64;
		}
		else if ((freq >= 32.39) && (freq <= 1152000.0))
		{
			prescaler = SCALE8;
			scale = 8;
		}
		else if ((freq >= 259.12) && (freq <= (double) (F_CPU / 2)))
		{
			prescaler = SCALE1;
			scale = 1;
		}
	}

	printf("Prescaler: %d\n\r", scale);
	//calculate value to compare to
	unsigned int comp = (int) (8000000 / (2 * scale * freq)) - 1;
	printf("Compare: %#X\n\r", comp);

	//set values of prescaler and compare
	setPrescale(timer, prescaler);
	setCompValue(timer, comp);
}

void setPrescale(int timer, int prescale)
{
	switch (timer)
	{
	case 0:
		TCCR0B &= 0b11111000;
		TCCR0B |= prescale;
		break;
	case 1:
		TCCR1B &= 0b11111000;
		TCCR1B |= prescale;
		break;
	case 2:
		TCCR2B &= 0b11111000;
		TCCR2B |= prescale;
		break;
	}
}

void initTimer(int timer, int mode, unsigned int comp)
{
	switch (timer)
	{
	case 0:	//Timer 0
		TCCR0A = 0b00000000;
		TCCR0B = 0b00000010;
		TIMSK0 = 0b00000001;
		break;
	case 1: //Timer 1
		//Clock source, no prescaling for now
		//to change the prescaling, see page 100
		TCCR1B &= 0b11111001;
		TCCR1B |= 0b00000001;
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
		setCompValue(1, comp);
		break;
	case 2:	//Timer 2
		//Clock source, no prescaling for now
		//to change the prescaling, see page 100
		TCCR2B &= 0b11111001;
		TCCR2B |= 0b00000001;

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
		setCompValue(2, comp);
		break;
	}
}

void setCompValue(unsigned char timer, unsigned int comp)
{
	switch (timer)
	{
	case 0:
		OCR0A = comp;
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

