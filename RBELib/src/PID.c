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
	float Td = Kd / Kp;
	float dT = 0.01;
	float kPropTerm = (1.0 + (dT * (Ki / Kp)) + (Td / dT));
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
		pidPrevs.Uk_L = 0;
		pidPrevs.Er_L_prev = 0;
		pidPrevs.Er_L = 0;
	} else if (link == 'U')
	{
		//set the higher link's PID constants
		pidConsts.Kp_H = Kp;
		pidConsts.Ki_H = Ki;
		pidConsts.Kd_H = Kd;
		pidConsts.KPropTerm_H = kPropTerm;
		pidConsts.KIntTerm_H = kIntTerm;
		pidConsts.KDiffTerm_H = kDiffTerm;
		pidPrevs.Uk_H = 0;
		pidPrevs.Er_H_prev = 0;
		pidPrevs.Er_H = 0;
	}
}


long calcPID(char link, int actPos)
{
	int setPoint;
	if (link == 'U') setPoint = upperLinkSetpoint;
	else if (link == 'L') setPoint = lowerLinkSetpoint;
	int er = setPoint - actPos;
	if(er <= 1 && er >= -1)
	{
		return 0;
	}

	int erPrev;
	int erPrevPrev;
	long Uk;
	float Kp, kProp, kInt, kDiff;

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

	Uk += Kp * (kProp * er + kInt * erPrev + kDiff * erPrevPrev);
//	printf("er: %d, erPrev: %d, erPrevPrev: %d, ", er, erPrev, erPrevPrev);
//	printf("kProp: %f, kInt: %f, kDiff: %f, ", kProp, kInt, kDiff);
//	printf("actual: %d, set: %d, err: %d, Uk: %ld\n\r", actPos, setPoint, er, Uk);

	//store the values as previous values for next loop
	if (link == 'U')
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

void setSetpoint(char link, int setPoint)
{
	if (link == 'U') upperLinkSetpoint = setPoint;
	else if (link == 'L') lowerLinkSetpoint = setPoint;
}

int getSetpoint(char link)
{
	if (link == 'U') return upperLinkSetpoint;
	else if (link == 'L') return lowerLinkSetpoint;
	return 0;
}
