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
	float Td = Kp / Ki;
	float Ti = Kp / Ki;
	float dT = 0.01;
	float kPropTerm = (1.0 + (dT / Ti) + (Td / dT));
	float kIntTerm = (-1.0 - (2 * Td / dT));
	float kDiffTerm = Td / dT;
	if (link == 'L')
	{
		//set the lower link's PID constants
		pidConsts.Kp_L = Kp;
		pidConsts.Ki_L = Ki;
		pidConsts.Kd_L = Kd;
		pidConsts.KPropTerm_L = kPropTerm;
		pidConsts.KIntTerm_L = kIntTerm;
		pidConsts.KDiffTerm_L = kDiffTerm;
	} else if (link == 'U')
	{
		//set the higher link's PID constants
		pidConsts.Kp_H = Kp;
		pidConsts.Ki_H = Ki;
		pidConsts.Kd_H = Kd;
		pidConsts.KPropTerm_H = kPropTerm;
		pidConsts.KIntTerm_H = kIntTerm;
		pidConsts.KDiffTerm_H = kDiffTerm;
	}
}


signed int calcPID(char link, int setPoint, int actPos)
{
	int er = setPoint - actPos;

	int erPrev;
	int erPrevPrev;
	float Uk, Kp, kProp, kInt, kDiff;

	// pull down relevant PID Symbols and previous values, calculate T values
	if (link == 'U')
	{
		Kp = pidConsts.Kp_H;
		Uk = pidPrevs.Uk_H;
		erPrev = pidPrevs.Er_H;
		erPrevPrev = pidPrevs.Er_H_prev;
		kProp = pidConsts.KPropTerm_H;
		kInt = pidConsts.KIntTerm_H;
		kDiff = pidConsts.KDiffTerm_H;
	} else if (link == 'L')
	{
		Kp = pidConsts.Kp_L;
		Uk = pidPrevs.Uk_L;
		erPrev = pidPrevs.Er_L;
		erPrevPrev = pidPrevs.Er_L_prev;
		kProp = pidConsts.KPropTerm_L;
		kInt = pidConsts.KIntTerm_L;
		kDiff = pidConsts.KDiffTerm_L;
	}

	Uk += Kp * (kDiff * er + kInt * erPrev + kDiff * erPrevPrev);

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
