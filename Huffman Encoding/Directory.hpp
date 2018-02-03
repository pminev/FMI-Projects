#pragma once
#include <iostream>
#include <dirent.h>
#include <direct.h>
#include <string>
#include <fstream>
#include "Huffman.hpp"

class DirectoryTree
{
private:
	struct DirTree
	{
		std::string name;
		int numberOfFolders;
		int numberOfFiles;
		DirTree** folders;
		std::string* files;

		DirTree()
		{
			name = "";
			numberOfFolders = 0;
			numberOfFiles = 0;
			folders = nullptr;
			files = nullptr;
		}

		DirTree(std::string name, int fold=0, int file=0, DirTree** folders = nullptr, std::string* fils = nullptr)
		{
			this->name = name;
			this->numberOfFolders = fold;
			this->numberOfFiles = file;
			this->folders = folders;
			this->files = fils;
		}

		void CopyTree(DirTree*& newTree,DirTree*& toCopy)
		{
			if (toCopy == nullptr)
			{
				newTree = nullptr;
				return;
			}

			newTree = new DirTree(toCopy->name, toCopy->numberOfFolders, toCopy->numberOfFiles);
			newTree->files = new std::string[numberOfFiles];
			for (int i = 0; i < numberOfFiles; i++)
			{
				newTree->files[i] = toCopy->files[i];
			}

			newTree->folders = new DirTree*[numberOfFolders];
			for (int i = 0; i < numberOfFolders; i++)
			{
				newTree->folders[i] = new DirTree();
				CopyTree(newTree->folders[i], toCopy->folders[i]);
			}

		}

		DirTree(const DirTree& other)
		{
			this->name = other.name;
			this->numberOfFiles = other.numberOfFiles;
			this->numberOfFolders = other.numberOfFolders;
			this->files = new std::string[numberOfFiles];
			for (int i = 0; i < numberOfFiles; i++)
			{
				this->files[i] = other.files[i];
			}

			this->folders = new DirTree*[numberOfFolders];
			for (int i = 0; i < numberOfFolders; i++)
			{
				this->folders[i] = new DirTree();
				CopyTree(this->folders[i], other.folders[i]);
			}

		}

		DirTree& operator=(const DirTree& other)
		{
			if (this != &other)
			{
				//clear memory
				delete[] this->files;
				this->files = nullptr;
				for (int i = 0; i < numberOfFolders; i++)
				{
					ClearTree(other.folders[i]);
				}
				//end of clearing memory

				this->name = other.name;
				this->numberOfFiles = other.numberOfFiles;
				this->numberOfFolders = other.numberOfFolders;
				this->files = new std::string[numberOfFiles];
				for (int i = 0; i < numberOfFiles; i++)
				{
					this->files[i] = other.files[i];
				}

				this->folders = new DirTree*[numberOfFolders];
				for (int i = 0; i < numberOfFolders; i++)
				{
					this->folders[i] = new DirTree();
					CopyTree(this->folders[i], other.folders[i]);
				}
			}

			return *this;
		}

		void ClearTree(DirTree*& tree)
		{
			if (tree != nullptr)
			{
				delete[] tree->files;
				tree->files = nullptr;

				for (int i = 0; i < tree->numberOfFolders; i++)
				{
					ClearTree(tree->folders[i]);

					delete tree->folders[i];
					tree->folders[i] = nullptr;

				}
			}
		}

		~DirTree()
		{
			delete[] this->files;
			this->files = nullptr;

			for (int i = 0; i < numberOfFolders; i++)
			{
				ClearTree(this->folders[i]);

				delete this->folders[i];
				this->folders[i] = nullptr;
			}
		}
	};

	DirTree *root;

public:
	DirectoryTree();
	DirectoryTree(DirectoryTree&);
	DirectoryTree& operator=(const DirectoryTree&);
	~DirectoryTree();

private:
	void CopyTree(DirTree*&, DirTree*);
	void CopyFrom(const DirectoryTree&);
	void CleanTree(DirTree*);

	bool IsFile(std::string);

	void CountFilesAndFolders(std::string, DirTree*&);
	void WriteFilesAndFoldersNames(std::string, DirTree*&);

	void BuildDirTree(std::string, DirTree*&);

	void WriteDirTree(DirTree*, std::ofstream&);
	void ReadDirTree(std::ifstream&, DirTree*&);

	void ZipEveryFolder(DirTree*&, std::ofstream&);
	void UnzipEveryFolder(DirTree*&, std::ifstream&);

public:
	void Zip(std::string,std::string);
	void Unzip(std::string, std::string);


};

DirectoryTree::DirectoryTree()
{
	root = new DirTree();
}

void DirectoryTree::CopyTree(DirTree*& tree,DirTree* other)
{
	if (other==nullptr)
	{
		tree = nullptr;
		return;
	}

	tree = new DirTree(other->name, other->numberOfFolders, other->numberOfFiles);
	tree->files = new std::string[tree->numberOfFiles];
	for (int i = 0; i < tree->numberOfFiles; i++)
	{
		tree->files[i] = other->files[i];
	}

	tree->folders = new DirTree*[tree->numberOfFolders];
	for (int i = 0; i < tree->numberOfFolders; i++)
	{
		CopyTree(tree->folders[i], other->folders[i]);
	}
}

void DirectoryTree::CopyFrom(const DirectoryTree& other)
{
	CopyTree(root, other.root);
}
DirectoryTree::DirectoryTree(DirectoryTree& other)
{
	CopyFrom(other);
}

DirectoryTree& DirectoryTree::operator=(const DirectoryTree& other)
{
	if (this!=&other)
	{
		CleanTree(root);
		root = nullptr;
		CopyFrom(other);
	}

	return *this;
}

void DirectoryTree::CleanTree(DirTree* tree)
{
	if (tree!= nullptr)
	{
		if (tree->files!= nullptr)
		{
			delete[] tree->files;
			tree->files = nullptr;
		}

		for (int i = 0; i < tree->numberOfFolders; i++)
		{
			CleanTree(tree->folders[i]);
			delete tree->folders[i];
			tree->folders[i] = nullptr;
		}
	}
}

DirectoryTree::~DirectoryTree()
{
	CleanTree(root);
}

bool DirectoryTree::IsFile(std::string name)
{
	DIR* directory = opendir(name.c_str());

	if (directory != NULL)
	{
		closedir(directory);
		return 0;
	}

	if (errno == ENOTDIR)
	{
		return 1;
	}

	return -1;
}

//Count files and folders in the directory to know how much memory to allocate
void DirectoryTree::CountFilesAndFolders(std::string name,DirTree*& tree)
{
	DIR *dir = 0;
	struct dirent *dp;
	dir = opendir(name.c_str());
	while ((dp = readdir(dir)) != NULL)
	{
		std::string fileDir = name + '/' + dp->d_name;

		if (!IsFile(fileDir.c_str()) && strcmp(dp->d_name, "$RECYCLE.BIN")
			&& strcmp(dp->d_name, ".") && strcmp(dp->d_name, ".."))
		{
			tree->numberOfFolders++;
		}
		else if (IsFile(fileDir.c_str()))
		{
			tree->numberOfFiles++;
		}
	}
}

void DirectoryTree::WriteFilesAndFoldersNames(std::string name , DirTree*& tree)
{
	DIR *dir = 0;
	struct dirent *dp;
	dir = opendir(name.c_str());
	int cntFiles = 0;
	int cntFolders=0;
	while ((dp = readdir(dir)) != NULL)
	{
		std::string fileDir = name + '/' + dp->d_name;

		if (!IsFile(fileDir.c_str()) && strcmp(dp->d_name, "$RECYCLE.BIN")
			&& strcmp(dp->d_name, ".") && strcmp(dp->d_name, ".."))
		{
			tree->folders[cntFolders] = new DirTree(dp->d_name);
			cntFolders++;
		}
		else if (IsFile(fileDir.c_str()))
		{
			tree->files[cntFiles] = fileDir;
			cntFiles++;
		}
	}
}


void DirectoryTree::BuildDirTree(std::string name,DirTree*& tree)
{
	CountFilesAndFolders(name, tree);
	if (tree->numberOfFiles)
		tree->files = new std::string[tree->numberOfFiles];
	if (tree->numberOfFolders)
		tree->folders = new DirTree*[tree->numberOfFolders];

	WriteFilesAndFoldersNames(name, tree);

	for (int i = 0; i < tree->numberOfFolders; i++)
	{
		//std::string FolderDir = name + '/' + root->folders[i]->name;
		BuildDirTree(name + '/' + tree->folders[i]->name, tree->folders[i]);
	}
}

void DirectoryTree::WriteDirTree(DirTree* tree,std::ofstream& writeStream)
{
	if (tree!=nullptr)
	{
		int lenName = strlen(tree->name.c_str());
		writeStream.write((char*)&lenName, sizeof(int));
		for (int i = 0; i < lenName; i++)
		{
			writeStream.write((char*)&tree->name[i], sizeof(char));
		}

		writeStream.write((char*)&tree->numberOfFiles, sizeof(int));
		writeStream.write((char*)&tree->numberOfFolders, sizeof(int));

		for (int i = 0; i < tree->numberOfFolders; i++)
		{
			WriteDirTree(tree->folders[i], writeStream);
		}
	}
}

void DirectoryTree::ReadDirTree(std::ifstream& readStream, DirTree*& tree)
{
	if (tree!=nullptr)
	{
		int lenName;
		readStream.read((char*)&lenName, sizeof(int));
		char tempCh;
		for (int i = 0; i < lenName; i++)
		{
			readStream.read((char*)&tempCh, sizeof(char));
			tree->name += tempCh;
		}

		readStream.read((char*)&tree->numberOfFiles, sizeof(int));
		readStream.read((char*)&tree->numberOfFolders, sizeof(int));
		tree->folders = new DirTree*[tree->numberOfFolders];
		for (int i = 0; i < tree->numberOfFolders; i++)
		{
			tree->folders[i] = new DirTree();
			ReadDirTree(readStream, tree->folders[i]);
		}
	}
}


void DirectoryTree::ZipEveryFolder(DirTree*& tree, std::ofstream& writeStream)
{
	Huffman huffTree;
	for (int i = 0; i < tree->numberOfFiles; i++)
	{
		huffTree.Archive((tree->files[i]).c_str(), writeStream);
	}

	for (int i = 0; i < tree->numberOfFolders; i++)
	{
		ZipEveryFolder(tree->folders[i], writeStream);
	}
}

void DirectoryTree::Zip(std::string fileName, std::string whereToZip)
{
	root->name = fileName;
	BuildDirTree(fileName, root);

	std::ofstream writeArc(whereToZip, std::ios::binary);
	WriteDirTree(root, writeArc);
	ZipEveryFolder(root, writeArc);

	writeArc.close();
}

void DirectoryTree::UnzipEveryFolder(DirTree*& tree, std::ifstream& readStream)
{
	Huffman huff;
	for (int i = 0; i < tree->numberOfFiles; i++)
	{
		huff.Unzip(readStream);
	}

	for (int i = 0; i < tree->numberOfFolders; i++)
	{
		UnzipEveryFolder(tree->folders[i], readStream);
	}
}

void DirectoryTree::Unzip(std::string arcFileName, std::string whereToUnzip)
{
	DirTree* tempTree=new DirTree();
	std::ifstream arc(arcFileName, std::ios::binary);
	ReadDirTree(arc, tempTree);
	std::ofstream unzipArc(whereToUnzip, std::ios::binary);
	UnzipEveryFolder(root, arc);
	arc.close();
	unzipArc.close();
	
}