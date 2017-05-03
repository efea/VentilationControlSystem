/*
 * MenuItem.cpp
 *
 *  Created on: 1.2.2016
 *      Author: krl
 */

#include "MenuItem.h"

MenuItem::MenuItem(PropertyEdit& property): focus(false), pe(property) {

}

MenuItem::~MenuItem() {

}

bool MenuItem::GetFocus(){
	return focus;
}

bool MenuItem::isSensor(){
	return pe.isSensor();
}

bool MenuItem::isAuto(){
	return pe.isAuto();
}

bool MenuItem::isAutoT(){
	return pe.isAutoT();
}

int16_t MenuItem::getValue(){
	return pe.getValue();
}

int MenuItem::getSetValue(){
	return pe.getSetValue();
}

bool MenuItem::event(menuEvent e) {
	bool handled = true;
	switch(e) {
	case ok:
		if(focus) {
			pe.accept();
			focus = false;
		}
		else {
			focus = true;
		}
		pe.setFocus(focus);
		break;
	case back:
		if(focus) {
			focus = false;
			pe.cancel();
		}
		else {
			handled = false;
		}
		pe.setFocus(focus);
		break;
	case show:
		pe.setFocus(focus);
		break;
	case up:
		if(focus) pe.increment();
		else handled = false;
		break;
	case down:
		if(focus) pe.decrement();
		else handled = false;
		break;
	}
	if(handled) pe.display();

	return handled;
}

