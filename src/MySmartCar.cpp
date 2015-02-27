/*
 * mySmartCar.cpp
 *
 *  Created on: Dec 29, 2014
 *      Author: Peter
 */

#include "../inc/MySmartCar.h"
#include <cstring>
#include <stdlib.h>

using namespace libsc::k60;
using namespace std;

MySmartCar *m_msc_instance;

AlternateMotor::Config getAlterMotorConfig(const uint8_t id)
{
	AlternateMotor::Config config;
	config.id = id;
	return config;
}
DirMotor::Config getDirMotorConfig(const uint8_t id)
{
	DirMotor::Config config;
	config.id = id;
	return config;
}

UartDevice::Config getUartConfig(const uint8_t id)
{
	UartDevice::Config config;
	config.id = id;
	config.baud_rate = libbase::k60::Uart::Config::BaudRate::k115200;
	config.rx_irq_threshold = 2;
	return config;
}

Mma8451q::Config getMma8451qConfig(const uint8_t id)
{
	Mma8451q::Config config;
	config.id = id;
	config.sensitivity = Mma8451q::Config::Sensitivity::kMid;
	config.output_data_rate = Mma8451q::Config::OutputDataRate::k200Hz;
	config.power_mode = Mma8451q::Config::PowerMode::kLowNoiseLowPower;
	return config;
}

Adc::Config getAdc0Config()
{
	Adc::Config config;
	config.adc = libbase::k60::Adc::Name::kAdc0Ad5B;
	config.avg_pass = libbase::k60::Adc::Config::AveragePass::k8;
	config.resolution = Adc::Config::Resolution::k16Bit;
	return config;
}

Adc::Config getAdc1Config()
{
	Adc::Config config;
	config.adc = libbase::k60::Adc::Name::kAdc1Ad5B;
	config.avg_pass = libbase::k60::Adc::Config::AveragePass::k8;
	config.resolution = Adc::Config::Resolution::k16Bit;
	return config;
}

TrsD05::Config getServoConfig(const uint8_t id)
{
	TrsD05::Config config;
	config.id = id;
	return config;
}

St7735r::Config getLcdConfig()
{
	St7735r::Config config;
	config.is_revert = false;
	config.is_bgr = false;
	return config;
}

//AbEncoder::Config getEncoderConfig(const uint8_t id)
//{
//	AbEncoder::Config config;
//	config.id = id;
//	return config;
//}

void MySmartCar::ledInit(void)
{
	for (Byte i = 0; i < 4; i++)
		myLeds.push_back(Led({i}));
}

void MySmartCar::reset(void)
{
	for (Byte i = 0; i < myLeds.size(); i++)
		myLeds.data()[i].SetEnable(false);
	myMotor.SetClockwise(true);
	myServo.SetDegree(900);
	myMotor.SetPower(0);
}

void MySmartCar::setSpeed(const int16_t speed)
{
	if (speed > 0)
		myMotor.SetClockwise(true);
	else
		myMotor.SetClockwise(false);

	myMotor.SetPower(abs(speed));
}

void MySmartCar::turnLeft(const uint16_t degree_x10)
{
	const uint16_t new_degree_x10 = inRange(MIN_SERVO_TURNING_DEGREE, degree_x10, MAX_SERVO_TURNING_DEGREE);
	myServo.SetDegree(MID_SERVO_DEGREE + new_degree_x10);
}

void MySmartCar::turnRight(const uint16_t degree_x10)
{
	const uint16_t new_degree_x10 = inRange(MIN_SERVO_TURNING_DEGREE, degree_x10, MAX_SERVO_TURNING_DEGREE);
	myServo.SetDegree(MID_SERVO_DEGREE - new_degree_x10);
}

void MySmartCar::turn(const int16_t degree_x10)
{
	const int16_t new_degree_x10 = inRange(-MAX_SERVO_TURNING_DEGREE, outRangeOf(degree_x10, old_degree_x10, 10), MAX_SERVO_TURNING_DEGREE);
	old_degree_x10 = new_degree_x10;
	myServo.SetDegree(MID_SERVO_DEGREE + new_degree_x10);
}

void MySmartCar::doBlink(Byte id)
{
	myLeds.data()[id].Switch();
}

void MySmartCar::ExecuteCommand(const Byte *bytes, const size_t size)
{
	if (!size)
		return;

	switch (bytes[0])
	{
	case CMD_FORWARD:
		m_msc_instance->setSpeed(m_msc_instance->car_speed);
		break;

	case CMD_BACKWARD:
		m_msc_instance->setSpeed((int16_t)-m_msc_instance->car_speed);
		break;

	case CMD_LEFT:
		m_msc_instance->turnLeft(m_msc_instance->turning_angle);
		break;

	case CMD_RIGHT:
		m_msc_instance->turnRight(m_msc_instance->turning_angle);
		break;

	case CMD_FRONT:
		m_msc_instance->turnLeft(0);
		break;

	case CMD_STOP:
		m_msc_instance->setSpeed(0);
		break;

	case CMD_RESET:
		m_msc_instance->turnLeft(0);
		m_msc_instance->setSpeed(0);
		break;

	case CMD_SETANGLE:
		m_msc_instance->turning_angle = bytes[1] * 10;
		break;

	case CMD_SETSPEED:
		m_msc_instance->car_speed = bytes[1] * 10;
		break;
//	case 's':
//		m_msc_instance->myVarMng.sendWatchData();
//		m_msc_instance->myVarMng.Init();
//		m_msc_instance->btStarted = false;
////		m_msc_instance->btStarted = true;
////		m_msc_instance->myUart.SendBuffer((Byte *)"/", 1);
//		break;
//	case 'e':
//		m_msc_instance->myVarMng.UnInit();
////		m_msc_instance->btStarted = false;
//		break;
	case 'w':
		m_msc_instance->btStarted = false;
		break;
	}
}

//void MySmartCar::plotData(const Byte *data, const size_t len)
//{
//	if (btStarted)
//		myUart.SendBuffer(data, len);
//}

MySmartCar::MySmartCar(void)
:
//	myUart(getUartConfig(0)),
	myMotor(getDirMotorConfig(0)),
//	myMotor(getAlterMotorConfig(0)),
	myVarMng(),
	myLcd(getLcdConfig()),
	myMagSensor0(getAdc0Config()),
	myMagSensor1(getAdc1Config()),
	myAccel(getMma8451qConfig(0)),
	isClockWise(true),
	car_speed(250),
	old_degree_x10(0),
	myServo(getServoConfig(0)),
	turning_angle(450)
//	myEncoder(getEncoderConfig(0))
{
	m_msc_instance = this;
	System::Init();
	ledInit();
	reset();
//	myUart.EnableRx(&ExecuteCommand);
}
