///*
// * PlotData.h
// *
// *  Created on: Feb 5, 2015
// *      Author: Peter
// */
//
//#ifndef INC_PLOTDATA_H_
//#define INC_PLOTDATA_H_
//
//#include <libsc/k60/ftdi_ft232r.h>
//#include <libsc/k60/system.h>
//#include <stddef.h>
//
//using namespace libsc::k60;
//
//class PlotData
//{
//public:
//
//	PlotData(FtdiFt232r *uart, char startChar, char endChar);
//	~PlotData();
//
//	void Plot(Byte *data, size_t n);
//
//	static void MyOnReceiveListener(const Byte *bytes, const size_t size);
//	bool IsStarted() { return Started; };
//
//private:
//
//	FtdiFt232r 		*m_uart;
//	char			StartSymbol, EndSymbol;
//	bool			Started = false;
//
//};
//
//
//#endif /* INC_PLOTDATA_H_ */
