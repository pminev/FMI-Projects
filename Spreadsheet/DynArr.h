#pragma once
#include <iostream>

template<class T>
class DynArr {
public:
	DynArr();
	DynArr(const DynArr<T>&);
	DynArr<T>& operator=(const DynArr<T>&);
	~DynArr();

public:
	void pushBack(const T&);
	void popBack();

	const T& operator[](int)const;
	T& operator[](int);

	int getSize()const;
	int getcapacity()const;

	//helpers
private:
	void copyFrom(const DynArr<T>&);
	void clear();
	void resize(int);

private:
	T* data;
	int size;
	int capacity;
};

template<class T>
void DynArr<T>::clear() {

	delete[] data;

	data = nullptr;
	size = 0;
	capacity = 0;

}

template<class T>
void DynArr<T>::copyFrom(const DynArr<T> &other) {

	data = new T[other.size];

	for (int i = 0; i < other.size; i++) {
		data[i] = other.data[i];
	}

	size = other.size;
	capacity = other.capacity;
}

template<class T>
void DynArr<T>::resize(int newCap) {

	T *temp = data;
	data = new T[newCap];

	for (int i = 0; i < size; i++) {

		data[i] = temp[i];
	}
	capacity = newCap;
	delete[] temp;
}


template<class T>
DynArr<T>::DynArr() :data(nullptr), size(0), capacity(0) {
	/*...*/
}

template<class T>
DynArr<T>::DynArr(const DynArr<T> &other) : data(nullptr), size(0), capacity(0) {
	copyFrom(other);
}

template<class T>
DynArr<T>& DynArr<T>::operator=(const DynArr<T> &other) {

	if (this != &other) {
		clear();
		copyFrom(other);
	}
	return *this;
}

template<class T>
DynArr<T>::~DynArr() {
	clear();
}

template<class T>
void DynArr<T>::pushBack(const T& elem) {

	if (size >= capacity) {

		int NewCap = (capacity == 0 ? 2 : capacity * 2);
		resize(NewCap);
	}

	data[size++] = elem;
}

template<class T>
void DynArr<T>::popBack() {

	if (size)
		size--;
	else
		throw std::length_error("already empty!");

	if (size * 2 <= capacity) {

		int NewCap = ((size == 0) ? 0 : capacity / 2);
		Resize(NewCap);
	}
}

template<class T>
const T& DynArr<T>::operator[](int pos)const {
	if (pos >= size) {
		throw std::out_of_range("Index out of range!");
	}
	return data[pos];
}

template<class T>
T& DynArr<T>::operator[](int pos) {

	if (pos > size) {
		throw std::out_of_range("Index out of range!");
	}
	return data[pos];
}

template<class T>
int DynArr<T>::getSize()const {
	return size;
}

template<class T>
int DynArr<T>::getcapacity()const {
	return capacity;
}