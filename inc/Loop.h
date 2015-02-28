/*
 * Loop.h
 *
 *  Created on: Feb 28, 2015
 *      Author: Peter
 */

#ifndef INC_LOOP_H_
#define INC_LOOP_H_

#include <functional>
#include <vector>

#include <libsc/k60/timer.h>

using namespace libsc::k60;

#define LOOP_IMMEDIATELY	0
#define LOOP_EVERYTIME		1

#define maxValue(a, b) ((a > b)? a : b)

void DelayMsByTicks(Timer::TimerInt delay);

class Loop
{
public:

	/*
	 * function:
	 * void (Timer::TimerInt timeError)
	 */
	typedef std::function<void (void)> LoopFunction;

	Loop();

	void start(void);

	void addFunctionToLoop(const LoopFunction &func, Timer::TimerInt delay, uint16_t every);

private:

	std::vector<LoopFunction>		m_function_list;
	std::vector<Timer::TimerInt>	m_delay_list;
	std::vector<uint16_t>			m_every_list;
	std::vector<uint16_t>			m_counter_list;

	Timer::TimerInt					m_start_time;
	Timer::TimerInt					m_last_time;

};

#endif /* INC_LOOP_H_ */
