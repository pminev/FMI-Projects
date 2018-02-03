#include "BoxDouble.h"
#include <iostream>

BoxDouble::BoxDouble(const char* content, unsigned short row, unsigned short col) :Box(content, row, col)
{
	/* ... */
	value = atof(content);
}

BoxDouble::BoxDouble(const BoxDouble& other) : Box(other)
{
	this->value = other.value;
}

BoxDouble & BoxDouble::operator=(const BoxDouble& other)
{
	if (this != &other)
	{
		Box::operator=(other);
		this->value = other.value;
	}
	return *this;
}

Box* BoxDouble::clone()
{
	return new BoxDouble(*this);
}

double BoxDouble::getValue() const
{
	return value;
}

bool BoxDouble::isFormula()
{
	return false;
}
