///*
// * PlotData.cpp
// *
// *  Created on: Feb 5, 2015
// *      Author: Peter
// */
//
//#include "PlotData.h"
//
//using namespace libsc::k60;
//
//PlotData *m_instance;
//
//PlotData::PlotData(FtdiFt232r *uart, char startChar, char endChar)
//:
//	m_uart(uart),
//	StartSymbol(startChar),
//	EndSymbol(endChar)
//{
//	m_instance = this;
//	m_uart->EnableRx(&MyOnReceiveListener);
//}
//
//PlotData::~PlotData()
//{}
//
//void PlotData::Plot(Byte *data, size_t n)
//{
//	if (IsStarted())
//		m_uart->SendBuffer(data, n);
//}
//
//void PlotData::MyOnReceiveListener(const Byte *bytes, const size_t size)
//{
//	if (size > 1)
//		return ;
//
//	if (bytes[0] == m_instance->StartSymbol)
//	{
//		m_instance->Started = true;
//		m_uart->SendBuffer((Byte *)"/", 1);
//	}
//	else if (bytes[0] == m_instance->EndSymbol)
//		m_instance->Started = false;
//}
//
//
