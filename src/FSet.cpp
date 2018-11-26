
#include "FSet.h"

FSet::FSet(Set* _set, bool _ok)
{
	if (_set == nullptr)
	{
		_set = new Set();
	}

	set = _set;
	ok = _ok;

	size.store(0, memory_order_relaxed);
}

FSet::~FSet()
{
	delete set;
}

bool FSet::HasMember(int32_t _k)
{
	return set->Contains(_k);
}

bool FSet::Invoke(FSetOp* _op)
{
	bool resp = false;

	if (ok && !_op->done)
	{
		if (_op->type == FSetOp::INSERT)
		{
			if (set->Insert(_op->key))
			{
				resp = true;
				//size.fetch_add(1);
			}
		}
		else // FSetOp::REMOVE
		{
			if (set->Remove(_op->key))
			{
				resp = true;
				//size.fetch_sub(1);
			}
		}
		_op->resp = resp;
		_op->done = true;
	}

	return _op->done;
}

Set* FSet::Freeze()
{
	if (ok)
		ok = false;
	return set;
}

FSet::FSetOp::FSetOp(OpType _type, int32_t _k)
{
	type = _type;
	key = _k;
	done.store(false, memory_order_relaxed);
	resp.store(false, memory_order_relaxed);
}

bool FSet::FSetOp::GetResponse()
{
	return resp;
}

int32_t FSet::GetSize()
{
	return size;
}