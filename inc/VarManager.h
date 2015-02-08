/*
 * VarManager.h
 *
 *  Created on: Feb 8, 2015
 *      Author: Peter
 */

#ifndef INC_VARMANAGER_H_

// TODO: enable following preprocessor command
//#ifdef LIBSC_USE_UART
#define INC_VARMANAGER_H_

#include <libsc/k60/system.h>
#include <libsc/k60/ftdi_ft232r.h>
#include <libbase/k60/sys_tick.h>

#include <cstdlib>

using namespace libsc::k60;
using namespace libbase::k60;

union ObjMng
{
	void			**ObjList;
	Byte			*LenList;
	Byte			ObjCount;
};

class PlotData
{
public:

	explicit PlotData(void);
	~PlotData(void);

	void Init(void);
	void shareVar(void *pObj, Byte len, const char *varName);
	void watchVar(void *pObj, Byte len, const char *varName);

	const Byte		max_obj_count = 10;
	const Byte		rx_threshold = 5;

private:

	FtdiFt232r		m_uart;
	SysTick			m_timer;

	ObjMng			sharedObjMng;
	ObjMng			watchingObjMng;

	bool			isStarted;

	static void listener(const Byte *bytes, const size_t size);
	static void timer_tick(SysTick *sys_tick);
	SysTick::Config getTimerConfig(void);
	FtdiFt232r::Config getUartConfig(const uint8_t id);

};

//#endif /* LIBSC_USE_UART */
#endif /* INC_VARMANAGER_H_ */
