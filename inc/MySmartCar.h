/*
 * mySmartCar.h
 *
 *  Created on: Dec 29, 2014
 *      Author: Peter
 */

#ifndef SRC_MYSMARTCAR_H_
#define SRC_MYSMARTCAR_H_

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
#include "VarManager.h"

using namespace libsc::k60;

#define	inRange(n, v, x) ((v > x)? x : ((v < n)? n : v))

#define MIN_MOTOR_POWER	0
#define MAX_MOTOR_POWER	1000

#define MIN_SERVO_DEGREE 0
#define MAX_SERVO_DEGREE 900

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

	MySmartCar(void);
	~MySmartCar(void);

	void reset(void);

	void setSpeed(const int16_t speed);
	void turnLeft(const uint16_t degree);
	void turnRight(const uint16_t degree);
	void doBlink(Byte id);

	void plotData(const Byte *data, const size_t len);
	static void ExecuteCommand(const Byte *bytes, const size_t size);

	bool				btStarted = true;

	VarManager			myVarMng;
	Adc					myMagSensor;
	St7735r				myLcd;
	FutabaS3010			myServo;

private:

#ifdef LIBSC_MOTOR0_DIR
	DirMotor			myMotor;
#else
	AlternateMotor		myMotor;
#endif
//	FtdiFt232r			myUart;
	Mma8451q			myAccel;
	Led					myLed0;
	Led					myLed1;
	Led					myLed2;
	Led					myLed3;

	bool				isClockWise;
	uint16_t			car_speed;
	uint16_t			turning_angle;

};

#endif /* SRC_MYSMARTCAR_H_ */
