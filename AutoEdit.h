/*
 * AutoEdit.h
 *
 *  Created on: Mar 23, 2016
 *      Author: macos
 */

#ifndef AUTOEDIT_H_
#define AUTOEDIT_H_

#include "PropertyEdit.h"
#include "LiquidCrystal.h"
#include "BarGraph.h"
#include "I2C.h"
#include <string>

class AutoEdit: public PropertyEdit {
public:
	AutoEdit(LiquidCrystal& lcd_, std::string editTitle, bool auto_, bool autoT_);
	virtual ~AutoEdit();
	void increment();
	void decrement();
	void accept();
	void cancel();
	void setFocus(bool focus);
	void display();
	int getValue();
	void setFValue(int value);
	void setHValue(int value);
	bool isSensor();
	bool isAuto();
	bool isAutoT();
	int getSetValue();
private:
	void save();
//	void displayEditValue();
	LiquidCrystal& lcd;
	std::string title;
	int Fvalue;
	int edit;
	bool focus;
	int Hvalue;
	int SetValue;
	bool Auto;
	bool AutoT;
};



#endif /* AUTOEDIT_H_ */
