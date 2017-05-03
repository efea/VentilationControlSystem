/*
 * AutoEdit.cpp
 *
 *  Created on: Mar 23, 2016
 *      Author: macos
 */

#include "AutoEdit.h"
#include <cstdio>

AutoEdit::AutoEdit(LiquidCrystal& lcd_, std::string editTitle, bool auto_, bool autoT_) :
		lcd(lcd_), title(editTitle), Auto(auto_), AutoT(autoT_){
	Fvalue = 0;
	Hvalue = 0;
	edit = 0;
	focus = false;
	SetValue = 25;

}

AutoEdit::~AutoEdit() {
}



void AutoEdit::increment() {
		SetValue++;
}

void AutoEdit::decrement() {
		SetValue--;
}

void AutoEdit::accept() {
	save();
}

void AutoEdit::cancel() {
	edit = Fvalue;
}

void AutoEdit::setFocus(bool focus) {
	this->focus = focus;
}

void AutoEdit::display() {
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.Print(title);
	lcd.setCursor(0, 1);
	char s[16];
/*	int integerEdit = edit / 10;
	int decimalEdit = edit % 10;*/
	if (focus) {
		snprintf(s, 16, "%4d%6d%4d", SetValue, Hvalue, Fvalue);
		lcd.Print(s);
	}
//	snprintf(s, 16, " %4d ", edit);
//	lcd.Print(s);
}

void AutoEdit::save() {
	// set current value to be same as edit value
	Fvalue = edit;
	// todo: save current value for example to EEPROM for permanent storage
}

int AutoEdit::getValue() {
	return Hvalue;
}

int AutoEdit::getSetValue() {
	return SetValue;
}

void AutoEdit::setFValue(int value) {
	edit = value;
	save();
}

void AutoEdit::setHValue(int value) {
	Hvalue = value;
}


bool AutoEdit::isSensor(){
	return false;
}

bool AutoEdit::isAuto(){
	return Auto;
}

bool AutoEdit::isAutoT(){
	return AutoT;
}
