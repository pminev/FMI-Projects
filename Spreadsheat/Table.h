#pragma once
#include "Box.h"
#include "Row.h"
#include "DynArr.h"

class Table {
public:
	Table();
	Table(const Table&);
	Table& operator=(const Table&);
	~Table();

	void open(const char*);
	void save();
	void saveAs(const char*);
	void close();
	void print();

	void edit(unsigned short,unsigned short, const char*);

private:
	void copyFrom(const Table&);
	void clear();
	void readFromFile(std::ifstream&);
	void saveInFile(std::ofstream&);

	Box* findBox(const Index&);

private:
	unsigned short sumOfLeadingSpaces(const char*);
	unsigned short sumOfTrailingSpaces(const char*);
	char* trimNeedlessSpaces(const char*);
	char** splitBoxes(const char*, unsigned short);
	unsigned short numberOfBoxes(const char*);

	Box* chooseRightBox(unsigned short,
		unsigned short,
		unsigned short,
		unsigned short,
		unsigned short,
		const char*,
		unsigned short,
		unsigned short);


	Box* createBox(const char*, unsigned short, unsigned short);

private:
	char* currFileName;
	DynArr<Row>* rows;
	unsigned short longestRow;
};