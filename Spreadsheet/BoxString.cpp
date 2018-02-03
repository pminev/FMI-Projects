#include "BoxString.h"
#include <iostream>

BoxString::BoxString(const char* content, unsigned short row, unsigned short col) :Box(content, row, col)
{
	/* ... */
	calculateTheValue(content);
}

BoxString::BoxString(const BoxString& other) : Box(other)
{
	this->value = other.value;
}

BoxString & BoxString::operator=(const BoxString& other)
{
	if (this != &other)
	{
		Box::operator=(other);
		this->value = other.value;
	}
	return *this;
}

Box* BoxString::clone()
{
	return new BoxString(*this);
}

double BoxString::getValue() const
{
	return value;
}

bool BoxString::isFormula()
{
	return false;
}

void BoxString::calculateTheValue(const char* content)
{
	unsigned short spaces = 0, points = 0;
	bool haveASymbol=false;
	int contentLength = strlen(content);
	for (int i = 0; i < contentLength; i++)
	{
		if ((content[i]>'9'||content[i]<'0')&&content[i]!='.')
		{
			haveASymbol = true;
		}

		if (content[i] == '.')
			points++;
		else if (content[i] == ' ')
			spaces++;
	}

	if (spaces==0 &&points<=1&& !haveASymbol)
	{
		this->value = atof(content);
	}
	else
	{
		this->value = 0;
	}
}
