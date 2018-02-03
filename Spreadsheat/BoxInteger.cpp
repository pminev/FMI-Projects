#include "BoxInteger.h"
#include <iostream>

BoxInteger::BoxInteger(const char* content, unsigned short row, unsigned short col):Box(content,row,col)
{
	/* ... */
	value = atoi(content);
}

BoxInteger::BoxInteger(const BoxInteger& other):Box(other)
{
	this->value = other.value;
}

BoxInteger & BoxInteger::operator=(const BoxInteger& other)
{
	if (this!=&other)
	{
		Box::operator=(other);
		this->value = other.value;
	}
	return *this;
}

Box* BoxInteger::clone()
{
	return new BoxInteger(*this);
}

double BoxInteger::getValue() const
{
	return value;
}

bool BoxInteger::isFormula()
{
	return false;
}


