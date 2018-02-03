#pragma once
#include "Box.h"

class BoxCollection
{
public:
	BoxCollection();
	BoxCollection(const BoxCollection&);
	BoxCollection& operator=(const BoxCollection&);
	~BoxCollection();

	bool addBox(Box*);
	bool removeBox(unsigned short);
	Box* getBox(unsigned short) const;
	void editBox(const char* ,unsigned short,Box*);

	const char* getContent(unsigned int) const;
	const char* getIndex(unsigned int)const;
	int getSize() const;
	void print();

private:
	void clear();
	void copyFrom(const BoxCollection&);
	void resize();

private:
	Box** boxes;
	int size;
	int cap;
};