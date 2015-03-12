/*
 * MyLoop.h
 *
 *  Created on: Feb 28, 2015
 *      Author: Peter
 */

#pragma once

#include <functional>
#include <vector>

#include <libsc/k60/timer.h>

using namespace libsc::k60;

#define LOOP_IMMEDIATELY	0
#define LOOP_EVERYTIME		1

#define maxValue(a, b) ((a > b)? a : b)

void DelayMsByTicks(Timer::TimerInt delay);

class MyLoop
{
public:

	/*
	 * function:
	 * void (Timer::TimerInt timeError)
	 */
	typedef std::function<void (void)> LoopFunction;

	MyLoop();

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
