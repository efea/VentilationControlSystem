/*
 * PinEvent.cpp
 *
 *  Created on: 31.1.2016
 *      Author: krl
 */

#include "PinEvent.h"

PinEvent::PinEvent(DigitalIoPin& iopin): pin(iopin), old(false) {
	// TODO Auto-generated constructor stub
}

PinEvent::~PinEvent() {
	// TODO Auto-generated destructor stub
}

bool PinEvent::event() {
	bool now = pin.read();
	bool result = false;

	if(now != old) {
		if(now) {
			result = true;
		}
		old = now;
	}

	return result;
}
