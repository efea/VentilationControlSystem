/*
 * IntegerEdit.cpp
 *
 *  Created on: 2.2.2016
 *      Author: krl
 */

#include "IntegerEdit.h"
#include <cstdio>

IntegerEdit::IntegerEdit(LiquidCrystal& lcd_, std::string editTitle,
		int low_lim, int up_lim, BarGraph bg_) :
		lcd(lcd_), title(editTitle), lower_lim(low_lim), upper_lim(up_lim), bg(bg_) {
	value = 0;
	edit = 0;
	focus = false;
}

IntegerEdit::~IntegerEdit() {
}

void IntegerEdit::increment() {
	if (edit < upper_lim)
		edit++;
}

void IntegerEdit::decrement() {
	if (edit > lower_lim)
		edit--;
}

void IntegerEdit::accept() {
	save();
}

void IntegerEdit::cancel() {
	edit = value;
}

void IntegerEdit::setFocus(bool focus) {
	this->focus = focus;
}

void IntegerEdit::display() {
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.Print(title);
	lcd.setCursor(0, 1);
	char s[16];
	int integerEdit = edit / 10;
	int decimalEdit = edit % 10;
	if (focus) {
		snprintf(s, 16, "[%3d.%d]", integerEdit, decimalEdit);
	} else {
		snprintf(s, 16, " %3d.%d ", integerEdit, decimalEdit);
	}
	lcd.Print(s);
	bg.draw(((edit - lower_lim)*50) / (upper_lim-lower_lim));
}

void IntegerEdit::save() {
	// set current value to be same as edit value
	value = edit;
	// todo: save current value for example to EEPROM for permanent storage
}

int IntegerEdit::getValue() {
	return value;
}

int IntegerEdit::getSetValue() {
}

void IntegerEdit::setValue(int value) {
	if (value > upper_lim) {
		edit = upper_lim;
	} else if (value < lower_lim) {
		edit = lower_lim;
	} else
		edit = value;
	save();
}

bool IntegerEdit::isSensor(){
	return false;
}

bool IntegerEdit::isAuto(){
	return false;
}

bool IntegerEdit::isAutoT(){
	return false;
}

