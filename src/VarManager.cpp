/*
 * VarManager.cpp
 *
 *  Created on: Feb 8, 2015
 *      Author: Peter
 */

#include "VarManager.h"

#include <cstdlib>
#include <functional>

// TODO: enable following preprocessor command
//#ifdef LIBSC_USE_UART

using namespace libbase::k60;
using namespace libsc::k60;

VarManager *m_pd_instance;

JyMcuBt106::Config VarManager::get106UartConfig(const uint8_t id)
{
	JyMcuBt106::Config config;
	config.baud_rate = libbase::k60::Uart::Config::BaudRate::k115200;
	config.rx_irq_threshold = rx_threshold;
	config.is_rx_irq_threshold_percentage = false;
	config.tx_buf_size = 50;
	return config;
}

FtdiFt232r::Config VarManager::get232UartConfig(const uint8_t id)
{
	FtdiFt232r::Config config;
	config.id = id;
	config.baud_rate = libbase::k60::Uart::Config::BaudRate::k115200;
	config.rx_irq_threshold = rx_threshold;
	config.is_rx_irq_threshold_percentage = false;
	config.tx_buf_size = 50;
	return config;
}

VarManager::VarManager(void)
:
	m_uart(get106UartConfig(0)),
	isStarted(false)
{
	m_pd_instance = this;
	System::Init();
}

VarManager::~VarManager()
{
	sharedObjMng.clear();
	watchedObjMng.clear();
}

VarManager::ObjMng::ObjMng()
:
	obj(nullptr),
	typeName(""),
	varName("")
{}

VarManager::ObjMng::~ObjMng()
{}

void VarManager::listener(const Byte *bytes, const size_t size)
{
	if (size != m_pd_instance->rx_threshold)
		return ;

	switch (bytes[0])
	{
	case 's':
		m_pd_instance->isStarted = true;
		break;

	case 'e':
		m_pd_instance->isStarted = false;
		break;

	case 'w':
		m_pd_instance->sendWatchedVarInfo();
		break;

	case 'h':
		m_pd_instance->sendSharedVarInfo();
		break;

	case 'c':
		// TODO: change variable here
		break;
	}

	if (m_pd_instance->m_origin_listener)
		m_pd_instance->m_origin_listener(bytes, size);
}

void VarManager::sendWatchData(void)
{
	if (isStarted)
		for (Byte i = 0; i < watchedObjMng.size(); i++)
			m_uart.SendBuffer((Byte *)watchedObjMng.at(i).obj, watchedObjMng.at(i).len);
}

void VarManager::sendWatchedVarInfo(void)
{
	m_uart.SendBuffer((Byte *)",", 1);
	Byte n = watchedObjMng.size();
	m_uart.SendBuffer((Byte *)&n, 1);
	for (Byte i = 0; i < watchedObjMng.size(); i++)
	{
		ObjMng temp = watchedObjMng.at(i);
		m_uart.SendBuffer((Byte *)temp.typeName.data(), temp.typeName.size() + 1);
		m_uart.SendBuffer((Byte *)temp.varName.data(), temp.varName.size() + 1);
		m_uart.SendBuffer((Byte *)",", 1);
	}
	m_uart.SendBuffer((Byte *)"end", 3);
}

void VarManager::sendSharedVarInfo(void)
{
	m_uart.SendBuffer((Byte *)".", 1);
	Byte n = sharedObjMng.size();
	m_uart.SendBuffer((Byte *)&n, 1);
	for (Byte i = 0; i < sharedObjMng.size(); i++)
	{
		ObjMng temp = sharedObjMng.at(i);
		m_uart.SendBuffer((Byte *)temp.typeName.data(), temp.typeName.size() + 1);
		m_uart.SendBuffer((Byte *)temp.varName.data(), temp.varName.size() + 1);
		m_uart.SendBuffer((Byte *)temp.obj, temp.len);
		m_uart.SendBuffer((Byte *)".", 1);
	}
	m_uart.SendBuffer((Byte *)"end", 3);
}

void VarManager::Init(const JyMcuBt106::OnReceiveListener &oriListener)
{
	if (!isStarted)
	{
		m_origin_listener = oriListener;
		m_uart.EnableRx(&listener);
	}
}

void VarManager::Init(void)
{
	if (!isStarted)
	{
		if (!m_origin_listener)
			m_origin_listener = nullptr;
		m_uart.EnableRx(&listener);
	}
}

void VarManager::UnInit(void)
{
	m_origin_listener = nullptr;
	m_uart.DisableRx();
}

void VarManager::addSharedVar(void *pObj, const char *pTypeName, Byte size, const char *pVarName)
{
	if (!isStarted)
	{
		ObjMng newObjMng;
		newObjMng.obj = pObj;
		newObjMng.len = size;
		newObjMng.typeName = std::string(pTypeName);
		newObjMng.varName = std::string(pVarName);
		sharedObjMng.push_back(newObjMng);
	}
}

void VarManager::addWatchedVar(void *pObj, const char *pTypeName, Byte size, const char *pVarName)
{
	if (!isStarted)
	{
		ObjMng newObjMng;
		newObjMng.obj = pObj;
		newObjMng.len = size;
		newObjMng.typeName = std::string(pTypeName);
		newObjMng.varName = std::string(pVarName);
		watchedObjMng.push_back(newObjMng);
	}
}

//#endif /* LIBSC_USE_UART */
