
#include "LFHashTable.h"

namespace LockFree
{
	HashTable::HNode::HNode(size_t _size)
	{
		size = _size;
		buckets = new atomic<FSet*>[_size];
		pred = nullptr;

		for (size_t i = 0u; i < _size; i++)
		{
			buckets->store(nullptr, memory_order_relaxed);
		}
	}

	HashTable::HashTable()
	{
		HNode* head = new HNode(1);
		head->buckets[0].store(
			new FSet(0u, true), 
			memory_order_relaxed);

		m_head.store(head, memory_order_release);
	}

	bool HashTable::Insert(int32_t k)
	{
		bool response = Apply(OpType::INSERT, k);

		// if ()
		{
			Resize(true);
		}

		return response;
	}

	bool HashTable::Remove(int32_t k)
	{
		bool response = Apply(OpType::REMOVE, k);

		// if ()
		{
			Resize(true);
		}

		return response;
	}

	bool HashTable::Contains(int32_t k)
	{
		HNode* t = m_head.load(memory_order_acquire);
		FSet* b = t->buckets[k % t->size];

		if (b == nullptr)
		{
			HNode* s = t->pred;

			b = (s != nullptr) ? 
				s->buckets[k % t->size] : 
				t->buckets[k % t->size];
		}

		return b->HasMember(k);
	}

	void HashTable::Resize(bool grow)
	{
		HNode* t = m_head.load(memory_order_acquire);

		if ((t->size > 1u) || grow)
		{
			for (size_t i = 0; i < t->size; i++)
			{
				InitBucket(t, i);
			}

			t->pred = nullptr;

			size_t size = grow ? t->size * 2 : t->size / 2;
			HNode* head = new HNode(size);
			m_head.compare_exchange_strong(t, head, memory_order_release);
		}
	}

	bool HashTable::Apply(OpType op, int32_t k)
	{

	}

	FSet* HashTable::InitBucket(HNode* _t, size_t _i)
	{

	}
}