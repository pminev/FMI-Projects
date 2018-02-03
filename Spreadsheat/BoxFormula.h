#pragma once
#include "Box.h"

class BoxFormula :public Box
{
public:
	BoxFormula(Box*, Box*,char, const char*, unsigned short, unsigned short);
	BoxFormula(const char*, unsigned short, unsigned short);
	BoxFormula(const BoxFormula&);
	BoxFormula& operator=(const BoxFormula&);

	virtual Box* clone();
	virtual double getValue() const;
	virtual bool isFormula();

private:
	double value;
};