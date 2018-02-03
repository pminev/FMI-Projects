#include <dirent.h>
#include <direct.h>
#include <string>
#include "Huffman.hpp"
#include "Heap.hpp"
#include <iostream>
#include "Directory.hpp"


/**
*
* Solution to Course Work
* Data Structures Course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2016/2017
*
* @author Plamen Minev
* @idnumber 45212
* @task 1
* @compiler VC
*
*/


int main(int argc,char* argv[])
{
	std::string codes[3];
	char buffer[512];
	DirectoryTree tree;
	std::cin.getline(buffer, 512);

	int lenbuffer=strlen(buffer);
	int cnt = 0;
	for (int i = 0; i < lenbuffer; i++)
	{
		if (buffer[i]==' '&&buffer[i+2]==':')
		{
			cnt++;
		}
		else
		{
			codes[cnt] += buffer[i];		
		}
	}

	if (codes[0]=="-Pack")
	{
		tree.Zip(codes[1], codes[2]);
	}
	else if (codes[0] == "-Unpack")
	{
		tree.Unzip(codes[1], codes[2]);
	}
	else if (codes[0] == "-List")
	{
		//not implemented yet....
		//The best is yet to come... :D
	}

	

	return 0;
}