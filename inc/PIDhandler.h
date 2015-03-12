/*
 * PIDhandler.h
 *
 *  Created on: Jan 15, 2015
 *      Author: Peter
 */

#include <libsc/k60/sys_tick_timer.h>

#pragma once

#define MAX(a, b) ((a > b)? a : b)

using namespace libsc::k60;

class PIDhandler
{
public:

	explicit PIDhandler(float *ref, float *kp, float *ki, float *kd);
	float updatePID(float val, Timer::TimerInt dt);

	float getKp(void);
	float getKi(void);
	float getKd(void);

	void reset();

private:

	float *reference;
	float *Kp;
	float *Ki;
	float *Kd;
	float eSum;
	float lastError;
};
