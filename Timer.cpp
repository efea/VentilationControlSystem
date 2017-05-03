/*
 * Timer.cpp
 *
 *  Created on: Jan 28, 2016
 *      Author: macos
 */
#include "Timer.h"
Timer::Timer(int min, int sec, int ms, bool recurring){
	this->min = min;
	this->sec = sec;
	this->ms = ms;
	totalMs = ((min*60) + sec)*1000 + ms;
	this->recurring = recurring;
}

Timer::~Timer(){
	delete this;
}

bool Timer::IsRecurring(){
	return recurring;
}

bool Timer::Tick(){
	bool isExpired;
	totalMs--;
	if (totalMs <= 0){
		isExpired = true;
		if (IsRecurring()){
			totalMs = ((min*60) + sec)*1000 + ms;
		}
	}
	else isExpired = false;
	return isExpired;
}



