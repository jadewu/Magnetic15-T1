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
using namespace std;

/* Game
const size_t sSize = 8;
const Byte maxW = 16;
const Byte maxH = 20;

Byte lastNextAction = 0;
bool points[maxW][maxH] = { 0 };

Byte errorCode = 0;

struct Point
{
	Byte x;
	Byte y;
};

Point *pointOrder = new Point[maxW * maxH] { {0, 0} };
Point food;
uint16_t pointCount = 0;

void randomFood(libbase::k60::RandGenerator &randGen)
{
	do
	{
		food = { (Byte)(randGen.GetRand() % maxW), (Byte)(randGen.GetRand() % maxH) };
	}
	while (points[food.x][food.y]);
}

void paintDrawing(MySmartCar &myCar)
{
	for (Byte px = 0; px < maxW; px++)
	{
		for (Byte py = 0; py < maxH; py++)
		{
			if (points[px][py])
				myCar.myLcd.DrawPixel(px * 8, py * 8, sSize, sSize, 0);
			else
				myCar.myLcd.DrawPixel(px * 8, py * 8, sSize, sSize, -1);
		}
	}
	myCar.myLcd.DrawPixel(food.x * 8, food.y * 8, sSize, sSize, 0x1F);
}

bool addPoint(MySmartCar &myCar, int8_t xOffset, int8_t yOffset, libbase::k60::RandGenerator &randGen)
{
	if (!((xOffset < 0 && pointOrder[pointCount - 1].x == 0) ||
		(xOffset > 0 && pointOrder[pointCount - 1].x == maxW)) &&
		!((yOffset < 0 && pointOrder[pointCount - 1].y == 0) ||
		(yOffset > 0 && pointOrder[pointCount - 1].y == maxH - 1)))
	{
		if (points[pointOrder[pointCount - 1].x + xOffset][pointOrder[pointCount - 1].y + yOffset])
		{
			errorCode = 2;
			goto hitBody;
		}
		if (pointOrder[pointCount - 1].x + xOffset == food.x && pointOrder[pointCount - 1].y + yOffset == food.y)
		{
			myCar.extendFlag = true;
			randomFood(randGen);
		}
		pointOrder[pointCount].y = pointOrder[pointCount - 1].y + yOffset;
		pointOrder[pointCount].x = pointOrder[pointCount - 1].x + xOffset;
		points[pointOrder[pointCount - 1].x + xOffset][pointOrder[pointCount - 1].y + yOffset] = true;
		pointCount++;
		return true;
	}
	else
	{
		errorCode = 1;
hitBody:
		myCar.myLcd.Clear(0);
		return false;
	}
}

void removePoint()
{
	points[pointOrder[0].x][pointOrder[0].y] = false;
	for (Byte i = 1; i < pointCount; i++)
		pointOrder[i - 1] = pointOrder[i];
	pointOrder[--pointCount].x = 0;
	pointOrder[pointCount].y = 0;
}

bool run(MySmartCar &myCar, Byte ref, libbase::k60::RandGenerator &randGen)
{
	if (!myCar.extendFlag)
		removePoint();
	else
		myCar.extendFlag = false;

	switch(ref)
	{
	case 1:
		if (!addPoint(myCar, 0, -1, randGen))
			return false;
		break;
	case 2:
		if (!addPoint(myCar, 0, 1, randGen))
			return false;
		break;
	case 3:
		if (!addPoint(myCar, -1, 0, randGen))
			return false;
		break;
	case 4:
		if (!addPoint(myCar, 1, 0, randGen))
			return false;
		break;
	}
	paintDrawing(myCar);
	return true;
}

//

int main(void)
{
	System::Init();
	MySmartCar myCar;

	libbase::k60::RandGenerator::Config randConfig;
	randConfig.is_ext_entropy = false;

	libbase::k60::RandGenerator rG((const libbase::k60::RandGenerator::Config)randConfig);

//	while (true)
//	{
//		if (myCar.btStarted)
//		{
//			for (float i = 0.0f; i < 100.0f; i += 0.5f)
//			{
//				myCar.myUart.SendBuffer((Byte *)&i, sizeof(float));
//				myCar.myUart.SendBuffer((Byte *)"/", sizeof(Byte));
//				System::DelayMs(20);
//			}
//		}
//		System::DelayMs(20);
//	}

// Game

	myCar.myLcd.Clear(-1);
	myCar.curDirection = 4;
	randomFood(rG);

	for (int8_t i = 1; i < 5; i++)
	{
		points[i][1] = true;
		pointOrder[pointCount].x = i;
		pointOrder[pointCount++].y = 1;
	}

	paintDrawing(myCar);

	while (true)
	{
		while (!myCar.isGameStarted);
		Timer::TimerInt lastTime = System::Time();
		while (myCar.isGameStarted)
		{
			if (System::Time() - lastTime >= 25 && !myCar.isPaused)
			{
				lastNextAction = myCar.nextAction;
				switch (lastNextAction)
				{
				case 0:
					if (!run(myCar, myCar.curDirection, rG))
						goto stop;
					break;

				default:
					if (!((lastNextAction == 1 && myCar.curDirection == 2) ||
						(lastNextAction == 2 && myCar.curDirection == 1) ||
						(lastNextAction == 3 && myCar.curDirection == 4) ||
						(lastNextAction == 4 && myCar.curDirection == 3)))
					{
						if (!run(myCar, lastNextAction, rG))
							goto stop;
						myCar.curDirection = lastNextAction;
					}
					else
					{
						if (!run(myCar, myCar.curDirection, rG))
							goto stop;
					}
					break;
				}
				lastTime = System::Time();
			}
		}
	}

stop:

	myCar.myLcd.DrawString(6 * 8, 8 * 8, "Lose", -1);

//

	while (true);

	return 0;
}
*/

/* MagSensor
int main()
{
	System::Init();
	MySmartCar myCar;
	uint16_t f = 0;
	Timer::TimerInt lastTime = System::Time();

	while (true)
	{
		if (System::Time() - lastTime >= 10)
		{
			myCar.myMagSensor.StartConvert();
			f = myCar.myMagSensor.GetResult();
			myCar.myMagSensor.StopConvert();
			myCar.plotData((Byte *)&f, 2);
			lastTime = System::Time();
		}
	}

	while (true);

	return 0;
}*/

int main()
{
	System::Init();
	MySmartCar myCar;

//	array<float, 3> f;

//	Timer::TimerInt lastTime = System::Time();

	while (true)
	{
//		if (System::Time() - lastTime >= 20)
//		{
//			if (myCar.myAccel.Update())
//			{
//				f = myCar.myAccel.GetAccel();
//
//				myCar.plotData((Byte *)&f, 12);
//			}
//
//			lastTime = System::Time();
//		}
		for (float i = 0.0f; i < 100.0f; i += 0.5f)
		{
			myCar.plotData((Byte *)&i, 4);
			System::DelayMs(20);
		}

	}

	while (true);

	return 0;
}
