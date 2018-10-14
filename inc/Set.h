
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

	bool Insert(uint32_t _k);

	bool Remove(uint32_t _k);

	bool Contains(uint32_t _k);

	Set* Union(Set* _rhs);

private:

	struct Node {
		int32_t key;
		atomic<uintptr_t> next;

		static inline Node* GetNextNode(
			uintptr_t _addr);
		static inline bool GetMarked(
			uintptr_t _addr);
		static inline uintptr_t MakeAddr(
			Node* _next, 
			bool  _marked);
	};

	struct Window {
		Node* pred;
		Node* curr;

		Window() = delete;
		Window(Node* _pred, Node* _curr);
	};

	void Insert(Set* _subset);
	Window Find(Node* _head, int32_t _k);

	atomic<Node*> m_head;
};

#endif // __SET_H__