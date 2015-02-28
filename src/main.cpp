/*
 * main.cpp
 *
 *  Created on: Dec 29, 2014
 *      Author: Peter
 */

#include <libbase/k60/mcg.h>
#include <libsc/k60/system.h>
#include <MySmartCar.h>
#include <libutil/remote_var_manager.h>
#include <libbase/k60/rand_generator.h>
#include <libsc/k60/lcd_console.h>
#include <libutil/kalman_filter.h>
#include <libbase/k60/gpio.h>

#include "PIDhandler.h"

#include <map>
#include <array>
#include <cstring>
#include <cstdio>
#include <queue>
#include <cstdlib>
#include <cmath>

namespace libbase
{
	namespace k60
	{

		Mcg::Config Mcg::GetMcgConfig()
		{
			Mcg::Config config;
			config.external_oscillator_khz = 50000;
			config.core_clock_khz = 150000;
			return config;
		}

	}
}

using namespace libsc::k60;
using namespace libbase::k60;
using namespace libutil;
using namespace std;


MySmartCar myCar;

#define timeInterval 	2
#define standardVoltage	0.8f
#define midDegree		900

float scaleDiff = 1.265;
float shiftValue = 0.65f;

float adcToAngleRatio = 10000;

float baseFilterQ = 0.001f;
float baseFilterR = 1.120019f;

float lpQ = 0.001f + shiftValue;
float lpR = 1.120019f + shiftValue;

float adcReadingL = 0;
float adcReadingR = 0;
float adcRealReadingL = 0;
float adcRealReadingR = 0;
float servoAngle = 0;

KalmanFilter filterL(baseFilterQ, baseFilterR, 0.5f, 0.5f);
KalmanFilter filterR(baseFilterQ, baseFilterR, 0.5f, 0.5f);
KalmanFilter filterAngle(lpQ - shiftValue, lpR - shiftValue, 0.5f, 0.5f);

PIDhandler turningPID(0.0f, 0.7f, 0.0f, 0.0f);

void kfTestingFunction(const Byte *bytes, const size_t size)
{
	switch (bytes[0])
	{
	case '4':
		if (lpQ + 0.001f <= 1.0f + shiftValue)
			lpQ += 0.001f;
		filterAngle.SetQ(lpQ - shiftValue);
		break;
	case '1':
		if (lpQ - 0.001f >= 0.0f + shiftValue)
			lpQ -= 0.001f;
		filterAngle.SetQ(lpQ - shiftValue);
		break;
	case '5':
		if (lpR + 0.001f <= 1.0f + shiftValue)
			lpR += 0.001f;
		filterAngle.SetR(lpR - shiftValue);
		break;
	case '2':
		if (lpR - 0.001f >= 0.0f + shiftValue)
			lpR -= 0.001f;
		filterAngle.SetR(lpR - shiftValue);
		break;
	case 'e':
		myCar.doBlink(0);
		break;

	case 'i':
		myCar.setSpeed(200);
//		adcToAngleRatio += 1000;
		break;
	case 'k':
		myCar.setSpeed(0);
//		if (adcToAngleRatio >= 1000)
//			adcToAngleRatio -= 1000;
		break;

	case 'o':
		scaleDiff += 0.005f;
		break;
	case 'l':
		if (scaleDiff >= 0.05f)
		scaleDiff -= 0.005f;
		break;

	case 'b':
		myCar.turn(0);
	}
}

void FilterProc()
{
	adcRealReadingL = myCar.myMagSensor0.GetResultF();
	adcReadingL = filterL.Filter(adcRealReadingL);
	adcRealReadingR = myCar.myMagSensor1.GetResultF() * scaleDiff;
	adcReadingR = filterR.Filter(adcRealReadingR);
	servoAngle = filterAngle.Filter((float)(adcReadingR - adcReadingL));

}

void applyResult()
{
	myCar.turn((int16_t)(servoAngle * 5000));
}

void sendData()
{
	myCar.myVarMng.sendWatchData();
	myCar.doBlink(0);
}

int main()
{
//	LcdConsole::Config lcdConfig;
//	lcdConfig.lcd = &myCar.myLcd;
//	LcdConsole myConsole(lcdConfig);

	myCar.myVarMng.addWatchedVar(&adcReadingL, "0");
	myCar.myVarMng.addWatchedVar(&adcReadingR, "0");

//	myCar.myVarMng.addWatchedVar(&servoAngle, "2");
//
//	myCar.myVarMng.addWatchedVar(&adcToAngleRatio, "2");

//	myCar.myVarMng.addWatchedVar(&lpQ, "3");
	myCar.myVarMng.addWatchedVar(&scaleDiff, "3");
//	myCar.myVarMng.addWatchedVar(&lpQ, "2");
//	myCar.myVarMng.addWatchedVar(&lpR, "3");

	myCar.myVarMng.Init(&kfTestingFunction);

	myCar.myLoop.addFunctionToLoop(&FilterProc, 2, LOOP_EVERYTIME);
	myCar.myLoop.addFunctionToLoop(&applyResult, LOOP_IMMEDIATELY, LOOP_EVERYTIME);
	myCar.myLoop.addFunctionToLoop(&sendData, LOOP_IMMEDIATELY, 5);

	myCar.myLoop.start();

	return 0;
}

/*********************************************************************/

/*
float curKp = 0.02f, curKi = 0.0f, curKd = 0.0f;

float step = 0.005f;

bool isStart = false;
bool isFirstTime = true;

PIDhandler SpeedPID(600, curKp, curKi, curKd);

void pidTestingFunction(const Byte *bytes, const size_t size)
{
	switch (bytes[0])
	{
	case '4':
		curKp += step;
		SpeedPID.setKp(curKp);
		break;
	case '1':
		if (curKp - step >= 0.0f)
			curKp -= step;
		SpeedPID.setKp(curKp);
		break;
	case '5':
		curKi += step;
		SpeedPID.setKi(curKi);
		break;
	case '2':
		if (curKi - step >= 0.0f)
			curKi -= step;
		SpeedPID.setKi(curKi);
		break;
	case '6':
		curKd += step;
		SpeedPID.setKd(curKd);
		break;
	case '3':
		if (curKd - step >= 0.0f)
			curKd -= step;
		SpeedPID.setKd(curKd);
		break;
	case '0':
		isStart = !isStart;
		break;
	}
}

int main()
{
	int32_t encoderReading = 0;
	uint16_t pidResult = 0.0f;

	myCar.myVarMng.addWatchedVar(&encoderReading, "a");
	myCar.myVarMng.addWatchedVar(&pidResult, "b");

	myCar.myVarMng.addWatchedVar(&curKp, "c");
	myCar.myVarMng.addWatchedVar(&curKi, "d");
	myCar.myVarMng.addWatchedVar(&curKd, "e");

	myCar.myVarMng.Init(&pidTestingFunction);


	Timer::TimerInt lastTime = System::Time();

	myCar.setSpeed(250);

	while (true)
	{
		while (isStart)
		{
			if (isFirstTime)
			{
				myCar.setSpeed(250);
				isFirstTime = false;
			}

			if (System::Time() >= lastTime + 20)
			{
				myCar.myEncoder.Update();
				encoderReading = myCar.myEncoder.GetCount();
				pidResult = (uint16_t)SpeedPID.updatePID((float)encoderReading, 20);
				myCar.setSpeed(pidResult);

				myCar.myVarMng.sendWatchData();

				lastTime = System::Time();
			}
		}

		myCar.setSpeed(0);
		isFirstTime = true;
	}
}
*/
