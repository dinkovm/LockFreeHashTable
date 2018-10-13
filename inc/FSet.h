
#pragma once
#ifndef __FSET_H__
#define __FSET_H__

#include <atomic>
#include <stdint.h>
#include <set>

using namespace std;

class FSet
{
	int32_t* set;
	bool ok;
	atomic<int32_t> tail;
	int32_t size = 1000;

public:

	FSet() = delete;
	FSet(bool _ok);
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
	int32_t* Freeze();
};

#endif // __FSET_H__