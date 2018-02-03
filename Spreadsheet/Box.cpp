#include "Box.h"
#include <iostream>

void Box::clear()
{
	delete[] content;
	content = nullptr;

	delete index;
	index = nullptr;
}

void Box::copyFrom(const Box& other)
{
		this->content = new char[strlen(other.content) + 1];
		strcpy(this->content, other.content);
		
		this->index = new Index(*other.index);
}

Box::Box(const char* content,unsigned short row,unsigned short col):content(nullptr),index(nullptr)
{
	try
	{
		this->content = new char[strlen(content)+1];
		strcpy(this->content, content);

		this->index = new Index(row, col);
	}
	catch (const std::bad_alloc& e)
	{
		std::cerr << "Not enough memory!\n";
		clear();
	}
}

Box::Box(const Box& other):content(nullptr),index(nullptr)
{
	try
	{
		copyFrom(other);
	}
	catch (const std::bad_alloc&)
	{
		std::cout << "Not enough memory Dude!\n";
		clear();
	}
}

Box& Box::operator=(const Box& other)
{
	if (this!=&other)
	{
		try
		{
			clear();
			copyFrom(other);
		}
		catch (const std::bad_alloc&)
		{
			std::cout << "Not enough memory Dude!\n";
			clear();
		}
	}

	return *this;
}

Box::~Box()
{
	clear();
}

const char* Box::getContent() const
{
	return content;
}

const char* Box::getIndex() const
{
	return index->getIndex();
}



