#include "Index.h"
#include <iostream>
Index::Index(unsigned short row, unsigned short col) :row(row), col(col), indexAsString(nullptr)
{
	char* rowAsString = numberToCharArr(row);
	char* colAsString = numberToCharArr(col);
	int lenghtOfFullIndex = strlen(rowAsString) + strlen(colAsString) + 2;
	indexAsString = new char[lenghtOfFullIndex + 1];
	indexAsString[0] = 0;
	strcat(indexAsString, "R");
	strcat(indexAsString, rowAsString);
	strcat(indexAsString, "C");
	strcat(indexAsString, colAsString);

	delete[] rowAsString;
	delete[] colAsString;
}

Index::Index(const char * str)
{
	char buff[16];
	buff[0] = 0;
	int strLength = strlen(str);
	unsigned short posOfCol, rowVal, colVal,buffCnt=0;
	for (int i = 1; i < strLength; i++)
	{
		if (str[i]=='C')
		{
			buff[buffCnt] = 0;
			posOfCol = i;
			break;
		}
		buff[buffCnt++] = str[i];
	}
	rowVal = atoi(buff);
	buff[0] = 0;
	buffCnt = 0;
	for (int i = posOfCol+1; i < strLength; i++)
	{
		buff[buffCnt++] = str[i];
	}

	buff[buffCnt] = 0;
	colVal = atoi(buff);

	this->row = rowVal;
	this->col = colVal;
	this->indexAsString = new char[strLength + 1];
	strcpy(this->indexAsString, str);

}

Index::Index(const Index& other) : indexAsString(nullptr)
{
	copyFrom(other);
}

Index& Index::operator=(const Index& other)
{
	if (this != &other)
	{
		clear();
		copyFrom(other);
	}
	return *this;
}

Index::~Index()
{
	clear();
}

const char * Index::getIndex() const
{
	return indexAsString;
}

unsigned short Index::getRowOfIndex() const
{
	return row;
}

unsigned short Index::getColOfIndex() const
{
	return col;
}

unsigned short Index::numberOfDigits(unsigned int number)
{
	unsigned short cnt = 0;
	while (number > 0)
	{
		number /= 10;
		cnt++;
	}
	return cnt;
}

char* Index::numberToCharArr(unsigned int number)
{
	char* byff = new char[numberOfDigits(number) + 2];
	return itoa(number, byff, 10);
}

void Index::clear()
{
	delete[] indexAsString;
	indexAsString = nullptr;
}

void Index::copyFrom(const Index& other)
{
	try
	{
		this->indexAsString = new char[strlen(other.indexAsString) + 1];
		strcpy(this->indexAsString, other.indexAsString);
		this->row = other.row;
		this->col = other.col;
	}
	catch (const std::bad_alloc& e)
	{
		std::cerr << "Not enough memory!\n";
		clear();
	}
}