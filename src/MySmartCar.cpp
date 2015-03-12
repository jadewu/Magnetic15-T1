/*
 * mySmartCar.cpp
 *
 *  Created on: Dec 29, 2014
 *      Author: Peter
 */

#include <cstring>
#include <vector>

#include <libsc/config.h>
#include <stdlib.h>
#include <libsc/k60/system.h>

#include "MySmartCar.h"
#include "MyConfig.h"

using namespace libsc::k60;
using namespace std;

#define	inRange(n, v, x) ((v > x)? x : ((v < n)? n : v))
#define outRangeOf(v, ov, r) ((v - ov < -r || v - ov > r)? v : 0)
#define abs(v) ((v > 0)? v : -v)

MySmartCar::MySmartCar(MyConfig &config)
:
	m_powerMode(&config.MySmartCarPowerMode),
	myLeds(getLeds()),
{
	System::Init();
	reset();
//	myUart.EnableRx(&ExecuteCommand);
}

std::vector<Led> MySmartCar::getLeds()
{
	std::vector<Led> newLeds;
	for (Byte i = 0; i < LIBSC_USE_LED; i++)
		newLeds.push_back(Led(i));
	return newLeds;
}

void MySmartCar::reset(void)
{
}
