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
void initADC(int channel)
{
	// For these bits, see page 249 of the guide
	ADEN = 0b1;				// Set the ADC Enable bit (ADEN) to 1
	ADSC = 0b1;				// Set the ADC start conversion bit (ADSC) to 1
	ADIE = 0b1;				// Sets the ADC to use it's interrupt flag (ADIF) (ADC Interrupt Enable)
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
unsigned short getADC(int channel);

/**
 * @brief Change the channel the ADC is sampling if using interrupts.
 *
 * @param channel  The ADC channel to switch to.
 *
 * @todo Create a way to switch ADC channels if you are using interrupts.
 */
void changeADC(int channel);
