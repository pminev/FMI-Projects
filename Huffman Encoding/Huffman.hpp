#pragma once
#include <iostream>
#include <fstream>
#include "Heap.hpp"
#include <string>

const int MAX = 256;

class Huffman
{
private:
	struct Tree
	{
		unsigned char sym;
		unsigned int weight;
		Tree* pLeft;
		Tree* pRight;
		Tree()
		{
			sym = '\0';
			weight = 0;
			pLeft = nullptr;
			pRight = nullptr;
		}

		Tree(unsigned char s, unsigned int w = 0, Tree* pL = nullptr, Tree* pR = nullptr)
		{
			sym = s;
			weight = w;
			pLeft = pL;
			pRight = pR;
		}

		void CopyTree(Tree*& newTree, const Tree* toCopy)
		{
			if (toCopy == nullptr)
			{
				newTree = nullptr;
				return;
			}

			newTree = new Tree(toCopy->sym, toCopy->weight);

			if (newTree != nullptr)
			{
				CopyTree(newTree->pLeft, toCopy->pLeft);
				CopyTree(newTree->pRight, toCopy->pRight);
			}
		}

		Tree(const Tree& other)
		{
			sym = other.sym;
			weight = other.weight;
			pLeft = new Tree();
			pRight = new Tree();
			CopyTree(pLeft, other.pLeft);
			CopyTree(pRight, other.pRight);
		}

		Tree& operator=(const Tree& other)
		{
			if (this != &other)
			{
				ClearTree(pLeft);
				ClearTree(pRight);

				sym = other.sym;
				weight = other.weight;
				pLeft = new Tree();
				pRight = new Tree();

				CopyTree(pLeft, other.pLeft);
				CopyTree(pRight, other.pRight);

			}

			return *this;
		}

		void ClearTree(Tree*& tree)
		{
			if (tree != nullptr)
			{
				ClearTree(tree->pLeft);
				ClearTree(tree->pRight);


				delete tree;
				tree = nullptr;
			}
		}

		~Tree()
		{
			ClearTree(pLeft);
			ClearTree(pRight);
		}

		bool operator<(const Tree& other)
		{
			return weight < other.weight;
		}

		bool operator>(const Tree& other)
		{
			return weight > other.weight;
		}

	};

	struct Code
	{
		unsigned char code[MAX];
		unsigned int lengthOfCode;

		Code()
		{
			code[0] = 0;
			lengthOfCode = 0;
		}

		Code(const Code& other)
		{
			for (int i = 0; i < other.lengthOfCode; i++)
			{
				code[i] = other.code[i];
			}

			lengthOfCode = other.lengthOfCode;
		}

		Code& operator=(const Code& other)
		{
			if (this != &other)
			{
				for (int i = 0; i < other.lengthOfCode; i++)
				{
					code[i] = other.code[i];
				}

				lengthOfCode = other.lengthOfCode;
			}
			return *this;
		}
	};

	unsigned int treesCnt;
	unsigned int frequency[MAX];
	Heap<Tree> heap;
	Code code[MAX];
	char tempCode[1024];


public:
	Huffman();
	Huffman(Huffman&);
	Huffman& operator=(Huffman&);
	~Huffman();


private:
	void CollectFrequencyOfSymbols(const char*);
	void MakeTrees();

	void BuildHuffmanTree();
	void WriteCodes(Tree*, unsigned int);

	void WriteHuffmanTree(Tree*);
	Tree* ReadHuffmanTree(std::ifstream&);
	//helpers
private:
	void CopyFrom(Huffman& other);

public:
	//void Archive(const char*, const char*);
	void Archive(const char*,std::ofstream&);
	//void Unzip(const char*, const char*);
	void Unzip(std::ifstream&);
};


Huffman::Huffman()
{
	treesCnt = 0;
	Heap<Tree> newHeap(512);
	heap = newHeap;
	for (int i = 0; i < MAX; i++)
	{
		frequency[i] = 0;
	}
}

void Huffman::CopyFrom(Huffman& other)
{
	for (int i = 0; i < MAX; i++)
	{
		heap = other.heap;
		code[i] = other.code[i];
		frequency[i] = other.frequency[i];
	}
	treesCnt = other.treesCnt;
}

Huffman::Huffman(Huffman& other)
{
	CopyFrom(other);
}

Huffman& Huffman::operator=(Huffman& other)
{
	if (this != &other)
	{
		CopyFrom(other);
	}

	return *this;
}

Huffman::~Huffman()
{

}

void Huffman::CollectFrequencyOfSymbols(const char *strOrName)
{

	//if it directory or file 
	///TO DO: IFSTREAM FOR ALL FILES AND DIRECTS 
	/*while (*strOrName)
	{
		frequency[(unsigned char)*strOrName]++;
		*strOrName++;
	}*/

	std::ifstream readCharsFromFile(strOrName, std::ios::binary);
	unsigned char nextChar;
	while (!readCharsFromFile.eof())
	{
		readCharsFromFile.read((char*)&nextChar, sizeof(char));
		if (readCharsFromFile.eof())
			break;

		frequency[(unsigned char)nextChar]++;
	}

	readCharsFromFile.close();
}

void Huffman::MakeTrees()
{

	for (int i = 0; i < MAX; i++)
	{
		if (frequency[i])
		{
			Tree temp(i, frequency[i]);
			heap.Insert(temp);
			treesCnt++;
		}
	}
}

void Huffman::BuildHuffmanTree()
{
	Tree first, second;
	while (treesCnt > 1)
	{
		first = heap.getMin(); heap.Remove(); treesCnt--;
		second = heap.getMin(); heap.Remove(); treesCnt--;

		Tree temp;
		temp.weight = first.weight + second.weight;
		temp.pLeft = new Tree(first);
		temp.pRight = new Tree(second);

		treesCnt++;
		heap.Insert(temp);
	}

}

void Huffman::WriteCodes(Tree* tree, unsigned int index)
{

	if (tree != nullptr)
	{
		tempCode[index] = '0';
		WriteCodes(tree->pLeft, index + 1);
		if (tree->pLeft == nullptr)
		{
			tempCode[index] = 0;
			int length = std::strlen(tempCode);
			code[(unsigned char)tree->sym].lengthOfCode = length;
			for (int i = 0; i < length; i++)
			{
				code[(unsigned char)tree->sym].code[i] = tempCode[i];
			}
		}
		tempCode[index] = '1';
		WriteCodes(tree->pRight, index + 1);
	}
}

unsigned char tempCh = '\0';
unsigned short cntBitWrite = 7;
int numberOfBytesWritten = 0;
int numberTrees = 0;
inline void Huffman::WriteHuffmanTree(Tree* tree)
{
	if (tree != nullptr)
	{
		if (tree->pRight == nullptr)
		{
			tempCh = tempCh | 0 << cntBitWrite;
			tempCode[numberOfBytesWritten++] = tempCh;
			tempCode[numberOfBytesWritten++] = tree->sym;
			numberTrees++;
			//cntBitWrite=7;
		}
		else
		{
			tempCh = tempCh | 1 << cntBitWrite--;
			if (cntBitWrite < 0||cntBitWrite>8)
			{
				tempCode[numberOfBytesWritten++] = tempCh;
				tempCh = '\0';
				cntBitWrite = 7;
			}
			WriteHuffmanTree(tree->pLeft);
			WriteHuffmanTree(tree->pRight);
		}
	}
}

int BytesRead = 0;
unsigned short cntBitRead = 8;
unsigned char toRead;
inline Huffman::Tree * Huffman::ReadHuffmanTree(std::ifstream& readFromZip)
{
	if (BytesRead==40435)
	{
		BytesRead++;
		BytesRead--;
	}

	Tree *left, *right, *parent;
	if (cntBitRead > 7)
	{
		readFromZip.read((char*)&tempCh, sizeof(char)); BytesRead++;
		cntBitRead = 0;
	}
	while (cntBitRead <= 7)
	{
		toRead = 1 << 7 & tempCh << cntBitRead++;
		if (toRead == 1 << 7)
		{
			left = ReadHuffmanTree(readFromZip);
			right = ReadHuffmanTree(readFromZip);
			parent = new Tree();
			parent->pLeft = left;
			parent->pRight = right;
			return parent;
		}
		else
		{
			readFromZip.read((char*)&tempCh, sizeof(char)); BytesRead++;
			parent = new Tree(tempCh);
			readFromZip.read((char*)&tempCh, sizeof(char)); BytesRead++;
			cntBitRead--;
			return parent;
		}
	}
	return NULL;
}

void Huffman::Archive(const char* str, std::ofstream& write)
{
	CollectFrequencyOfSymbols(str);//collect frequency

	MakeTrees();//make tree for every symbol

	BuildHuffmanTree(); //connects trees into one root

	Tree* tempTree = new Tree(heap.getMin());

	WriteCodes(tempTree, 0); //write code as chars in 'CODE' struct  for every symbol in the array

	WriteHuffmanTree(tempTree);

	char temp = '\0';

	for (int i = 0; i < numberOfBytesWritten; i++)
		write.write((char*)&tempCode[i], sizeof(char));

	numberOfBytesWritten = 0;//because they
	cntBitWrite = 7;
	tempCh = '\0';         //are global

	unsigned short cntCode, cntBit = 7;
	std::ifstream readFile(str, std::ios::binary);
	//Write in arc size of file name
	int LengthOfFileName = strlen(str);
	write.write((char*)&LengthOfFileName, sizeof(int));
	//write in arc every symbol
	for (int i = 0; i < LengthOfFileName; i++)
	{
		write.write((char*)&str[i], sizeof(char));
	}
	//write in arc the number of symbols
	readFile.seekg(0, readFile.end);
	unsigned long long length = readFile.tellg() / sizeof(char); //number of symbols in the zip
	readFile.seekg(0, readFile.beg);

	write.write((char*)&length, sizeof(unsigned long long));//write number of symbols in archive

	char nextChar;

	while (true)
	{
		readFile.read((char*)&nextChar, sizeof(char));
		if (readFile.eof())
		{
			if (cntBit != 0)
				write.write((char*)&temp, sizeof(char));

			break;
		}

		cntCode = 0;
		while (cntCode < code[(unsigned char)nextChar].lengthOfCode)
		{

			if (code[(unsigned char)nextChar].code[cntCode] == '1')
			{
				temp = temp | 1 << cntBit;
			}
			else if (code[(unsigned char)nextChar].code[cntCode] == '0')
			{
				temp = temp | 0 << cntBit;
			}

			if (cntBit == 0)
			{
				write.write((char*)&temp, sizeof(char));
				cntBit = 8;
				temp = '\0';

			}
			cntCode++;
			cntBit--;
		}
	}

	readFile.close();
}

void Huffman::Unzip(std::ifstream& zip)//, const char* whereToUnzip)
{

	BytesRead = 0;
	cntBitRead = 8;
	tempCh = '\0';

	unsigned char zippedChars;
//	std::ifstream zip(FileName, std::ios::binary);
	unsigned long long numberofSymbols, symCnt = 0;
	unsigned short cntBit = 8, zeroOrOne;
	int fileNameSize;
	std::string fileName = "";
	Tree* tree = ReadHuffmanTree(zip);
	Tree* checkNextTree = tree;


	zip.seekg(-1, zip.cur);
	//read the number of chars of the fileNAme
	zip.read((char*)&fileNameSize, sizeof(int));
	//read evcery char and put it in string
	for (int i = 0; i < fileNameSize; i++)
	{
		zip.read((char*)&zippedChars, sizeof(char));
		fileName += zippedChars;
	}
	zip.read((char*)&numberofSymbols, sizeof(unsigned long long));

	std::ofstream result(fileName, std::ios::binary);


	while (symCnt < numberofSymbols)
	{
		if (cntBit > 7)
		{
			zip.read((char*)&zippedChars, sizeof(char));
			cntBit = 0;
		}
		if (zip.eof())
			break;

		while (cntBit <= 7)
		{
			if (symCnt == numberofSymbols)break;
			zeroOrOne = 1 << 7 & zippedChars << cntBit;
			cntBit++;

			if (zeroOrOne == 1 << 7)
				checkNextTree = checkNextTree->pRight;
			else
				checkNextTree = checkNextTree->pLeft;

			if (checkNextTree->pRight == nullptr)
			{
				result.write((char*)&checkNextTree->sym, sizeof(unsigned char));
				checkNextTree = tree;
				symCnt++;
			}
		}
	}
	result.close();
}