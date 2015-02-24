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

#include <map>
#include <array>
#include <cstring>
#include <cstdio>
#include <queue>
#include <cstdlib>

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

float shiftValue = 0.65f;
float lpQ = 0.008f + shiftValue;
float lpR = 1.120019f + shiftValue;
float realReading = 0.0f;

MySmartCar myCar;

void kfTestingFunction(const Byte *bytes, const size_t size)
{
	switch (bytes[0])
	{
	case '4':
		if (lpQ + 0.001f <= 1.0f + shiftValue)
			lpQ += 0.001f;
		break;
	case '1':
		if (lpQ - 0.001f >= 0.0f + shiftValue)
			lpQ -= 0.001f;
		break;
	case '5':
		if (lpR + 0.001f <= 1.0f + shiftValue)
			lpR += 0.001f;
		break;
	case '2':
		if (lpR - 0.001f >= 0.0f + shiftValue)
			lpR -= 0.001f;
		break;
	case '6':
		myCar.doBlink(0);
		break;
	}
}

void changeAngle(const Byte *bytes, const size_t size)
{
	switch (bytes[0])
	{
	case 'n':
		if (myCar.myServo.GetDegree() >= 100)
			myCar.myServo.SetDegree(myCar.myServo.GetDegree() - 100);
		break;
	case 'm':
		myCar.myServo.SetDegree(myCar.myServo.GetDegree() + 100);
		break;
	}
}

int main()
{
	System::Init();

//	LcdConsole::Config lcdConfig;
//	lcdConfig.lcd = &myCar.myLcd;
//	LcdConsole myConsole(lcdConfig);

//	float adcReading = 0;
//	KalmanFilter filter(lpQ - shiftValue, lpR - shiftValue, 0.5f, 0.5f);
//
//	myCar.myVarMng.addWatchedVar(&realReading, "float", sizeof(float), "0");
//	myCar.myVarMng.addWatchedVar(&lpQ, "float", sizeof(float), "2");
//	myCar.myVarMng.addWatchedVar(&adcReading, "float", sizeof(float), "1");
//	myCar.myVarMng.addWatchedVar(&lpR, "float", sizeof(float), "3");
	float f = 0.0f;
	uint8_t i = 0;

	myCar.myVarMng.addWatchedVar(&f, "1");
	myCar.myVarMng.addWatchedVar(&i, "3");

	myCar.myVarMng.Init(&changeAngle);

//	System::DelayMs(3000);
//	myCar.myLcd.Clear(0);
//	uint16_t n = 0;
//	char *buf = new char[125] { 0 };


	while (true)
	{
//		myConsole.Clear(true);
//		n = sprintf(buf, "pTimer \nCount: %d", i++);
//		myConsole.WriteBuffer(buf, n);

//		filter.SetQ(lpQ - shiftValue);
//		filter.SetR(lpR - shiftValue);
//		realReading = myCar.myMagSensor.GetResultF();
//		adcReading = filter.Filter(realReading);
		for (f = 0.0f; f < 100.0f; f += 0.5f)
		{
			System::DelayMs(20);
			myCar.doBlink(i++ % 4);
			myCar.myVarMng.sendWatchData();
		}
//		myCar.myVarMng.sendWatchData();
//		System::DelayMs(20);
	}

	while (true);

	return 0;
}
