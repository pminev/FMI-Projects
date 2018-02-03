#pragma once
#include "Box.h"

class BoxString:public Box
{
public:
	BoxString(const char*, unsigned short, unsigned short);
	BoxString(const BoxString&);
	BoxString& operator=(const BoxString&);

	virtual Box* clone();
	virtual double getValue() const;
	virtual bool isFormula();

private:
	void calculateTheValue(const char*);
private:
	double value;
};