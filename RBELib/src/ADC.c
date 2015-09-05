/*
 * ADC.c
 *
 *  Created on: Sep 3, 2015
 *      Author: Tucker Haydon
 */
#include <RBELib/RBELib.h>
#include <RBELib/ADC.h>

/**
 * @brief Initializes the ADC and make one channel active.
 * You can choose to use either interrupts or polling to read
 * the desired channel.
 *
 * @param channel The ADC channel to initialize.
 *
 * @todo Create the corresponding function to initialize the ADC
 * using the channel parameter.
 */
void initADC(int channel, adcMode mode)
{
	// For these bits, see page 259 of the guide
	ADCSRA |= BIT(ADEN) | (BIT(ADSC) & 0) | BIT(ADATE) | (BIT(ADIF) & 0) | BIT(ADIE) | BIT(ADPS2) | BIT(ADPS1) | BIT(ADPS0);

	// For these bits, see page 262 of the guide
	ADCSRB |= mode;
}

/**
 * @brief Disables ADC functionality and clears any saved values (globals).
 *
 * @param channel  The ADC channel to disable.
 *
 * @todo Create the corresponding function to clear the last ADC
 * calculation register and disconnect the input to the ADC if desired.
 */
void clearADC(int channel);

/**
 * @brief Run a conversion on and get the analog value from one ADC
 * channel if using polling.
 *
 * @param channel  The ADC channel to run a conversion on.
 * @return adcVal The 8-10 bit value returned by the ADC
 * conversion.  The precision depends on your settings and
 * how much accuracy you desire.
 *
 * @todo Create the corresponding function to obtain the value of the
 * last calculation if you are using polling.
 */
UINT16 getADC(int channel)
{
	ADCSRA &= !BIT(ADATE);
	ADCSRA |= BIT(ADSC);
	while (ADCSRA & BIT(ADSC))
		;
	return getADCValue();
}

UINT16 getADCValue()
{
	if (ADMUX & BIT(ADLAR)) //Left adjusted
	{
		UINT16 val = ADCH << 2;
		val |= (ADCL & 0b11000000) >> 6;
		return val;
	}
	else //right adjusted
	{
		UINT16 val = (ADCH & 0b00000011) << 8;
		val |= ADCL;
		return val;
	}
}
/**
 * @brief Change the channel the ADC is sampling if using interrupts.
 *
 * @param channel  The ADC channel to switch to.
 *
 * @todo Create a way to switch ADC channels if you are using interrupts.
 */
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
		break;
	}
}
