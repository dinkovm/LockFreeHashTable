

#include "LFHashTable.h"
#include <fstream>
#include <iostream>

using namespace LockFree;
using namespace std;

int main(int argc, char *argv[])
{
	HashTable ht;
	ifstream inFile;
	char* command = new char[256];
	inFile.open(argv[1]);
	if (!inFile)
	{
		cout << "Unable to open file!";
		exit(1);
	}
	for (int i = 0; i < 500000; i++)
	{
		inFile >> command;
		int value;
		inFile >> value;
		if (command == "INSERT")
		{
			ht.Insert(value);
		}
		else if (command == "REMOVE")
		{
			ht.Remove(value);
		}
		else
		{
			ht.Contains(value);
		}
	}
	
	inFile.close();
	delete[] command;

	/*bool ins4 = ht.Insert(4);
	bool has4 = ht.Contains(4);
	bool has5 = ht.Contains(5);
	bool rem4 = ht.Remove(4);
	has4 = ht.Contains(4);*/

	return 0;
}