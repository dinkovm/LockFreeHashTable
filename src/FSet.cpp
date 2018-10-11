
#include "FSet.h"

FSet::FSet(size_t _size, bool _ok)
{
	ok = _ok;
}

FSet::~FSet()
{
	set.clear();
}

bool FSet::HasMember(int32_t _k)
{
	if (set.find(_k) != set.end())
		return true;
	else
		return false;
}

bool FSet::Invoke(FSetOp _op)
{
	if (ok && !_op.done)
	{
		if (_op.type == true)
		{
			_op.resp = !(HasMember(_op.key));
			if (_op.resp)
				set.insert(_op.key);
		}
		else
		{
			_op.resp = (HasMember(_op.key));
			if (_op.resp)
				set.erase(_op.key);
		}
		_op.done = true;
	}

	return _op.done;
}

std::set<int32_t> FSet::Freeze()
{
	if (ok)
		ok = false;
	return set;
}