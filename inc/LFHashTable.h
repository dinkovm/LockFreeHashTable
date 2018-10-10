
#pragma once
#ifndef __LFHASHTABLE_H__
#define __LFHASHTABLE_H__

#include "FSet.h"

namespace LockFree
{
	class HashTable
	{
	public:

		struct HNode
		{
			atomic<FSet*>* buckets;
			size_t size;
			atomic<HNode*> pred;

			HNode(size_t _size);
		};

		HashTable();
		~HashTable();

		bool Insert(int32_t _k);
		bool Remove(int32_t _k);
		bool Contains(int32_t _k);

	private:

		using OpType = FSet::FSetOp::OpType;

		atomic<HNode*> m_head;

		void Resize(bool _grow);
		bool Apply(OpType _type, int32_t _k);
		FSet* InitBucket(HNode* _t, size_t _i);

	};
}

#endif // __LFHASHTABLE_H__