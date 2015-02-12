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

	float f = 0.0f;
	uint8_t rf = 0;
	float sf = 0.0f;

	myCar.myVarMng.addWatchedVar(&f, "float", sizeof(float), "TestValue1");
	myCar.myVarMng.addWatchedVar(&rf, "uint8_t", sizeof(uint8_t), "TestValue2");
	myCar.myVarMng.addWatchedVar(&sf, "float", sizeof(float), "TestValue3");
//	myCar.myVarMng.Init(&myCar.ExecuteCommand);
	myCar.myVarMng.Init();

	while (true)
	{
		for (float i = 0.0f; i < 100.0f; i += 0.5f)
		{
			f = i;
			rf = 100 - f;
			sf = sin(f) + 50.0f;
			myCar.myVarMng.sendWatchData();
			System::DelayMs(20);
		}
	}

	while (true);

	return 0;
}
