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
#include <libsc/k60/trs_d05.h>
#include <libsc/k60/ab_encoder.h>

#include "VarManager.h"
#include "Loop.h"

#include <vector>

using namespace libsc::k60;

#define	inRange(n, v, x) ((v > x)? x : ((v < n)? n : v))
#define outRangeOf(v, ov, r) ((v - ov < -r || v - ov > r)? v : 0)

#define MIN_MOTOR_POWER	0
#define MAX_MOTOR_POWER	1000

#define MID_SERVO_DEGREE 			900
#define MIN_SERVO_TURNING_DEGREE 	0
#define MAX_SERVO_TURNING_DEGREE	750

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

	void reset(void);

	void setSpeed(const int16_t speed);
	void turnLeft(const uint16_t degree_x10);
	void turnRight(const uint16_t degree_x10);
	void turn(const int16_t degree_x10);
	void doBlink(Byte id);

	void plotData(const Byte *data, const size_t len);
	static void ExecuteCommand(const Byte *bytes, const size_t size);

	bool				btStarted = true;

	Loop				myLoop;

	VarManager			myVarMng;
	Adc					myMagSensor0;
	Adc					myMagSensor1;
	St7735r				myLcd;
	TrsD05				myServo;
//	AbEncoder			myEncoder;

private:

	DirMotor			myMotor;
//	AlternateMotor		myMotor;
//	FtdiFt232r			myUart;
	Mma8451q			myAccel;
	std::vector<Led>	myLeds;

	bool				isClockWise;
	uint16_t			car_speed;
	uint16_t			turning_angle;
	int16_t				old_degree_x10;

	void ledInit(void);
	void magSensorInit(void);

};

#endif /* SRC_MYSMARTCAR_H_ */
