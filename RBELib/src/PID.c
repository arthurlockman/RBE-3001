/*
 * PID.c
 *
 *  Created on: Sep 10, 2015
 *      Author: Rachael
 */

#include <RBELib/RBELib.h>
#include <RBELib/PID.h>

void setConst(char link, float Kp, float Ki, float Kd)
{
	if (link == 'L')
	{
		//set the lower link's PID constants
		pidConsts.Kp_L = Kp;
		pidConsts.Ki_L = Ki;
		pidConsts.Kd_L = Kd;
	} else if (link == 'H')
	{
		//set the higher link's PID constants
		pidConsts.Kp_H = Kp;
		pidConsts.Ki_H = Ki;
		pidConsts.Kd_H = Kd;
	}

}


signed int calcPID(char link, int setPoint, int actPos)
{
	int er = setPoint - actPos;

	int erPrev;
	int erPrevPrev;
	float Uk;

	float Td;
	float Ti;
	float Kp;

	// pull down relevant PID Symbols and previous values, calculate T values
	if (link == 'H')
	{
		Td = pidConsts.Kd_H / pidConsts.Kp_H;
		Ti = pidConsts.Kp_H / pidConsts.Ki_H;
		Kp = pidConsts.Kp_H;
		Uk = pidPrevs.Uk_H;
		erPrev = pidPrevs.Er_H;
		erPrevPrev = pidPrevs.Er_H_prev;
	} else
	{
		Td = pidConsts.Kd_L / pidConsts.Kp_L;
		Ti = pidConsts.Kp_L / pidConsts.Ki_L;
		Kp = pidConsts.Kp_L;
		Uk = pidPrevs.Uk_L;
		erPrev = pidPrevs.Er_L;
		erPrevPrev = pidPrevs.Er_L_prev;
	}

	Uk += Kp * ((1+ 1/Ti + Td/1)*er - (2*Td + 1)*erPrev + Td*erPrevPrev);

	//store the values as previous values for next loop
	if (link == 'H')
	{
		pidPrevs.Er_H_prev = pidPrevs.Er_H;
		pidPrevs.Er_H = er;
		pidPrevs.Uk_H = Uk;
	} else
	{
		pidPrevs.Er_L_prev = pidPrevs.Er_L;
		pidPrevs.Er_L = er;
		pidPrevs.Uk_L = Uk;
	}

	return Uk;
}
