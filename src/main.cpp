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
using namespace std;

int main()
{
	System::Init();
	MySmartCar myCar;

	LcdConsole::Config lcdConfig;
	lcdConfig.lcd = &myCar.myLcd;
	LcdConsole myConsole(lcdConfig);

//	uint16_t adcReading = 0;
//	float adcReadingF = 0.0f;

//	myCar.myVarMng.addWatchedVar(&adcReading, "uint16_t", 2, "1");
//	myCar.myVarMng.addWatchedVar(&adcReadingF, "float", 4, "2");
//	myCar.myVarMng.Init(&myCar.ExecuteCommand);
//	myCar.myVarMng.Init();

//	System::DelayMs(3000);
//	myCar.myLcd.Clear(0);

	uint16_t i = 0;
	uint16_t n = 0;
	char *buf = new char[125] { 0 };

	while (true)
	{
		myConsole.Clear(true);
		n = sprintf(buf, "pTimer \nCount: %d", i++);
		myConsole.WriteBuffer(buf, n);

//		adcReading = myCar.myMagSensor.GetResult() & ~3;
//		adcReadingF = myCar.myMagSensor.GetResultF();
//		myCar.myVarMng.sendWatchData();

		System::DelayMs(1000);
	}

	while (true);

	return 0;
}
