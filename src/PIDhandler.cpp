/*
 * PIDhandler.cpp
 *
 *  Created on: Jan 15, 2015
 *      Author: Peter
 */

#include "PIDhandler.h"
#include <cstring>

using namespace libsc::k60;

PIDhandler::PIDhandler(int32_t ref, float kp, float ki, float kd)
:
	reference(ref),
	Kp(kp),
	Ki(ki),
	Kd(kd),
	eSum(0),
	lastError(0)
{
	System::Init();
}

void PIDhandler::reset()
{
	eSum = 0;
}

float PIDhandler::updatePID(float val, uint16_t dt)
{
	float error = reference - val;
	float de = (error - lastError) / dt;
	eSum += error;
	lastError = error;

	return (float)(Kp * error + Ki * dt + Kd * de);
}
