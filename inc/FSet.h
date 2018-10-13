
#pragma once
#ifndef __FSET_H__
#define __FSET_H__

#include <atomic>
#include <stdint.h>s

using namespace std;

class FSet
{

public:

	struct Set {
		int32_t* values;
		int32_t size = 1000;

		Set();
	};

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

		FSetOp() = delete;
		FSetOp(OpType _type, int32_t _k);
		bool GetResponse();
	};

	FSet() = delete;
	FSet(FSet::Set* _set, bool _ok);
	~FSet();

	bool HasMember(int32_t _k);
	bool Invoke(FSetOp* _op);
	FSet::Set* Freeze();

private:

	Set* set;
	bool ok;
};

#endif // __FSET_H__