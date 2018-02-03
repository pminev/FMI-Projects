#include "BoxCollection.h"
#include <iostream>
#include <iomanip>

BoxCollection::BoxCollection():size(0),cap(4)
{
	try
	{
		boxes = new Box*[cap];
	}
	catch (const std::bad_alloc&)
	{
		std::cerr << "Not enough memory!\n";
		clear();
	}
}

BoxCollection::BoxCollection(const BoxCollection& other)
{
	copyFrom(other);
}

BoxCollection& BoxCollection::operator=(const BoxCollection& other)
{
	if (this!=&other)
	{
		clear();
		copyFrom(other);
	}
	return *this;
}

BoxCollection::~BoxCollection()
{
	clear();
}

//TO DO APROPRIATE ADDING A BOX
bool BoxCollection::addBox(Box* box)
{
	if (size>=cap)
	{
		resize();
	}
	boxes[size++] = box->clone();
	return true;
}

//TO DO APROPRIATE REMOVING A BOX
bool BoxCollection::removeBox(unsigned short index)
{
	if (index>size)
	{
		throw new std::out_of_range("Index out of range!\n");
		return false;
	}

	for (int i = 0; i < size; i++)
	{

	}

	return true;

}

Box * BoxCollection::getBox(unsigned short index) const
{
	if (index>=size)
	{
		return nullptr;
	}
	return boxes[index];

}

void BoxCollection::editBox(const char * content, unsigned short col,Box* newBox)
{
	Box* box;
	if (boxes[col-1])
	{
		box = boxes[col-1];
		boxes[col-1] = newBox;
	}
	else
	{
		boxes[size++] = newBox->clone();
	}
	delete box;
}

const char * BoxCollection::getContent(unsigned int index) const
{
	return boxes[index]->getContent();
}

const char * BoxCollection::getIndex(unsigned int index) const
{
	return boxes[index]->getIndex();
}

int BoxCollection::getSize() const
{
	return size;
}

void BoxCollection::print()
{
	for (int i = 0; i < size; i++)
	{
		std::cout << std::setw(10) << boxes[i]->getContent() << '|';
	}
}

void BoxCollection::clear()
{
	for (int i = 0; i < size; i++)
	{
		delete boxes[i];
	}
	delete[] boxes;

	size = 0;
	cap = 0;
}

void BoxCollection::copyFrom(const BoxCollection& other)
{
	try
	{
		boxes = new Box*[other.cap];
		for (size_t i = 0; i < other.size; i++)
		{
			boxes[i] = other.boxes[i]->clone();
		}
		cap = other.cap;
		size = other.size;
	}
	catch (const std::bad_alloc& e)
	{
		std::cerr << "Not enough memory!/n";
		clear();
	}
}

void BoxCollection::resize()
{
	try
	{
		Box** oldBoxes = boxes;
		boxes = new Box*[cap * 2];

		for (int i = 0; i < size; i++)
		{
			boxes[i] = oldBoxes[i]->clone();
		}
		cap = cap * 2;
	}
	catch (const std::bad_alloc& e )
	{
		std::cerr << "Not enough memory!\n";
		clear();
	}
}