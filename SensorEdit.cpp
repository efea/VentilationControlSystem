/*
 * SensorEdit.cpp
 *
 *  Created on: Mar 22, 2016
 *      Author: macos
 */
#include "SensorEdit.h"
#include <cstdio>

SensorEdit::SensorEdit(LiquidCrystal& lcd_, std::string editTitle,
		int low_lim, int up_lim, BarGraph bg_) :
		lcd(lcd_), title(editTitle), lower_lim(low_lim), upper_lim(up_lim), bg(bg_) {
	value = 0;
	edit = 0;
	focus = false;

}

SensorEdit::~SensorEdit() {
}



void SensorEdit::increment() {
//	if (edit < upper_lim)
//		edit++;
}

void SensorEdit::decrement() {
//	if (edit > lower_lim)
//		edit--;
}

void SensorEdit::accept() {
	save();
}

void SensorEdit::cancel() {
	edit = value;
}

void SensorEdit::setFocus(bool focus) {
	this->focus = focus;
}

void SensorEdit::display() {
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.Print(title);
	lcd.setCursor(0, 1);
	char s[16];
/*	int integerEdit = edit / 10;
	int decimalEdit = edit % 10;*/
	if (focus) {
		snprintf(s, 16, "[%4d]", edit);
	} else {
		snprintf(s, 16, " %4d ", edit);
	}
	lcd.Print(s);
	bg.draw(((edit - lower_lim)*50) / (upper_lim-lower_lim));


}

void SensorEdit::save() {
	// set current value to be same as edit value
	value = edit;
	// todo: save current value for example to EEPROM for permanent storage
}

int SensorEdit::getValue() {
	return value;
}

int SensorEdit::getSetValue() {
}

void SensorEdit::setValue(int value) {
	if (value > upper_lim) {
		edit = upper_lim;
	} else if (value < lower_lim) {
		edit = lower_lim;
	} else
		edit = value;
	save();
}

bool SensorEdit::isSensor(){
	return true;
}

bool SensorEdit::isAuto(){
	return false;
}

bool SensorEdit::isAutoT(){
	return false;
}

