#pragma once
#include "Box.h"

class BoxInteger:public Box
{
public:
	BoxInteger(const char*, unsigned short, unsigned short);
	BoxInteger(const BoxInteger&);
	BoxInteger& operator=(const BoxInteger&);

	virtual Box* clone();
	virtual double getValue() const;
	virtual bool isFormula();

private:
	int value;
};