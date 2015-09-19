/**
 * @file pot.h
 *
 * @brief The header file and function prototypes for the potentiometers.
 * @details  Use these functions to read the values from the pots.
 * @author Eric Willcox
 * @date August 17 2013
 */

#ifndef POT_H_
#define POT_H_

#include "RBELib.h"

typedef struct
{
	int DEG_0;
	int DEG_90;
	int DEG_180;
} potCalibration;

typedef struct
{
	int min;
	int adcChannel;
	double scaler;
} potentiometer;

potentiometer pots[2];

/**
 * Initalize the potentiometer and calculate the
 * line of best fit for the calibration data.
 *
 * @param pot The potentiometer to init and store.
 * @param channel The ADC channel that the pot is connected to.
 * @param calib The calibration data.
 */
void initPot(int pot, int channel, potCalibration calib);

/** 
 * @brief Find the angle of the given potentiometer.
 * @param  pot The pot to check.
 * @param  calib The calibration for the potentiometer.
 * @return angle Angle of the potentiometer.
 */
int potAngle(int pot);
float potAngleFloat(int pot);

/** 
 * @brief Find the voltage value of the given potentiometer.
 * @param  pot The pot to get the value of.
 * @return volts Voltage of  potentiometer.
 */
int potVolts(int pot);

#endif /* PERIPH_H_ */
