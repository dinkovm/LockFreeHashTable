

#include "LFHashTable.h"

using namespace LockFree;
using namespace std;

int main()
{
	HashTable ht;

	bool ins4 = ht.Insert(4);
	bool has4 = ht.Contains(4);
	bool has5 = ht.Contains(5);
	bool rem4 = ht.Remove(4);
	has4 = ht.Contains(4);

	return 0;
}