

#include "LFHashTable.h"
#include <fstream>
#include <iostream>

#include <vector>
#include <chrono>
#include <thread>
#include <atomic>

using namespace LockFree;
using namespace std;

enum OpType : uint8_t
{
	INSERT,
	REMOVE,
	CONTAINS
};

struct Operation
{
	OpType opType;
	int32_t key;
};

#define DEFAULT_NUM_OPS 500000
vector<Operation> operations(DEFAULT_NUM_OPS);
atomic<uint32_t> currOp;

#define NUM_THREADS 2
thread* threads[2];

int main(int argc, char *argv[])
{
	HashTable ht(4u, 16u, 32u);

	ifstream file;
	char* command = new char[256];
	file.open(argv[1]);

	if (!file)
	{
		cout << "Unable to open file!";
		exit(1);
	}

	while (!file.eof())
	{
		Operation op;

		file >> command;
		file >> op.key;

		if (strcmp(command, "INSERT") == 0)
		{
			op.opType = INSERT;
		}
		else if (strcmp(command, "REMOVE") == 0)
		{
			op.opType = REMOVE;
		}
		else if (strcmp(command, "CONTAINS") == 0)
		{
			op.opType = CONTAINS;
		}
		else
		{
			cout << "Invalid operation type: " << command << "!" << endl;
			exit(1);
		}

		operations.push_back(op);
	}

	chrono::time_point<std::chrono::system_clock> time = chrono::system_clock::now();

#if 0 // Single-Threaded

	for (auto op : operations)
	{
		switch (op.opType)
		{
		case INSERT:
			ht.Insert(op.key);
			break;
		case REMOVE:
			ht.Remove(op.key);
			break;
		case CONTAINS:
			ht.Contains(op.key);
			break;
		}
	}

#else // Multi-Threaded

	// Spawn Threads
	for (size_t i = 0; i < NUM_THREADS; i++)
	{
		threads[i] = new thread()
	}
#endif

	std::cout << "Elapsed Time: " <<
		chrono::duration_cast<std::chrono::milliseconds>(chrono::system_clock::now() - time).count() << endl;
	
	file.close();
	delete[] command;

	return 0;
}

void RunThread()
{
	uint32_t curr;

	while ()
}