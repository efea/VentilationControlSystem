/*
 * SimpleMenu.cpp
 *
 *  Created on: 3.2.2016
 *      Author: krl
 */

#include "SimpleMenu.h"

SimpleMenu::SimpleMenu() {
	position = 0;
	flag = false;
}

SimpleMenu::~SimpleMenu() {
	// TODO Auto-generated destructor stub
}

void SimpleMenu::addItem(MenuItem *item) {
	items.push_back(item);
}

void SimpleMenu::event(MenuItem::menuEvent e) {
	flag = false;
	if(items.size() <= 0) return;

	if(!items[position]->event(e)) {
		if(e == MenuItem::up) position++;
		else if(e == MenuItem::down) position--;

		if(position < 0) position = items.size() - 1;
		if(position >= (int) items.size()) position = 0;

		items[position]->event(MenuItem::show);
	}
	if ((e == MenuItem::ok) && (!items[position]->GetFocus())){
		flag = true;
	}
}

int16_t SimpleMenu::getValue(){
	return items[position]->getValue();
}

int SimpleMenu::getSetValue(){
	return items[position]->getSetValue();
}

bool SimpleMenu::isSensor(){
	return items[position]->isSensor();
}

bool SimpleMenu::isAuto(){
	return items[position]->isAuto();
}

bool SimpleMenu::isAutoT(){
	return items[position]->isAutoT();
}

bool SimpleMenu::GetFocus(){
	return items[position]->GetFocus();
}

