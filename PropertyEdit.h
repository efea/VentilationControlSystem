/*
 * PropertyEdit.h
 *
 *  Created on: 2.2.2016
 *      Author: krl
 */

#ifndef PROPERTYEDIT_H_
#define PROPERTYEDIT_H_

class PropertyEdit {
public:
	PropertyEdit() {};
	virtual ~PropertyEdit() {};
	virtual void increment() = 0;
	virtual void decrement() = 0;
	virtual void accept() = 0;
	virtual void cancel() = 0;
	virtual void setFocus(bool focus) = 0;
	virtual void display() = 0;
	virtual int getValue() = 0;
	virtual bool isSensor() = 0;
	virtual bool isAuto() = 0;
	virtual bool isAutoT() = 0;
	virtual int getSetValue() = 0;
};

#endif /* PROPERTYEDIT_H_ */
