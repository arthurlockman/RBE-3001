/**
 * @file timer.h
 *
 * @brief The header file and function prototypes for Timers 0-2.
 *
 * @author Justin Barrett
 * @date August 25, 2011
 *
 * @author Eric Willcox
 * @date August 20, 2013
 */

#ifndef TIMER_H_
#define TIMER_H_
/**
 * @brief Timer normal mode.
 */
#define NORMAL 0
/**
 * @brief Timer Clear Timer on Compare (CTC) mode.
 */
#define CTC	1

#define SCALE1 0b001
#define SCALE8 0b010
#define SCALE64 0b011
#define SCALE256 0b100
#define SCALE1024 0b101

#define CLK_IO 8000000

/**
 * Starts timer 0 running a 1ms timer whcih triggers the interrupt
 * on overflow.
 */
void configureMsTimer();

/**
 * @brief Initializes the specified timer in the specified mode. This
 * header file provides constants for NORMAL operation mode and
 * CTC operation mode, however there are many more modes of operation
 * for the Timers that can be experimented with.
 *
 * @param timer The number of the timer to be initialized (0-2).
 * @param mode The mode to initialize the specified timer in.
 * @param comp Only used in CTC mode. The value that the
 * @param prescale used to set the prescale
 * timer counts to before it triggers an interrupt and resets.
 *
 * @todo Create a function that initializes the desired timer in a given mode and set the compare value
 * (as appropriate).
 */
void setTimerPeriod(int timer, double freq);

/**
 * @brief sets the given timer's period to the given period in micro seconds
 *
 * @param timer The number of the timer to be initialized (0-2).
 * @param period used to set the period
 * timer counts to before it triggers an interrupt and resets.
 *
 * @todo Create a function that initializes the desired timer in a given mode and set the compare value
 * (as appropriate).
 */
void setPrescale(int timer, int prescale);

/**
 * @brief sets the given timer's prescale to the given prescale
 *
 * @param timer The number of the timer to be initialized (0-2).
 * @param prescale used to set the prescale
 *
 * @todo Create a function that initializes the desired timer in a given mode and set the compare value
 * (as appropriate).
 */
void initTimer(int timer, int mode, unsigned int comp);

/**
 * @brief Initializes the specified timer in the specified mode. This
 * header file provides constants for NORMAL operation mode and
 * CTC operation mode, however there are many more modes of operation
 * for the Timers that can be experimented with.
 *
 * @param timer The number of the timer to be initialized (0-2).
 * @param mode The mode to initialize the specified timer in.
 * @param comp Only used in CTC mode. The value that the
 * timer counts to before it triggers an interrupt and resets.
 *
 * @todo Create a function that initializes the desired timer in a given mode and set the compare value
 * (as appropriate).
 */
void initTimer(int timer, int mode, unsigned int comp);

/**
 * @brief Only used when the specified timer is in CTC mode. Changes
 * the value of the comparison register to the new specified value.
 *
 * @param timer The timer comparison value to change (0-2).
 * @param comp The value to set the comparison register to.
 *
 * @todo Create a function that will set a new compare value for the given timer.
 */
void setCompValue(unsigned char timer, unsigned int comp);

#endif /* TIMER_H_ */
