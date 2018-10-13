
#pragma once
#ifndef __SET_H__
#define __SET_H__

#include <atomic>
#include <stdint.h>

using namespace std;

class Set
{
public:

	Set();

	bool Insert(uint32_t);
	bool Remove(uint32_t);
	bool Contains(uint32_t);

private:

	struct Node {
		int32_t value;
		Node* next;
	};

	struct Window {
		Node* pred;
		Node* curr;

		Window(Node* _pred, Node* _curr);
	};

	Window Find(Node* head, int32_t _k);

	atomic<Node*> m_head;
};

#endif // __SET_H__