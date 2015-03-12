/*
 * mySmartCar.h
 *
 *  Created on: Dec 29, 2014
 *      Author: Peter
 */

#pragma once

#include <vector>

#include <libsc/k60/system.h>
#include <libsc/k60/led.h>
#include <libsc/k60/alternate_motor.h>
#include <libsc/k60/dir_motor.h>
#include <libsc/k60/tower_pro_mg995.h>
#include <libbase/k60/adc.h>
#include <libsc/k60/ftdi_ft232r.h>
#include <libsc/k60/st7735r.h>
#include <libsc/k60/mma8451q.h>
#include <libsc/k60/futaba_s3010.h>
#include <libsc/k60/trs_d05.h>
#include <libsc/k60/ab_encoder.h>

#include "MyLoop.h"
#include "MyVarManager.h"
#include "MyMotor.h"
#include "MyServo.h"
#include "MyConfig.h"

using namespace libsc::k60;
using namespace libbase::k60;

#define	inRange(n, v, x) ((v > x)? x : ((v < n)? n : v))
#define outRangeOf(v, ov, r) ((v - ov < -r || v - ov > r)? v : 0)

#define MIN_MOTOR_POWER	0
#define MAX_MOTOR_POWER	1000

#define CMD_FORWARD		0
#define CMD_BACKWARD	1
#define CMD_LEFT		2
#define CMD_RIGHT		3
#define CMD_STOP		4
#define CMD_FRONT		5
#define CMD_RESET		6
#define CMD_SETANGLE	7
#define CMD_SETSPEED	8

class MySmartCar
{

public:

	MySmartCar(MyConfig &config);

	std::vector<Led> getLeds();

	void reset(void);

private:

#ifdef LIBSC_USE_LED
	std::vector<Led>	myLeds;
#endif
	MyVarManager		myVarMng;
	MyLoop				myLoop;
	MyServo				myEncoder;
	MyMotor				myMotor;

	MyConfig::SmartCarPowerMode		*m_powerMode;
	MyConfig::SmartCarTurningMode	*m_turningMode;

	void doBlink(Byte id);

};
