#pragma once

class Index
{
public:
	Index(unsigned short,unsigned short);
	Index(const char*);
	Index(const Index&);
	Index& operator=(const Index&);
	~Index();

	const char* getIndex() const;
	unsigned short getRowOfIndex() const;
	unsigned short getColOfIndex() const;

private:
	//interface to construate the index easier
	unsigned short numberOfDigits(unsigned int);
	char* numberToCharArr(unsigned int);

	void clear();
	void copyFrom(const Index&);
private:
	char* indexAsString;
	unsigned short row;
	unsigned short col;
};