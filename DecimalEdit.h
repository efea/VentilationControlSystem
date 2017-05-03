/*
 * DecimalEdit.h
 *
 *  Created on: Mar 1, 2016
 *      Author: macos
 */

#ifndef DECIMALEDIT_H_
#define DECIMALEDIT_H_

#include "PropertyEdit.h"
#include "LiquidCrystal.h"
#include "BarGraph.h"
#include <string>

class DecimalEdit: public PropertyEdit{
public:
	DecimalEdit(LiquidCrystal& lcd_, std::string editTitle, double low_lim, double up_lim, BarGraph bg_);
	virtual ~DecimalEdit();
	void increment();
	void decrement();
	void accept();
	void cancel();
	void setFocus(bool focus);
	void display();
	int getValue();
	void setValue(double value);
	int getSetValue();
private:
	void save();
	void displayEditValue();
	LiquidCrystal& lcd;
	std::string title;
	double value;
	double edit;
	bool focus;
	double upper_lim;
	double lower_lim;
	BarGraph bg;
};

#endif /* DECIMALEDIT_H_ */
