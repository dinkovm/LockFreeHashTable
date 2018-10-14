
#include "Set.h"

Set::Node* Set::Node::GetNextNode(
	uintptr_t _addr)
{
	return reinterpret_cast<Node*>(
		_addr & static_cast<uintptr_t>(UINTPTR_MAX ^ 0x1));
}

bool Set::Node::GetMarked(
	uintptr_t _addr)
{
	return _addr & static_cast<uintptr_t>(1u);
}

uintptr_t Set::Node::MakeAddr(
	Set::Node* _next, 
	bool _marked)
{
	return (reinterpret_cast<uintptr_t>(_next) &
		    (UINTPTR_MAX ^ 1)) | 
		   _marked;
}

Set::Window::Window(Node* _pred, Node* _curr)
{
	pred = _pred;
	curr = _curr;
}

Set::Set()
{
}

bool Set::Insert(uint32_t _k)
{
	while (true)
	{
		Window window = Find(m_head.load(memory_order_acquire), _k);

		if (window.curr->key == _k)
		{
			// Entry already present
			return false;
		}
		else
		{
			Node* node = new Node();
			node->next = Node::MakeAddr(window.curr, false);

			uintptr_t expected = Node::MakeAddr(window.curr, false);
			uintptr_t desired = Node::MakeAddr(node, false);

			if (window.pred->next.compare_exchange_weak(expected, desired))
			{
				return true;
			}
		}
	}
}

bool Set::Remove(uint32_t _k)
{
	while (true)
	{
		Window window = Find(m_head.load(memory_order_acquire), _k);

		if (window.curr->key != _k)
		{
			return false;
		}
		else
		{
			Node* succ = Node::GetNextNode(
				window.curr->next.load(memory_order_acquire));

			uintptr_t expLogical = Node::MakeAddr(succ, false);
			uintptr_t desLogical = Node::MakeAddr(succ, true);

			if (!window.curr->next.compare_exchange_weak(expLogical, desLogical))
			{
				continue;
			}

			uintptr_t expPhysical = Node::MakeAddr(window.curr, false);
			uintptr_t desPhysical = Node::MakeAddr(succ, false);

			window.pred->next.compare_exchange_weak(expPhysical, desPhysical);
			return true;
		}
	}
}

bool Set::Contains(uint32_t _k)
{
	bool marked = false;
	Node* curr = m_head.load(memory_order_acquire);

	while (curr->key < _k)
	{
		curr = Node::GetNextNode(
			curr->next.load(memory_order_acquire));
		marked = Node::GetMarked(
			curr->next.load(memory_order_acquire));
	}
	
	return (curr->key == _k && !marked);
}

Set::Window Set::Find(Node* _head, int32_t _k)
{
	Node* pred   = nullptr;
	Node* curr   = nullptr;
	Node* succ   = nullptr;
	bool  marked = false;

retry:
	while (true)
	{
		pred = _head;
		curr = Node::GetNextNode(pred->next);

		while (true)
		{
			uintptr_t addr = curr->next.load(memory_order_acquire);
			succ           = Node::GetNextNode(addr);
			marked         = Node::GetMarked(addr);

			while (marked)
			{
				uintptr_t expected = Node::MakeAddr(curr, false);
				uintptr_t desired  = Node::MakeAddr(succ, false);
				
				if (!pred->next.compare_exchange_weak(expected, desired))
				{
					goto retry;
				}

				curr = succ;
				addr = curr->next.load(memory_order_acquire);
				succ = Node::GetNextNode(addr);
				marked = Node::GetMarked(addr);
			}

			if (curr->key >= _k)
			{
				return Window(pred, curr);
			}

			pred = curr;
			curr = succ;
		}
	}
	
}