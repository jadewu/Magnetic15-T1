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

void PIDhandler::setKp(float kp)
{
	Kp = kp;
}
void PIDhandler::setKi(float ki)
{
	Ki = ki;
}
void PIDhandler::setKd(float kd)
{
	Kd = kd;
}

void PIDhandler::reset()
{
	eSum = 0;
}

float PIDhandler::updatePID(float val, uint32_t dt)
{
	float error = reference - val;
	float dE = (error - lastError) / dt;
	eSum += error * dt;
	lastError = error;

	return (float)(Kp * error + Ki * dE + Kd * eSum);
}
