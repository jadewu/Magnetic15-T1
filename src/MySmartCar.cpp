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

AlternateMotor::Config getMotorConfig(const uint8_t id)
{
	AlternateMotor::Config config;
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
	config.sens = Mma8451q::Config::Sensitivity::Mid;
	config.output_data_rate = Mma8451q::Config::ODR::k200Hz;
	config.scl_pin = libbase::k60::Pin::Name::kPtb0;
	config.sda_pin = libbase::k60::Pin::Name::kPtb1;
	config.power_mode = Mma8451q::Config::PowerMode::LowNoiseLowPower;
	return config;
}

Adc::Config getAdcConfig()
{
	Adc::Config config;
	config.adc = libbase::k60::Adc::Name::kAdc0Ad5B;
	config.avg_pass = libbase::k60::Adc::Config::AveragePass::k4;
	return config;
}
St7735r::Config getLcdConfig()
{
	St7735r::Config config;
	config.is_revert = false;
	return config;
}

void MySmartCar::reset(void)
{
	myLed.SetEnable(false);
	myMotor.SetClockwise(true);
	myServo.SetDegree(900);
	myMotor.SetPower(0);
}

void MySmartCar::setSpeed(const int16_t speed)
{
	if (speed < 0)
		myMotor.SetClockwise(true);
	else
		myMotor.SetClockwise(false);

	myMotor.SetPower(abs(speed));
}

void MySmartCar::turnLeft(const uint16_t degree_x10)
{
	const uint16_t new_degree_x10 = inRange(MIN_SERVO_DEGREE, degree_x10, MAX_SERVO_DEGREE);
	myServo.SetDegree(900 + new_degree_x10);
}

void MySmartCar::turnRight(const uint16_t degree_x10)
{
	const uint16_t new_degree_x10 = inRange(MIN_SERVO_DEGREE, degree_x10, MAX_SERVO_DEGREE);
	myServo.SetDegree(900 - new_degree_x10);
}

void MySmartCar::startBlinking(bool Enabled)
{
	isBlinking = Enabled;
}

void MySmartCar::doBlink(void)
{
	if (isBlinking)
		myLed.Switch();
}

MySmartCar* m_instance;

void MySmartCar::ExecuteCommand(const Byte *bytes, const size_t size)
{
	if (!size)
		return;

	switch (bytes[0])
	{
	case CMD_FORWARD:
		m_instance->setSpeed(m_instance->car_speed);
		break;

	case CMD_BACKWARD:
		m_instance->setSpeed((int16_t)-m_instance->car_speed);
		break;

	case CMD_LEFT:
		m_instance->turnLeft(m_instance->turning_angle);
		break;

	case CMD_RIGHT:
		m_instance->turnRight(m_instance->turning_angle);
		break;

	case CMD_FRONT:
		m_instance->turnLeft(0);
		break;

	case CMD_STOP:
		m_instance->setSpeed(0);
		break;

	case CMD_RESET:
		m_instance->turnLeft(0);
		m_instance->setSpeed(0);
		break;

	case CMD_SETANGLE:
		m_instance->turning_angle = bytes[1] * 10;
		break;

	case CMD_SETSPEED:
		m_instance->car_speed = bytes[1] * 10;
		break;
//	case 'x':
	case 's':
		m_instance->btStarted = true;
		m_instance->myUart.SendBuffer((Byte *)"/", 1);
		// TODO: Start with sth special.
		break;
	case 'e':
		m_instance->btStarted = false;
		break;

/* Game:
	case 'q':
		m_instance->isGameStarted = true;
		break;
	case 'w':
		m_instance->nextAction = 1;
		break;
	case 's':
		m_instance->nextAction = 2;
		break;
	case 'a':
		m_instance->nextAction = 3;
		break;
	case 'd':
		m_instance->nextAction = 4;
		break;
	case 'p':
		m_instance->isPaused = !m_instance->isPaused;
/*/

	}
}

void MySmartCar::plotData(const Byte *data, const size_t len)
{
	if (btStarted)
		myUart.SendBuffer(data, len);
}

MySmartCar::MySmartCar(void)
:
	myUart(getUartConfig(0)),
	myLed({0}),
	myMotor(getMotorConfig(0)),
	myServo({0}),
	myLcd(getLcdConfig()),
	myMagSensor(getAdcConfig())
//	myAccel(getMma8451qConfig(0))
{
	m_instance = this;
	reset();
	myUart.EnableRx(&ExecuteCommand);
}

MySmartCar::~MySmartCar(void) {}
