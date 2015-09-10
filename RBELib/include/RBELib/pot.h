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

typedef struct
{
	int DEG_0;
	int DEG_90;
	int DEG_180;
} potCalibration;

long analogRange;
potCalibration calibration;

const potCalibration DEFAULT_POT_CALIBRATION = {250, 625, 975};

/**
 * Initalize the potentiometer and calculate the
 * line of best fit for the calibration data.
 *
 * @param calib The calibration data.
 */
void initPot(potCalibration calib);

/** 
 * @brief Find the angle of the given potentiometer.
 * @param  pot The pot to check.
 * @return angle Angle of the potentiometer.
 *
 * @todo Calculate the angle using the ADC reading.
 */
int potAngle(long pot);

/** 
 * @brief Find the voltage value of the given potentiometer.
 * @param  pot The pot to get the value of.
 * @return volts Voltage of  potentiometer.
 *
 * @todo Convert the ADC value into a voltage in mV (so no floats needed).
 */
int potVolts(long pot);

#endif /* PERIPH_H_ */
