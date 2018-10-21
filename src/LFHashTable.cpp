
#include "LFHashTable.h"

namespace LockFree
{
	HashTable::HNode::HNode(size_t _size, HNode* _pred)
	{
		size = _size;
		buckets = new atomic<FSet*>[_size];
		pred = _pred;

		for (size_t i = 0u; i < _size; i++)
		{
			buckets[i].store(nullptr, memory_order_relaxed);
		}
	}

	HashTable::HashTable(uint32_t _lowerThresh, uint32_t _upperThresh, size_t _maxBins) :
		m_lowerThresh(static_cast<int32_t>(_lowerThresh)),
		m_upperThresh(static_cast<int32_t>(_upperThresh)),
		m_maxBins(_maxBins)
	{
		HNode* head = new HNode(1, nullptr);
		head->buckets[0].store(
			new FSet(nullptr, true),
			memory_order_relaxed);

		m_head.store(head, memory_order_release);
	}

	bool HashTable::Insert(int32_t _k)
	{
		bool response = Apply(OpType::INSERT, _k);

		if (m_grow.load(memory_order_relaxed))
		{
			Resize(true);
		}

		return response;
	}

	bool HashTable::Remove(int32_t _k)
	{
		bool response = Apply(OpType::REMOVE, _k);

		if (m_shrink.load(memory_order_relaxed))
		{
			Resize(false);
		}

		return response;
	}

	bool HashTable::Contains(int32_t _k)
	{
		HNode* t = m_head.load(memory_order_acquire);
		FSet* b  = t->buckets[_k % t->size];

		if (b == nullptr)
		{
			HNode* s = t->pred;

			b = (s != nullptr) ? 
				s->buckets[_k % s->size] : 
				t->buckets[_k % t->size];
		}

		return b->HasMember(_k);
	}

	void HashTable::Resize(bool _grow)
	{
		HNode* t = m_head.load(memory_order_acquire);

		if (((t->size > 1u) || _grow) && 
			((t->size < m_maxBins || !_grow)))
		{
			for (size_t i = 0; i < t->size; i++)
			{
				InitBucket(t, i);
			}

			t->pred = nullptr;

			size_t size = _grow ? t->size * 2 : t->size / 2;
			HNode* head = new HNode(size, t);
			m_head.compare_exchange_strong(
				t, 
				head, 
				memory_order_release);

			bool expected = true;
			bool desired = false;

			if (_grow)
			{
				m_grow.compare_exchange_strong(expected, desired, memory_order_relaxed);
			}
			else
			{
				m_shrink.compare_exchange_strong(expected, desired, memory_order_relaxed);
			}
		}
	}

	bool HashTable::Apply(OpType _type, int32_t _k)
	{
		FSet::FSetOp op(_type, _k);

		while (true)
		{
			HNode* t = m_head.load(memory_order_acquire);
			FSet* b  = t->buckets[_k % t->size];

			if (b == nullptr)
			{
				b = InitBucket(t, _k % t->size);
			}
			
			if (b->Invoke(&op))
			{
				int32_t size = b->GetSize();

				if (size > m_upperThresh)
				{
					bool expected = false;
					bool desired = true;

					m_grow.compare_exchange_strong(expected, desired, memory_order_relaxed);
				}
				else if(size < m_lowerThresh)
				{
					bool expected = false;
					bool desired = true;

					m_shrink.compare_exchange_strong(expected, desired, memory_order_relaxed);
				}

				return op.GetResponse();
			}
		}
	}

	FSet* HashTable::InitBucket(HNode* _t, size_t _i)
	{
		FSet* b  = _t->buckets[_i];
		HNode* s = _t->pred;

		if ((b == nullptr) &&
			(s != nullptr))
		{
			Set* set_new = nullptr;

			if (_t->size == s->size * 2)
			{
				FSet* m = s->buckets[_i % s->size];
				
				set_new = m->Freeze()->IntersectRemainder(
					static_cast<int32_t>(_t->size), _i);
			}
			else
			{
				FSet* m = s->buckets[_i];
				FSet* n = s->buckets[_i + _t->size];
				
				set_new = m->Freeze()->Union(n->Freeze());
			}

			FSet* b_new = new FSet(set_new, true);
			FSet* b_curr = nullptr;
			_t->buckets[_i].compare_exchange_strong(
				b_curr, 
				b_new, 
				memory_order_release);
		}

		return _t->buckets[_i];
	}
}