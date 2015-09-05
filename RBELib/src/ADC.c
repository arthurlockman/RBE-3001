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
unsigned short getADC(int channel)
{
	ADCSRA &= !BITS(ADATE);
	ADCSRA |= BITS(ADSC);
	while (ADCSRA & BITS(ADSC))
		;
	if (ADMUX & BITS(ADLAR)) //Left adjusted
	{

	}
	else //right adjusted
	{

	}
}

/**
 * @brief Change the channel the ADC is sampling if using interrupts.
 *
 * @param channel  The ADC channel to switch to.
 *
 * @todo Create a way to switch ADC channels if you are using interrupts.
 */
void changeADC(int channel);
