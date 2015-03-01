/*
 * PID.h
 *
 *  Created on: Jan 15, 2015
 *      Author: Peter
 */

#include <libsc/k60/system.h>

#ifndef INC_PID_H_
#define INC_PID_H_

#define MAX(a, b) ((a > b)? a : b)

class PIDhandler
{
public:

	explicit PIDhandler(float *ref, float kp, float ki, float kd);
	explicit PIDhandler(float ref, float kp, float ki, float kd);
	float updatePID(float val, uint32_t dt);

	void setKp(float kp);
	void setKi(float ki);
	void setKd(float kd);

	float getKp(void);
	float getKi(void);
	float getKd(void);

	void reset();

private:

	float *reference;
	float Kp;
	float Ki;
	float Kd;
	float eSum;
	float lastError;
};


#endif /* INC_PID_H_ */
