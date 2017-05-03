/*
 * Timer.h
 *
 *  Created on: Jan 28, 2016
 *      Author: macos
 */

#ifndef TIMER_H_
#define TIMER_H_

class Timer{
public:
	Timer(int min = 1, int sec = 0, int ms = 0, bool recurring = false);
	virtual ~Timer();
	bool IsRecurring();
	bool Tick();
private:
	int min;
	int sec;
	int ms;
	bool recurring;
	int totalMs;
};



#endif /* TIMER_H_ */
