
#include "FSet.h"

FSet::Window::Window(FSet::Node* _pred, FSet::Node* _curr)
{
	pred = _pred;
	curr = _curr;
}


FSet::Set::Set()
{
	values = new int32_t[size];
	memset(values, -1, size * sizeof(int32_t));
}

FSet::FSet(FSet::Set* _set, bool _ok)
{
	set = _set;
	ok = _ok;
}

FSet::~FSet()
{
	delete set;
}

bool FSet::HasMember(int32_t _k)
{
	for (int i = 0; i < set->size; i++)
	{
		if (set->values[i] == _k)
			return true;
	}
	return false;
}

bool FSet::Invoke(FSetOp* _op)
{
	if (ok && !_op->done)
	{
		if (_op->type == true)
		{
			_op->resp = !(HasMember(_op->key));
			if (_op->resp)
			{
				for (int i = 0; i < set->size; i++)
				{
					if (set->values[i] == -1)
					{
						set->values[i] = _op->key;
					}
				}
			}
		}
		else
		{
			_op->resp = (HasMember(_op->key));
			if (_op->resp)
			{
				for (int i = 0; i < set->size; i++)
				{
					if (set->values[i] == _op->key)
						set->values[i] = -1;
				}
			}
				
		}
		_op->done = true;
	}

	return _op->done;
}

FSet::Set* FSet::Freeze()
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