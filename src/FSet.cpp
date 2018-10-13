
#include "FSet.h"

FSet::FSet(bool _ok)
{
	set = new int32_t[size];
	memset(set, 0, size*sizeof(int32_t));
	ok = _ok;
}

FSet::~FSet()
{
	delete set;
}

bool FSet::HasMember(int32_t _k)
{
	for (int i = 0; i < tail; i++)
	{
		if (set[i] == _k)
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
				for (int i = 0; i < tail; i++)
				{
					if (set[i] == -1)
					{
						set[i] = _op->key;
						_op->done = true;
					}
				}
				if (!_op->done)
				{
					set[tail] = _op->key;
					tail++;
				}
			}
		}
		else
		{
			_op->resp = (HasMember(_op->key));
			if (_op->resp)
			{
				for (int i = 0; i < tail; i++)
				{
					if (set[i] == _op->key)
						set[i] = -1;
				}
			}
				
		}
		_op->done = true;
	}

	return _op->done;
}

int32_t* FSet::Freeze()
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