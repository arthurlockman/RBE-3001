/**
 * @file motors.h
 *
 * @brief Motor driving functions for the arm.
 *
 * @author Eric Willcox
 * @date July 9, 2014
 */

#ifndef MOTORS_H_
#define MOTORS_H_

#define IN_RANGE(val, upper, lower) (val >= lower && val <= upper)
#define abs(val) ((val < 0) ? -val : val)

/**
 * @brief Helper function to stop the motors on the arm.
 *
 * @todo Create way to stop the motors using the DAC.
 */
void stopMotors();
void calcXY(int* pos);

/**
 * @brief Drive the arm to a desired angle
 *
 * @param lowerTheta The desired angle for the lower link.
 * @param upperTheta The desired angle for the upper link.
 *
 * @todo Make a way to drive the links to a desired angle.
 */
void gotoAngles(int lowerTheta, int upperTheta);

/**
 * @brief Drive the end effector of the arm to a desired X and Y position in the workspace.
 *
 * @param x The desired x position for the end effector.
 * @param y The desired y position for the end effector.
 *
 * @todo Use kinematic equations to move the end effector to the desired position.
 */
void gotoXY(int x, int y);

/**
 * @brief Drive a link (upper or lower) in a desired direction.
 *
 * @param link Which link to control.
 * @param dir Signed int representing the direction and magnitude.
 *
 * @todo Create a way to drive either link in any direction.
 */
void driveLink(int link, long dir);
void driveLinkSlow(int link, long dir);

/**
 * @brief Drive the arm to a "home" position using the potentiometers.  This
 * should be called before using the encoders and just goes to a default position.
 * Once this has been called once, you can initialize/clear the encoders.
 *
 * @todo Drive the arm to a known position using the potentiometers.
 */
void homePos();

//@TODO: Comments
int readCurrentMilliamps(int link);

void openGripper(int servoNum);

void closeGripper(int servoNum);

void gotoXYMatlab(int x, int y);

#endif /* MOTORS_H_ */
