
#include "FSet.h"

FSet::FSet(size_t _size, bool _ok)
{
    set = new int32_t[_size];
    ok = _ok;
}

FSet::~FSet()
{
    delete[] set;
}