#pragma once
#include "Box.h"

class BoxDouble :public Box
{
public:
	BoxDouble(const char*, unsigned short, unsigned short);
	BoxDouble(const BoxDouble&);
	BoxDouble& operator=(const BoxDouble&);

	virtual Box* clone();
	virtual double getValue() const;
	virtual bool isFormula();

private:
	double value;
};