/*
 * VarManager.cpp
 *
 *  Created on: Feb 8, 2015
 *      Author: Peter
 */

#include "VarManager.h"

// TODO: enable following preprocessor command
//#ifdef LIBSC_USE_UART

using namespace libbase::k60;

PlotData *m_pd_instance;

SysTick::Config PlotData::getTimerConfig()
{
	SysTick::Config config;
	config.count = 20;
	config.isr = &timer_tick;
	config.is_enable = false;
	return config;
}

FtdiFt232r::Config PlotData::getUartConfig(const uint8_t id)
{
	FtdiFt232r::Config config;
	config.id = id;
	config.baud_rate = libbase::k60::Uart::Config::BaudRate::k115200;
	config.rx_irq_threshold = rx_threshold;
	config.is_rx_irq_threshold_percentage = false;
	return config;
}

PlotData::PlotData(void)
:
	m_timer(getTimerConfig()),
	m_uart(getUartConfig(0)),
	isStarted(false)
{
	m_pd_instance = this;
	System::Init();
	sharedObjMng.ObjCount = 0;
	watchingObjMng.ObjCount = 0;
	sharedObjMng.ObjList = (void **)malloc(sizeof(void *) * max_obj_count);
	watchingObjMng.ObjList = (void **)malloc(sizeof(void *) * max_obj_count);
}

PlotData::~PlotData()
{
	free(sharedObjMng.ObjList);
	free(watchingObjMng.ObjList);
}

void PlotData::listener(const Byte *bytes, const size_t size)
{
	if (size != m_pd_instance->rx_threshold)
		return ;
	switch (bytes[0])
	{
	case 's':
		m_pd_instance->m_timer.SetEnable(true);
		break;

	case 'e':
		m_pd_instance->m_timer.SetEnable(false);
		break;
	}
}

void PlotData::timer_tick(SysTick *sys_tick)
{

}

void PlotData::Init()
{
	if (!isStarted)
		m_uart.EnableRx(&listener);
}

void PlotData::shareVar(void *pObj, Byte len, const char *varName)
{
	if (!isStarted)
	{
		sharedObjMng.ObjList[sharedObjMng.ObjCount++] = pObj;

	}
}

void PlotData::watchVar(void *pObj, Byte len, const char *varName)
{
	if (!isStarted)
	{

	}
}

//#endif /* LIBSC_USE_UART */
