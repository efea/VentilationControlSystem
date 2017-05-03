/*
 * PinEvent.h
 *
 *  Created on: 31.1.2016
 *      Author: krl
 */

#ifndef PINEVENT_H_
#define PINEVENT_H_

#include "DigitalIoPin.h"

class PinEvent {
public:
	PinEvent(DigitalIoPin& iopin);
	~PinEvent();
	bool event();
private:
	DigitalIoPin& pin;
	bool old;
};

#endif /* PINEVENT_H_ */
