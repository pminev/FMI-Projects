#include "Row.h"
#include "BoxDouble.h"
#include "BoxFormula.h"
#include "BoxInteger.h"
#include "BoxString.h"
#include <iostream>

Row::Row() :collection(nullptr)
{
	collection = new BoxCollection();
}

Row::Row(const char* str, unsigned short col)
{
	collection = new BoxCollection();
	setRow(str, col);
}

Row::Row(const Row& other) : collection(nullptr)
{
	collection = new BoxCollection(*other.collection);
}

Row& Row::operator=(const Row& other)
{
	if (this != &other)
	{
		delete collection;
		collection = new BoxCollection(*other.collection);
	}

	return *this;
}

Row::~Row()
{
	delete collection;
	collection = nullptr;
}

void Row::pushBack(Box* box)
{
	collection->addBox(box);
}

void Row::print()
{
	(*collection).print();
}

void Row::edit(const char * content,unsigned short row, unsigned short col)
{
	int cols = col;
	while (col>collection->getSize())
	{
		Box* emptyBox = new BoxInteger(" ",row,++cols);
		collection->addBox(emptyBox);
	}
	Box* newBox = createBox(content,row,col);
	collection->editBox(content, col,newBox);
}

Box* Row::getBox(unsigned short index)
{
	return collection->getBox(index);
}



void Row::setRow(const char* line, unsigned short currRow)
{
	unsigned short boxesNumber = numberOfBoxes(line);
	char** splittedBoxes = splitBoxes(line, boxesNumber);
	for (int i = 0; i < boxesNumber; i++)
	{
		std::cout << splittedBoxes[i] << '\n';
	}

	for (int i = 0; i < boxesNumber; i++)
	{
		Box* newBox = createBox(splittedBoxes[i], currRow, i);
		collection->addBox(newBox);
		delete newBox;
		newBox = nullptr;
	}
}

const char * Row::getIndex(int index)
{
	return collection->getIndex(index);
}

int Row::getNumberOfBoxes() const
{
	return collection->getSize();
}

const char * Row::getContentOfBox(unsigned short index) const
{
	return collection->getContent(index);
}

char** Row::splitBoxes(const char* str, unsigned short boxesNumber)
{
	unsigned short strLength = strlen(str);
	char buffer[64];
	char** boxes = new char*[boxesNumber + 1];
	short quotesNumber = 0, bufferCnt = 0, boxesCnt = 0;
	for (size_t i = 0; i <= strLength; i++)
	{
		if (str[i] == '"')
			quotesNumber++;

		if (quotesNumber % 2 == 0 && str[i] == ',' || (bufferCnt > 0 && i == strLength))
		{
			buffer[bufferCnt] = 0;
			boxes[boxesCnt] = new char[strlen(buffer) + 1];
			strcpy(boxes[boxesCnt++], buffer);
			buffer[0] = 0;
			bufferCnt = 0;
		}
		else
		{
			buffer[bufferCnt++] = str[i];
		}
	}

	if (boxesNumber == 1 && strLength == 0)
	{
		boxes[boxesCnt] = new char[2];
		boxes[boxesCnt][0] = 0;
	}
	return boxes;
}

unsigned short Row::numberOfBoxes(const char * str)
{
	unsigned short strLength = strlen(str);
	short quotesNumber = 0, boxesCnt = 0;

	for (size_t i = 0; i < strLength; i++)
	{
		if (str[i] == '"')
			quotesNumber++;

		if (quotesNumber % 2 == 0 && str[i] == ',')
		{
			boxesCnt++;
		}
	}

	return boxesCnt + 1;
}

Box * Row::createBox(const char * boxString, unsigned short currRow, unsigned short currCol)
{
	char* trimmedBoxString = trimNeedlessSpaces(boxString);
	unsigned short strLength = strlen(trimmedBoxString);
	unsigned short	quotesNumber = 0,
		pointsNumber = 0,
		spacesNumber = 0,
		equalsNumber = 0,
		dashNumber = 0;

	for (int i = 0; i < strLength; i++)
	{
		if (trimmedBoxString[i] == '"')
			quotesNumber++;
		else if (trimmedBoxString[i] == '.')
			pointsNumber++;
		else if (trimmedBoxString[i] == ' ')
			spacesNumber++;
		else if (trimmedBoxString[i] == '=')
			equalsNumber++;
		else if (trimmedBoxString[i] == '\'')
			dashNumber++;
	}
	return chooseRightBox(quotesNumber,
		pointsNumber,
		spacesNumber,
		equalsNumber,
		dashNumber,
		trimmedBoxString,
		currRow, currCol);
}

Box* Row::chooseRightBox(unsigned short quotesNumber,
	unsigned short pointsNumber,
	unsigned short spacesNumber,
	unsigned short equalsNumber,
	unsigned short dashNumber,
	const char* trimmedBoxString,
	unsigned short currRow,
	unsigned short currCol)
{

	if (quotesNumber == 0 &&
		pointsNumber == 0 &&
		spacesNumber == 0 &&
		equalsNumber == 0 &&
		dashNumber == 0)
	{
		return new BoxInteger(trimmedBoxString, currRow, currCol);
	}
	else if (quotesNumber == 0 &&
		pointsNumber <= 1 &&
		spacesNumber == 0 &&
		equalsNumber == 1 &&
		dashNumber == 0)
	{
		char buff[16];
		buff[0] = 0;
		int lengthOfString = strlen(trimmedBoxString), posOfOperation = 0, bufferCnt = 0;
		char op;
		for (int i = 1; i < lengthOfString; i++)
		{
			if (trimmedBoxString[i] == '+' ||
				trimmedBoxString[i] == '-' ||
				trimmedBoxString[i] == '*' ||
				trimmedBoxString[i] == '/')
			{
				op = trimmedBoxString[i];
				posOfOperation = i;
				break;
			}
			buff[bufferCnt++] = trimmedBoxString[i];

		}
		buff[bufferCnt] = 0;
		Box* firstBox;
		Box* secBox;
		if (buff[0]='R')
		{
			Index first(buff);
			//There is no way to Find the box with the Adress, because we are int some row and we
			//dont know about the whole table... So creating the boxes must be in Table
		}
		else
		{
			firstBox = createBox(buff,currRow,currCol);
		}
		buff[0] = 0;
		bufferCnt = 0;

		for (int i = posOfOperation+1; i < lengthOfString; i++)
		{
			buff[bufferCnt++] = trimmedBoxString[i];
		}

		if (buff[0] = 'R')
		{
			Index sec(buff);
			//There is no way to Find the box with the Adress, because we are int some row and we
			//dont know about the whole table... So creating the boxes must be in Table
		}
		else
		{
			secBox = createBox(buff, currRow, currCol);
		}

		return new BoxFormula(firstBox,secBox,op,trimmedBoxString, currRow, currCol);
	}
	else if (quotesNumber == 0 &&
		pointsNumber == 1 &&
		spacesNumber == 0 &&
		equalsNumber == 0 &&
		dashNumber == 0)
	{
		return new BoxDouble(trimmedBoxString, currRow, currCol);
	}
	else if (quotesNumber % 2 == 0 &&
		dashNumber == quotesNumber - 2)
	{
		return new BoxString(trimmedBoxString, currRow, currCol);
	}
	else
	{
		throw new std::exception("Just fuck it... constructing box is not right!\n");
	}
	
}

unsigned short Row::sumOfLeadingSpaces(const char* str)
{
	unsigned short cnt = 0;
	int length = strlen(str);
	for (size_t i = 0; i < length; i++)
	{
		if (str[i] == ' ')
			cnt++;
		else
			break;
	}

	return cnt;
}

unsigned short Row::sumOfTrailingSpaces(const char* str)
{
	unsigned short cnt = 0;
	int length = strlen(str);
	for (size_t i = length - 1; i >= 0; i--)
	{
		if (str[i] == ' ')
			cnt++;
		else
			break;
	}

	return cnt;
}
char* Row::trimNeedlessSpaces(const char* str)
{
	unsigned short leading = sumOfLeadingSpaces(str);
	unsigned short trailing = sumOfTrailingSpaces(str);
	unsigned short sizeOfNewStrin = strlen(str) - leading - trailing;

	char* newStr = new char[sizeOfNewStrin | +1];
	newStr[0] = 0;
	strncpy(newStr, str + leading, strlen(str) - trailing + 1);
	newStr[sizeOfNewStrin] = 0;

	return newStr;
}