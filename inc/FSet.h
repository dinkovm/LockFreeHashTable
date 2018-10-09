
#pragma once
#indef __FSET_H__
#define __FSET_H__

#include <atomic>
#include <stdint.h>

using namespace std;

class FSet
{
	int32_t* set;
	bool ok;

public:

    FSet() = delete;
    FSet(size_t _size, bool _ok);
    ~FSet();

	struct  FSetOp
	{
		enum OpType : bool
		{
			INSERT = true,
			REMOVE = false
		} type;

	    int32_t key;
		atomic<bool> done;
		atomic<bool> resp;

        bool GetResponse();
	};

	bool HasMember(int32_t _k);
	bool Invoke(FSetOp _op);
};

#endif // __FSET_H__