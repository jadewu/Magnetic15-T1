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
#include <libsc/k60/jy_mcu_bt_106.h>

using namespace libsc::k60;
using namespace libbase::k60;

class VarManager
{
public:

	class ObjMng
	{
	public:

		ObjMng();
		~ObjMng();

		void						*obj;
		Byte						len;
		std::string					typeName;
		std::string					varName;
	};

	explicit VarManager(void);
	~VarManager(void);

	void Init(const JyMcuBt106::OnReceiveListener &oriListener);
	void Init(void);
	void UnInit(void);

	void addSharedVar(void *pObj, const char *pTypeName, Byte size, const char *pVarName);
	void addWatchedVar(void *pObj, const char *pTypeName, Byte size, const char *pVarName);
	void sendWatchData(void);

private:

	JyMcuBt106						m_uart;
//	FtdiFt232r						m_uart;

	JyMcuBt106::OnReceiveListener	m_origin_listener;

	std::vector<ObjMng>				sharedObjMng;
	std::vector<ObjMng>				watchedObjMng;

	bool							isStarted;
	const Byte						rx_threshold = 6;

	static void listener(const Byte *bytes, const size_t size);

	SysTick::Config getTimerConfig(void);
	JyMcuBt106::Config get106UartConfig(const uint8_t id);
	FtdiFt232r::Config get232UartConfig(const uint8_t id);

	void sendWatchedVarInfo(void);
	void sendSharedVarInfo(void);

};

//#endif /* LIBSC_USE_UART */
#endif /* INC_VARMANAGER_H_ */
