#include "BoxFormula.h"

double calcValue(double val1, double val2, char o)
{
	switch (o)
	{
	case'+':return val1 + val2; break;
	case'-':return val1 - val2; break;
	case'*':return val1 * val2; break;
	case'/':return val1 / val2; break;

	default:
		break;
	}
}

BoxFormula::BoxFormula(Box * box1, Box * box2,char op, const char * content, unsigned short row , unsigned short col):Box(content, row, col)
{
	this->value = calcValue(box1->getValue(), box2->getValue(), op);
}

BoxFormula::BoxFormula(const char* content, unsigned short row, unsigned short col) :Box(content, row, col)
{
	/* ... */
}

BoxFormula::BoxFormula(const BoxFormula& other) : Box(other)
{
	this->value = other.value;
}

BoxFormula & BoxFormula::operator=(const BoxFormula& other)
{
	if (this != &other)
	{
		Box::operator=(other);
		this->value = other.value;
	}
	return *this;
}

Box* BoxFormula::clone()
{
	return new BoxFormula(*this);
}

double BoxFormula::getValue() const
{
	return value;
}

bool BoxFormula::isFormula()
{
	return true;
}

