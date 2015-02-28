/*
 * Loop.cpp
 *
 *  Created on: Feb 28, 2015
 *      Author: Peter
 */

#include "Loop.h"
#include <libsc/k60/system.h>

Loop::Loop(void)
:
	m_start_time(0)
{}

void Loop::addFunctionToLoop(const LoopFunction &func, Timer::TimerInt delay, uint16_t every)
{
	m_function_list.push_back(func);
	m_delay_list.push_back(delay);
	m_every_list.push_back(every);
	m_counter_list.push_back(0);
}

void DelayMsByTicks(Timer::TimerInt delay)
{
	Timer::TimerInt startTime = System::Time();

	while (Timer::TimeDiff(startTime, System::Time()) < delay);
}

void Loop::start(void)
{
	Timer::TimerInt timeDiff = 0;

	while (true)
	{
		for (uint16_t i = 0; i < m_function_list.size(); i++)
		{
			if (m_counter_list.at(i) >= m_every_list.at(i))
			{
				((LoopFunction)m_function_list.at(i))();
				DelayMsByTicks(m_delay_list.at(i));
				m_counter_list.at(i) = 0;
			}
			(m_counter_list.at(i))++;
		}
	}
}
