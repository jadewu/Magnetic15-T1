/*
 * PIDhandler.cpp
 *
 *  Created on: Jan 15, 2015
 *      Author: Peter
 */

#include "PIDhandler.h"

#include <cstring>

#include <libsc/k60/system.h>

using namespace libsc::k60;

PIDhandler::PIDhandler(float *ref, float *kp, float *ki, float *kd)
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

float PIDhandler::getKp(void)
{
	return *Kp;
}

float PIDhandler::getKi(void)
{
	return *Ki;
}

float PIDhandler::getKd(void)
{
	return *Kd;
}

void PIDhandler::reset()
{
	eSum = 0;
	lastError = 0;
}

float PIDhandler::updatePID(float val, Timer::TimerInt dt)
{
	float error = *reference - val;
	float dE = (error - lastError) / dt;
	eSum += error * dt;
	lastError = error;

	return (float)(*Kp * error + *Ki * dE + *Kd * eSum);
}
