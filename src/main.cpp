

#include "LFHashTable.h"
#include <fstream>
#include <iostream>

#include <vector>
#include <chrono>
#include <thread>
#include <atomic>
#include <cstring>

#include <assert.h>

#if _WIN32
#include <Windows.h>

SYNCHRONIZATION_BARRIER barrier;
#endif

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
atomic<uint32_t> current;

HashTable ht(4u, 16u, 32u);

chrono::time_point<std::chrono::system_clock> startTime;

void RunThread(bool mainThread)
{
	uint32_t index = 0u;
	uint32_t numOps = static_cast<uint32_t>(operations.size());

#if _WIN32
	EnterSynchronizationBarrier(&barrier, SYNCHRONIZATION_BARRIER_FLAGS_NO_DELETE);
#endif

	if (mainThread)
	{
		// Start timer and release barrier
		startTime = chrono::system_clock::now();
	}

	while (true)
	{
		index = current.fetch_add(1u);

		if (index >= numOps)
		{
			break;
		}

		Operation& op = operations[index];

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
		default:
			assert(false);
			break;
		}
	}
}

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		cout << "Invalid number of arguments!\n";
		cout << "Usage: LockFreeHashTable.exe \"Test Data\" \"Number of Threads\"" << endl;
		exit(1);
	}

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

	const uint32_t NUM_WORKER_THREADS = static_cast<uint32_t>(atoi(argv[2])) - 1;
	thread** threads;
	if (NUM_WORKER_THREADS > 0)
	{
		threads = new thread*[NUM_WORKER_THREADS];
	}

#ifdef _WIN32
	InitializeSynchronizationBarrier(&barrier, NUM_WORKER_THREADS + 1, -1);
#endif

	current.store(0u);

	// Spawn Worker Threads
	for (size_t i = 0; i < NUM_WORKER_THREADS; i++)
	{
		threads[i] = new thread(RunThread, false);
	}

	// Use Main Thread as a Worker Thread
	RunThread(true);

	// Join Worker Threads
	for (size_t i = 0; i < NUM_WORKER_THREADS; i++)
	{
		threads[i]->join();
	}

	std::cout << "Elapsed Time: " <<
		chrono::duration_cast<std::chrono::milliseconds>(chrono::system_clock::now() - startTime).count() << endl;

	// Delete Worker Threads
	for (size_t i = 0; i < NUM_WORKER_THREADS; i++)
	{
		delete threads[i];
	}
	
	file.close();
	delete[] command;
	
	if (NUM_WORKER_THREADS > 0)
	{
		delete[] threads;
	}

	return 0;
}