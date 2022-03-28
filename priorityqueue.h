#pragma once

#include <algorithm>
#include <cstdlib>

template <typename T>
class PriorityQueue {
private:
	const static size_t capacity = 100; // Max entries in the queue

	class Node { // An entry in the queue
	public:
		T value; // The value stored in the queue
		size_t priority; // The queue entry's priority
		size_t sequence; // The queue entry's sequence number
		bool operator< (const Node& n) const {
			return priority < n.priority ||
				(priority == n.priority && sequence < n.sequence);
		}
	};

	Node entries[capacity]; // The queue entries

	size_t entry_count; // Number of entries in the queue
	size_t sequence_count; // The last sequence number used

	// For navitaging the binary tree
	size_t left (size_t i) const { return 2 * i + 1; }
	size_t right (size_t i) const { return 2 * i + 2; }
	size_t parent (size_t i) const { return (i - 1) / 2; }

	void push_up (size_t i);
	void push_down (size_t i);
public:

	PriorityQueue () { entry_count = sequence_count = 0; }

	bool empty () const { return !entry_count; }
	size_t size () const { return entry_count; }

	T front () const { return entries[0].value; }
	void push (const T& val, size_t pri);

	T pop ();
};

template <typename T>
void PriorityQueue<T>::push (const T& val, size_t pri) {
	entries[entry_count].value = val;
	entries[entry_count].priority = pri;
	entries[entry_count].sequence = ++sequence_count;
	entry_count++;
	push_up(entry_count-1);
}

template <typename T>
void PriorityQueue<T>::push_up (size_t i) {
	if (i > 0) {
		size_t p = parent(i);
		if (entries[i] < entries[p]) {
			std::swap(entries[i], entries[p]);
			push_up(p);
		}
	}
}

template <typename T>
T PriorityQueue<T>::pop () {
	// Assumes that entry_count > 0
	T val = entries[0].value;

	entries[0] = entries[--entry_count];

	if (entry_count > 0)
		push_down(0);

	return val;
}

template <typename T>
void PriorityQueue<T>::push_down (size_t i) {
	size_t l = left(i); // Index of left child
	size_t r = right(i); // Index of right child
	size_t max = i; // Index i, l, or r, that belongs in the root of subtree i
	// Check left child
	if (l < entry_count && entries[l] < entries[max])
		max = l;
	// Check right child
	if (r < entry_count && entries[r] < entries[max])
		max = r;
	// Swap if necessary
	if (max != i) {
		std::swap(entries[i], entries[max]);
		push_down(max);
	}
}
