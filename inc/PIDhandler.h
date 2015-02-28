/*
 * PID.h
 *
 *  Created on: Jan 15, 2015
 *      Author: Peter
 */

#include <libsc/k60/system.h>

#ifndef INC_PID_H_
#define INC_PID_H_

class PIDhandler
{
public:

	explicit PIDhandler(int32_t ref, float kp, float ki, float kd);
	float updatePID(float val, uint32_t dt);

	void setKp(float kp);
	void setKi(float ki);
	void setKd(float kd);
	void reset();

private:

	float reference;
	float Kp;
	float Ki;
	float Kd;
	float eSum;
	float lastError;
};


#endif /* INC_PID_H_ */
