
#include "FSet.h"

FSet::FSet(Set* _set, bool _ok)
{
	if (_set == nullptr)
	{
		_set = new Set();
	}

	set = _set;
	ok = _ok;
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
	if (ok && !_op->done)
	{
		if (_op->type == FSetOp::INSERT)
		{
			_op->resp = !(HasMember(_op->key));
			if (_op->resp)
			{
				set->Insert(_op->key);
			}
		}
		else // FSetOp::REMOVE
		{
			_op->resp = (HasMember(_op->key));
			if (_op->resp)
			{
				set->Remove(_op->key);
			}
		}
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