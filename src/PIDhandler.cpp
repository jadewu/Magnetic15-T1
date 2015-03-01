/*
 * PIDhandler.cpp
 *
 *  Created on: Jan 15, 2015
 *      Author: Peter
 */

#include "PIDhandler.h"
#include <cstring>

using namespace libsc::k60;

PIDhandler::PIDhandler(float *ref, float kp, float ki, float kd)
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

PIDhandler::PIDhandler(float ref, float kp, float ki, float kd)
:
	reference(new float(ref)),
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
	Kp = MAX(0, kp);
}
void PIDhandler::setKi(float ki)
{
	Ki = MAX(0, ki);
}
void PIDhandler::setKd(float kd)
{
	Kd = MAX(0, kd);
}

float PIDhandler::getKp(void)
{
	return Kp;
}

float PIDhandler::getKi(void)
{
	return Ki;
}

float PIDhandler::getKd(void)
{
	return Kd;
}

void PIDhandler::reset()
{
	eSum = 0;
}

float PIDhandler::updatePID(float val, uint32_t dt)
{
	float error = *reference - val;
	float dE = (error - lastError) / dt;
	eSum += error * dt;
	lastError = error;

	return (float)(Kp * error + Ki * dE + Kd * eSum);
}
