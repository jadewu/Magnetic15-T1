/*
 * main.cpp
 *
 *  Created on: Dec 29, 2014
 *      Author: Peter
 */

#include <libsc/k60/battery_meter.h>
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


MySmartCar myCar(MySmartCar::SmartCarMode::kNormalMode);

void kfTestingFunction(const Byte *bytes, const size_t size)
{
	switch (bytes[0])
	{
	case '4':
		if (lpQ + 0.0001 <= 1.0f)
			lpQ += 0.0001;
		filterAngle.SetQ(lpQ);
		break;
	case '1':
		if (lpQ - 0.0001 >= 0.0f)
			lpQ -= 0.0001;
		filterAngle.SetQ(lpQ);
		break;
	case '5':
		if (lpR + 0.001 <= 1.0f)
			lpR += 0.001;
		filterAngle.SetR(lpR);
		break;
	case '2':
		if (lpR - 0.001 >= 0.0f)
			lpR -= 0.001;
		filterAngle.SetR(lpR);
		break;
	case '6':
		if (baseFilterQ + 0.00001f <= 1.0f)
			baseFilterQ += 0.00001f;
		filterL.SetQ(baseFilterQ);
		filterR.SetQ(baseFilterQ);
		break;
	case '3':
		if (baseFilterQ - 0.00001f >= 0.0f)
			baseFilterQ -= 0.00001f;
		filterL.SetQ(baseFilterQ);
		filterR.SetQ(baseFilterQ);
		break;
	case '+':
		if (baseFilterR + 0.05f <= 2.0f)
			baseFilterR += 0.05f;
		filterL.SetR(baseFilterR);
		filterR.SetR(baseFilterR);
		break;
	case '-':
		if (baseFilterR - 0.05f >= 0.0f)
			baseFilterR -= 0.05f;
		filterL.SetR(baseFilterR);
		filterR.SetR(baseFilterR);
		break;

	case 'p':
		turningPID.setKp(turningPID.getKp() + 50);
		break;
	case 'P':
		turningPID.setKp(turningPID.getKp() - 50);
		break;
	case 'i':
		turningPID.setKi(turningPID.getKi() + 50);
		break;
	case 'I':
		turningPID.setKi(turningPID.getKi() - 50);
		break;
	case 'd':
		turningPID.setKd(turningPID.getKd() + 0.005);
		break;
	case 'D':
		turningPID.setKd(turningPID.getKd() - 0.005);
		break;

	case 'e':
//		myCar.doBlink(0);
		break;

	case 'u':
//		myCar.setSpeed(160);
//		adcToAngleRatio += 1000;
		break;
	case 'j':
//		myCar.setSpeed(0);
//		if (adcToAngleRatio >= 1000)
//			adcToAngleRatio -= 1000;
		break;
	case 'm':
//		myCar.setSpeed(-160);
		break;

	case 'h':
		scaleDiff += 0.005f;
		break;
	case 'n':
		if (scaleDiff >= 0.05f)
		scaleDiff -= 0.005f;
		break;

//	case 'b':
//		myCar.turn(0);
	}
}

void FilterProc()
{
//	adcRealReadingL = myCar.myMagSensor0.GetResultF();
	adcReadingL = filterL.Filter(adcRealReadingL);
//	adcRealReadingR = myCar.myMagSensor1.GetResultF() * scaleDiff;
	adcReadingR = filterR.Filter(adcRealReadingR);
	retServoAngle = turningPID.updatePID(adcReadingL - adcReadingR, 1);
}

void applyResult()
{
	if (retServoAngle)
		lastTurningDirection = 1;
	else
		lastTurningDirection = 2;

	if (abs(retServoAngle) < 180)
		retServoAngle = 0;

//	if(adcReadingL < 0.16 && adcReadingR < 0.16f)
//	{
//		if (lastTurningDirection == 1)
//			myCar.turn(-MAX_SERVO_TURNING_DEGREE);
//		else if (lastTurningDirection == 2)
//			myCar.turn(MAX_SERVO_TURNING_DEGREE);
//	}
//	else
//		myCar.turn(retServoAngle);
}

void sendData()
{
//	lpP = turningPID.getKp();
//	lpD = turningPID.getKd();
	batteryVoltage = batteryMng.GetVoltage();
//	myCar.myVarMng.sendWatchData();
//	myCar.doBlink(0);
}

int main()
{
//	LcdConsole::Config lcdConfig;
//	lcdConfig.lcd = &myCar.myLcd;
//	LcdConsole myConsole(lcdConfig);

//	myCar.myVarMng.addWatchedVar(&adcRealReadingL, "0");
//	myCar.myVarMng.addWatchedVar(&adcRealReadingR, "0");
//
//	myCar.myVarMng.addWatchedVar(&adcReadingL, "0");
//	myCar.myVarMng.addWatchedVar(&adcReadingR, "0");
//
//	myCar.myVarMng.addWatchedVar(&baseFilterQ, "3");
//	myCar.myVarMng.addWatchedVar(&baseFilterR, "3");
//
//	myCar.myVarMng.addWatchedVar(&lpP, "0");
//	myCar.myVarMng.addWatchedVar(&lpD, "0");
////
//	myCar.myVarMng.addWatchedVar(&retServoAngle, "2");
//
//
//	myCar.myVarMng.addWatchedVar(&scaleDiff, "3");
//
//	myCar.myVarMng.addWatchedVar(&lpQ, "2");
//	myCar.myVarMng.addWatchedVar(&lpR, "3");

//	myCar.myVarMng.addWatchedVar(&batteryVoltage, "4");

//	myCar.myVarMng.Init(&kfTestingFunction);
//
//	myCar.myLoop.addFunctionToLoop(&FilterProc, 1, LOOP_EVERYTIME);
//	myCar.myLoop.addFunctionToLoop(&applyResult, LOOP_IMMEDIATELY, LOOP_EVERYTIME);
//	myCar.myLoop.addFunctionToLoop(&sendData, LOOP_IMMEDIATELY, 10);

//	System::DelayMs(2000);
//	myCar.setSpeed(160);

//	myCar.myLoop.start();

	while (true);

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
