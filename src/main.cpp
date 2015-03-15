/*
 * main.cpp
 *
 *  Created on: Dec 29, 2014
 *      Author: Peter
 */

#include <map>
#include <array>
#include <cstring>
#include <cstdio>
#include <queue>
#include <cstdlib>
#include <cmath>

#include <libsc/k60/battery_meter.h>
#include <libbase/k60/mcg.h>
#include <libsc/k60/system.h>
#include <libutil/remote_var_manager.h>
#include <libbase/k60/rand_generator.h>
#include <libsc/k60/lcd_console.h>
#include <libutil/kalman_filter.h>
#include <libbase/k60/gpio.h>

#include "MySmartCar.h"
#include "PIDhandler.h"
#include "MyConfig.h"
#include "MyVarManager.h"
#include "MyVar.h"
#include "MySmartCar.h"

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

MyConfig globalConfig;
MyVar globalVars;
MySmartCar myCar(globalConfig, globalVars);

int main()
{
	myCar.myVarMng.addWatchedVar(globalVars.MagSenSDLeft, "asd");
	myCar.myVarMng.addWatchedVar(globalVars.MagSenSDRight, "asd");
	myCar.myVarMng.addWatchedVar(globalVars.speed, "asd");
	myCar.myVarMng.Init();

	myCar.startMainLoop();
}
