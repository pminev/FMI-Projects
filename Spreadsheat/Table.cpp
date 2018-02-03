#include <iostream>
#include <fstream>
#include <iomanip>
#include "Table.h"
#include "BoxDouble.h"
#include "BoxFormula.h"
#include "BoxInteger.h"
#include "BoxString.h"

Table::Table() :currFileName(nullptr), longestRow(0)
{
	this->rows = new DynArr<Row>();
}

Table::Table(const Table& other) : rows(nullptr), longestRow(0)
{
	copyFrom(other);
}

Table& Table::operator=(const Table& other)
{
	if (this != &other)
	{
		clear();
		copyFrom(other);
	}

	return *this;
}

Table::~Table()
{
	clear();
}

void Table::open(const char* fileName)
{
	std::ifstream ifs(fileName);
	if (!ifs)
	{
		std::cout << "File not found!\n";
		ifs.close();

		return;
	}

	currFileName = new char[strlen(fileName) + 1];
	strcpy(currFileName, fileName);
	if(ifs)
		readFromFile(ifs);

	ifs.close();
}

void Table::save()
{
	std::ofstream ofs(currFileName);
	saveInFile(ofs);
	ofs.close();
}

void Table::saveAs(const char * newFile)
{
	std::ofstream ofs(newFile);
	saveInFile(ofs);
	ofs.close();
}

void Table::close()
{
	clear();
}

void Table::print()
{
	for (int i = 0; i < rows->getSize(); i++)
	{
		for (int k = 0; k < rows->operator[](i).getNumberOfBoxes(); k++)
		{
			if (rows->operator[](i).getBox(k)->isFormula())
				std::cout << std::setw(11) << std::left << rows->operator[](i).getBox(k)->getValue() << '|';
			else
				std::cout << std::setw(11) <<std::left<<rows->operator[](i).getContentOfBox(k)<<'|';
		}

		for (int j = rows->operator[](i).getNumberOfBoxes(); j < longestRow; j++)
		{
			std::cout << std::setw(11) <<std::left<<" "<< '|';
		}
		std::cout << '\n';

	}
}

void Table::edit(unsigned short row, unsigned short col, const char * content)
{
	while (row>rows->getSize())
	{
		Row* row=new Row();
		rows->pushBack(*row);
		delete row;
	}
	rows->operator[](row).edit(content,row,col);
}


void Table::readFromFile(std::ifstream& ifs)
{
	char buffer[256];
	unsigned short rowCounter = 0;
	char** splittedBoxes;

	do
	{
		ifs.getline(buffer, 256);
		unsigned short boxesNumber = numberOfBoxes(buffer);
		splittedBoxes = splitBoxes(buffer, boxesNumber);
		Row* row = new Row();

		for (int i = 0; i < boxesNumber; i++)
		{
			Box* newBox = createBox(splittedBoxes[i], rowCounter, i);
			row->pushBack(newBox);

			delete newBox;
		}

		rows->pushBack(*row);

		if (longestRow<row->getNumberOfBoxes())
		{
			longestRow = row->getNumberOfBoxes();
		}

		delete row;
		row = nullptr;

		for (int i = 0; i < boxesNumber; i++)
		{
			delete[] splittedBoxes[i];
		}
		delete[] splittedBoxes;

	} while (!ifs.eof());
}

void Table::saveInFile(std::ofstream & ofs)
{
	for (int i = 0; i < rows->getSize(); i++)
	{
		for (int k = 0; k < rows->operator[](i).getNumberOfBoxes(); k++)
		{
			ofs << rows->operator[](i).getContentOfBox(k)<< ',';
		}
		ofs << '\n';
	}
}

Box* Table::findBox(const Index & index)
{
	return rows->operator[](index.getRowOfIndex()).getBox(index.getColOfIndex());
}

unsigned short Table::sumOfLeadingSpaces(const char * str)
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

unsigned short Table::sumOfTrailingSpaces(const char * str)
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

char * Table::trimNeedlessSpaces(const char * str)
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

char ** Table::splitBoxes(const char * str, unsigned short boxesNumber)
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

unsigned short Table::numberOfBoxes(const char * str)
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

Box * Table::chooseRightBox(unsigned short quotesNumber,
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
		if (buff[0] = 'R')
		{
			Index first(buff);
			firstBox = findBox(first);
			//There is no way to Find the box with the Adress, because we are int some row and we
			//dont know about the whole table... So creating the boxes must be in Table
		}
		else
		{
			firstBox = createBox(buff, currRow, currCol);
		}
		buff[0] = 0;
		bufferCnt = 0;

		for (int i = posOfOperation + 1; i < lengthOfString; i++)
		{
			buff[bufferCnt++] = trimmedBoxString[i];
		}

		if (buff[0] = 'R')
		{
			Index sec(buff);
			secBox = findBox(sec);

		}
		else
		{
			secBox = createBox(buff, currRow, currCol);
		}

		return new BoxFormula(firstBox, secBox, op, trimmedBoxString, currRow, currCol);
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

Box * Table::createBox(const char * boxString, unsigned short currRow, unsigned short currCol)
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

//Box * Table::findBox(const Index & index)
//{
//	for (int i = 0; i < rows->getSize(); i++)
//	{
//		for (int j = 0; j < rows->operator[](i).getNumberOfBoxes(); j++)
//		{
//			if (!strcmp(rows->operator[](i).getIndex(j),index.getIndex()))
//			{
//				return rows->operator[](i).operator[](j);
//			}
//		}
//	}
//	return nullptr;
//}


void Table::copyFrom(const Table& other)
{
	try
	{
		this->currFileName = new char[strlen(other.currFileName) + 1];
		strcpy(this->currFileName, other.currFileName);

		this->rows = new DynArr<Row>(*other.rows);
		this->longestRow = other.longestRow;
	}
	catch (const std::bad_alloc& e)
	{
		std::cerr << "Not enough memory!\n";
		clear();
	}
}

void Table::clear()
{
	delete[] currFileName;
	currFileName = nullptr;

	delete rows;
	rows = nullptr;
}