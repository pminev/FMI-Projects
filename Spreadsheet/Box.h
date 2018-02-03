#pragma once
#include "Index.h"

class Box
{
public:
	Box(const char*,unsigned short,unsigned short);
	Box(const Box&);
	Box& operator=(const Box&);
	virtual ~Box();

	const char* getContent() const;
	const char* getIndex() const ;

	virtual double getValue() const = 0;
	virtual Box* clone() = 0;
	virtual bool isFormula() = 0;

protected:
	void clear();
private:
	void copyFrom(const Box&);

private:
	Index* index;
	char* content;
};