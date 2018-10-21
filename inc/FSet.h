
#pragma once
#ifndef __FSET_H__
#define __FSET_H__

#include <atomic>
#include <stdint.h>

#include "Set.h"

using namespace std;

class FSet
{
public:

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
	FSet(Set* _set, bool _ok);
	~FSet();

	bool HasMember(int32_t _k);
	bool Invoke(FSetOp* _op);
	Set* Freeze();
	int32_t GetSize();

private:

	Set* set;
	bool ok;

	atomic<int32_t> size;
};

#endif // __FSET_H__