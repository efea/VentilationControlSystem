/*
 * SensorEdit.h
 *
 *  Created on: Mar 22, 2016
 *      Author: macos
 */

#ifndef SENSOREDIT_H_
#define SENSOREDIT_H_

#include "PropertyEdit.h"
#include "LiquidCrystal.h"
#include "BarGraph.h"
#include "I2C.h"
#include <string>

class SensorEdit: public PropertyEdit {
public:
	SensorEdit(LiquidCrystal& lcd_, std::string editTitle, int low_lim, int up_lim, BarGraph bg_);
	virtual ~SensorEdit();
	void increment();
	void decrement();
	void accept();
	void cancel();
	void setFocus(bool focus);
	void display();
	int getValue();
	void setValue(int value);
	bool isSensor();
	bool isAuto();
	bool isAutoT();
	int getSetValue();
private:
	void save();
//	void displayEditValue();
	LiquidCrystal& lcd;
	std::string title;
	int value;
	int edit;
	bool focus;
	int upper_lim;
	int lower_lim;
	BarGraph bg;
};



#endif /* SENSOREDIT_H_ */
