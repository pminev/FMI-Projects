#include <iostream>
#include <iomanip>
#include <limits>
#include "Table.h"
#include <fstream>

char* editParams(unsigned short& row, unsigned short& col, const char* buff,unsigned short com)	
{
	char rowChars[8], colChars[8];
	bool findNum = false;
	int len = strlen(buff);
	for (int i = com + 1, j = 0; i < len; i++, j++)
	{
		if (buff[i] == ' '&&findNum)
		{
			rowChars[j] = 0;
			break;
		}
		if (buff[i] >= '0' || buff[i] <= '9')
		{
			findNum = true;
			rowChars[j] = buff[i];
		}
	}
	row = atoi(rowChars);

	findNum = false;
	for (int i = com + 1+strlen(rowChars), j = 0; i < len; i++, j++)
	{
		if (buff[i] == ' '&&findNum)
		{
			colChars[j] = 0;
			break;
		}
		if (buff[i] >= '0' || buff[i] <= '9')
		{
			findNum = true;
			colChars[j] = buff[i];
		}
	}
	col = atoi(colChars);

	char fileName[128];
	for (int i = com +strlen(rowChars)+strlen(colChars)+2,j=0; i < len; i++,j++)
	{
		if (buff[i] != ' ')
		{
			fileName[j] = buff[i];
		}

		if (i==len-1)
		{
			fileName[j + 1] = 0;
		}
	}	

	char* cellVal = new char[strlen(fileName) + 1];
	strcpy(cellVal, fileName);

	return cellVal;
}

int main(){

	Table table;
	bool exit=false;
	unsigned short lineLength,endOfCommand;
	while (!exit)
	{

		char buff[256], command[8], fileName[64];
		command[0] = 0;
		fileName[0] = 0;
		std::cin.getline(buff, 256);;
		lineLength = strlen(buff);
		for (int i = 0; i <= lineLength; i++)
		{
			if (buff[i]==' ')
			{
				command[i] = 0;
				endOfCommand = i;
				break;
			}
				command[i] = buff[i];

		}
		
		if (!strcmp(command,"exit"))
		{
			exit = true;
		}
		else if (!strcmp(command, "open"))
		{
			for (int i = endOfCommand + 1,j=0; i < lineLength; i++,j++)
			{
				if (buff[i] != ' ')
				{
					fileName[j] = buff[i];
				}
				if (i==lineLength-1)
				{
					fileName[j + 1] = 0;
				}
			}

			table.open(fileName);
		}
		else if (!strcmp(command, "save"))
		{
			table.save();
		}
		else if (!strcmp(command, "saveas"))
		{
			for (int i = endOfCommand + 1; i < lineLength; i++)
			{
				if (buff[i] != ' ')
				{
					fileName[i] == buff[i];
				}
			}

			table.saveAs(fileName);
		}
		else if (!strcmp(command, "close"))
		{
			table.close();
		}
		else if (!strcmp(command, "edit"))
		{
			unsigned short row, col;
			char * cellVal = editParams(row, col, buff, endOfCommand);

			table.edit(row,col,cellVal);

			delete[] cellVal;
		}
		else if (!strcmp(command, "print"))
		{
			table.print();
		}

	}


	return 0;
}