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
#include <typeinfo>
#include <string.h>
#include <cxxabi.h>

using namespace libsc::k60;
using namespace libbase::k60;

class VarManager
{
public:

	class ObjMng
	{
	public:

		explicit ObjMng(void *pObj, Byte len, const std::string &typeName, const std::string &objName)
		:
			obj(pObj),
			len(len),
			typeName(typeName),
			varName(objName)
		{}

		~ObjMng() {};

		void						*obj;
		Byte						len;
		std::string					typeName;
		std::string					varName;
	};

	explicit VarManager(void);
	~VarManager(void);

	void Init(void);
	void Init(const JyMcuBt106::OnReceiveListener &oriListener);
	void UnInit(void);

	template<typename ObjType>
	void addSharedVar(ObjType *sharedObj, std::string s)
	{
		if (!isStarted)
		{
			std::string tn = std::string(typeid(ObjType).name());
			ObjMng newObj(sharedObj, sizeof(sharedObj), tn, s);
			sharedObjMng.push_back(newObj);
		}
	}

	template<typename ObjType>
	void addWatchedVar(ObjType *watchedObj, std::string s)
	{
		if (!isStarted)
		{
			ObjMng newObj(watchedObj, sizeof(*watchedObj), __cxxabiv1::__cxa_demangle(typeid(ObjType).name(), nullptr, 0, new int(0)), s);
			watchedObjMng.push_back(newObj);
		}
	}

	void sendWatchData(void);

private:

	JyMcuBt106						m_uart;

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
