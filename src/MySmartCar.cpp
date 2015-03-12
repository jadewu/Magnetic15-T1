/*
 * mySmartCar.cpp
 *
 *  Created on: Dec 29, 2014
 *      Author: Peter
 */

#include <cstring>
#include <vector>
#include <cassert>

#include <libsc/config.h>
#include <stdlib.h>
#include <libsc/k60/system.h>

#include "MySmartCar.h"
#include "MyMotor.h"
#include "MyServo.h"
#include "MyConfig.h"
#include "MyVar.h"

using namespace libsc::k60;
using namespace libbase::k60;
using namespace std;

#define	inRange(n, v, x) ((v > x)? x : ((v < n)? n : v))
#define outRangeOf(v, ov, r) ((v - ov < -r || v - ov > r)? v : 0)
#define abs(v) ((v > 0)? v : -v)

Byte getLedCount(void)
{
#if (LIBSC_USE_LED == 1)
	return 1;
#elif (LIBSC_USE_LED == 2)
	return 2;
#elif (LIBSC_USE_LED == 3)
	return 3;
#elif (LIBSC_USE_LED == 4)
	return 4;
#else
	assert(false);
#endif
}

MySmartCar::MySmartCar(MyConfig &config, MyVar &vars)
:
	myLoop(),
	m_powerMode(&config.MySmartCarPowerMode),
	m_turningMode(&config.MySmartCarTurningMode),
	myLeds(getLeds()),
	myMotor(config, vars, myLoop),
	myServo(config, vars, myLoop)
{
	reset();
//	myUart.EnableRx(&ExecuteCommand);
}
void MySmartCar::startMainLoop(void)
{
	myLoop.start();
}

vector<Led> MySmartCar::getLeds()
{
	vector<Led> newLeds;
	for (Byte i = 0; i < getLedCount(); i++)
		newLeds.push_back(Led({i}));
	return newLeds;
}

void MySmartCar::reset(void)
{
	myMotor.reset();
	myServo.reset();
}
