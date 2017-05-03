/*
 * DecimalEdit.cpp
 *
 *  Created on: Mar 1, 2016
 *      Author: macos
 */

#include "DecimalEdit.h"
#include <cstdio>

DecimalEdit::DecimalEdit(LiquidCrystal& lcd_, std::string editTitle,
		double low_lim, double up_lim,  BarGraph bg_) :
		lcd(lcd_), title(editTitle), lower_lim(low_lim), upper_lim(up_lim), bg(bg_) {
	value = 0;
	edit = 0;
	focus = false;
}

DecimalEdit::~DecimalEdit() {
}

void DecimalEdit::increment() {
	if (edit < upper_lim)
		edit += 0.1;
}

void DecimalEdit::decrement() {
	if (edit > lower_lim)
		edit -= 0.1;
}

void DecimalEdit::accept() {
	save();
}

void DecimalEdit::cancel() {
	edit = value;
}

void DecimalEdit::setFocus(bool focus) {
	this->focus = focus;
}

void DecimalEdit::display() {
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.Print(title);
	lcd.setCursor(0, 1);
	char s[16];
	if (focus) {
		snprintf(s, 16, "[%.1f]", edit);
	} else {
		snprintf(s, 16, " %.1f ", edit);
	}
	lcd.Print(s);
	bg.draw(((edit - lower_lim)*50) / (upper_lim-lower_lim));
}

void DecimalEdit::save() {
	// set current value to be same as edit value
	value = edit;
	// todo: save current value for example to EEPROM for permanent storage
}

int DecimalEdit::getValue() {
	return value;
}

int DecimalEdit::getSetValue() {
}

void DecimalEdit::setValue(double value) {
	if (value > upper_lim) {
		edit = upper_lim;
	} else if (value < lower_lim) {
		edit = lower_lim;
	} else
		edit = value;
	save();
}



