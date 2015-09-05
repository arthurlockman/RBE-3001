/*
 * ADC.c
 *
 *  Created on: Sep 3, 2015
 *      Author: Tucker Haydon
 */
#include <RBELib/RBELib.h>
#include <RBELib/ADC.h>

void initADC(int channel, adcMode mode, adcVref vref)
{
	// For these bits, see page 259 of the guide
	// Enable the ADC, allow interrupts, and set the clock division
	ADCSRA = BIT(ADEN) | (BIT(ADSC) & 0) | BIT(ADATE) | (BIT(ADIF) & 0) | (BIT(ADIE) & 0) | BIT(ADPS2) | BIT(ADPS1) | BIT(ADPS0);

	// For these bits, see page 262 of the guide
	// Set the trigger mode for the ADC
	ADCSRB &= 0b11111000;
	ADCSRB |= mode;

	// For these bits, see page 258 of the guide.
	// Set the voltage reference
	ADMUX &= 0b00111111;
	ADMUX |= vref;

	// Set the input channel
	changeADC(channel);
}

void clearADC(int channel)
{
	// For these bits, see page 259 of the guide
	// Disable the ADC
	ADCSRA &= (BIT(ADEN) & 0) | BIT(ADSC) | BIT(ADATE) | BIT(ADIF) | BIT(ADIE) | BIT(ADPS2) | BIT(ADPS1) | BIT(ADPS0);

	// Clear the data registers
	ADCH = 0b00000000;
	ADCL = 0b00000000;

	changeADC(-1);
}

UINT16 getADC(int channel)
{
	changeADC(channel);
	ADCSRA |= BIT(ADSC);
	while ((ADCSRA & BIT(ADIF)) == 0)
		;
	return getADCValue();
}

UINT16 getADCValue()
{
	return ADCW;
}

void changeADC(int channel)
{
	switch (channel)
	{
	case 0:
		ADMUX &= 0b11100000;
		break;
	case 1:
		ADMUX &= 0b11100000;
		ADMUX |= 0b00000001;
		break;
	case 2:
		ADMUX &= 0b11100000;
		ADMUX |= 0b00000010;
		break;
	case 3:
		ADMUX &= 0b11100000;
		ADMUX |= 0b00000011;
		break;
	case 4:
		ADMUX &= 0b11100000;
		ADMUX |= 0b00000100;
		break;
	case 5:
		ADMUX &= 0b11100000;
		ADMUX |= 0b00000101;
		break;
	case 6:
		ADMUX &= 0b11100000;
		ADMUX |= 0b00000110;
		break;
	case 7:
		ADMUX &= 0b11100000;
		ADMUX |= 0b00000111;
		break;
	default:
		ADMUX &= 0b11100000;
		ADMUX |= 0b00011111;
		break;
	}
}
