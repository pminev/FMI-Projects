#pragma once
#include "BoxCollection.h"

class Row {
public:
	Row();
	Row(const char*,unsigned short);
	Row(const Row&);
	Row& operator=(const Row&);
	~Row();

	void pushBack(Box*);
	void popBack();

	void print();

	//Edit some Box
	//First we need to create it
	void edit(const char * content, unsigned short row, unsigned short col);
	Box* getBox(unsigned short);
	//const Box*& operator[](int)const;
	//Box*& operator[](int);

	void setRow(const char*,unsigned short);
	const char* getIndex(int);
	int getNumberOfBoxes() const;
	const char* getContentOfBox(unsigned short) const;
	Box* createBox(const char*, unsigned short, unsigned short);

	Box* createBox(const Box&, unsigned short, unsigned short);

private:
	Box* chooseRightBox(unsigned short,
		unsigned short,
		unsigned short,
		unsigned short,
		unsigned short,
		const char*,
		unsigned short,
		unsigned short);

	unsigned short sumOfLeadingSpaces(const char*);
	unsigned short sumOfTrailingSpaces(const char*);
	char* trimNeedlessSpaces(const char*);
	char** splitBoxes(const char*,unsigned short);
	unsigned short numberOfBoxes(const char*);

private:
	BoxCollection* collection;
};