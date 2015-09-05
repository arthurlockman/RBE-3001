/**
 * @file ADC.h
 *
 * @brief The header file and function prototypes for the ADC.
 * @details For single ended conversion, the ADC value an be found from the voltage using:
 * @f[ \frac {V_{in} * 1024}{V_{ref}} @f]
 * @author Kevin Harrington
 * @date February 11, 2010
 *
 * @author Justin Barrett
 * @date August 23, 2011
 *
 * @author Eric Willcox
 * @date August 19, 2013
 */

#ifndef ADC_H_
#define ADC_H_

/**
 * An enum to hold the different init modes for the ADC.
 * @see Page 262 of datasheet.
 */
typedef enum
{
	ADC_FREE_RUNNING = (BIT(ADTS2) & 0) | (BIT(ADTS1) & 0) | (BIT(ADTS0) & 0),
	ADC_ANALOG_COMPARATOR = (BIT(ADTS2) & 0) | (BIT(ADTS1) & 0) | BIT(ADTS0),
	ADC_EXT_INT = (BIT(ADTS2) & 0) | BIT(ADTS1) | (BIT(ADTS0) & 0),
	ADC_TIMER0_COMPARE = (BIT(ADTS2) & 0) | BIT(ADTS1) | BIT(ADTS0),
	ADC_TIMER0_OVERFLOW = BIT(ADTS2) | (BIT(ADTS1) & 0) | (BIT(ADTS0) & 0),
	ADC_TIMER1_COMPARE = BIT(ADTS2) | (BIT(ADTS1) & 0) | BIT(ADTS0),
	ADC_TIMER1_OVERFLOW = BIT(ADTS2) | BIT(ADTS1) | (BIT(ADTS0) & 0),
	ADC_TIMER1_CAPTURE = BIT(ADTS2) | BIT(ADTS1) | BIT(ADTS0)
} adcMode;

/**
 * An enum to hold the register settings for the different
 * possible voltage reference settings.
 * @see Page 258 of the datasheet.
 */
typedef enum
{
	ADC_REF_EXT = 0b00000000,
	ADC_REF_VCC = 0b01000000,
	ADC_REF_11  = 0b10000000,
	ADC_REF_256 = 0b11000000,
} adcVref;

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
void initADC(int channel, adcMode mode);

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
UINT16 getADC(int channel);

/**
 * @brief Gets the ADC value and properly left or right adjusts it
 * based on the current settings.
 *
 * @return A UINT16, the raw value of the ADC.
 */
UINT16 getADCValue();

/**
 * @brief Change the channel the ADC is sampling if using interrupts.
 *
 * @param channel  The ADC channel to switch to.
 *
 * @todo Create a way to switch ADC channels if you are using interrupts.
 */
void changeADC(int channel);

#endif /* ADC_H_ */
