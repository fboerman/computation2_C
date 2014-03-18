//Frank Boerman 0802910 ©2014
#ifndef _LIST_H_
#define _LIST_H_

#include <iostream>
#include "item.h"
#include <time.h>

using namespace std;

template<typename T> class Item; // Forward declaration

// A simple data structure that forms the core of List
template<typename T> struct Node
{
    Node* next;
    Node* prev;
	Item<T>* item;
};

// A helper class used to efficiently access individual elements of List
// Its advantage is that it does not expose the Node
template<typename T> class ListIterator
{
    template<typename T> friend class List; // Give List full access to ListIterator
                       // Only use when classes are very tightly related

public:
    ListIterator();

    // Returns the item of the node that this iterator refers to
    Item<T>* get();

    // Proceed to the next node
    void next();

    // Operator overloading! These allow comparing.
    bool operator==(ListIterator other);
    bool operator!=(ListIterator other);

private:
    ListIterator(Node<T>* node);

    Node<T>* _node;
};

// This is what it's all about
// A double-linked circular list of nodes containing pointers to items.
template<typename T> class List
{
public:
    List();
    ~List();

    ListIterator<T> begin();
    ListIterator<T> end();
	ListIterator<T> find(int id);

    void clear();
	void print();
	void push_back(Item<T>*);
	void push_front(Item<T>*);
	void remove(ListIterator<T> it);
	void put_first(ListIterator<T> it);
	void sort();

private:
    Node<T>* _head;
	int highest_id();
};



//Frank Boerman 0802910 ©2014

////////////////////////////
// ListIterator functions //
////////////////////////////
// These should not have to be altered

template<typename T> ListIterator<T>::ListIterator()
: _node(nullptr)
{ }

template<typename T> ListIterator<T>::ListIterator(Node<T>* node)
: _node(node)
{ }

template<typename T> Item<T>* ListIterator<T>::get()
{
	return _node ? _node->item : nullptr;
}

template<typename T> void ListIterator<T>::next()
{
	_node = _node->next;
}

template<typename T> bool ListIterator<T>::operator== (ListIterator other)
{
	return _node == other._node;
}

template<typename T> bool ListIterator<T>::operator!= (ListIterator other)
{
	return _node != other._node;
}

template<typename T> ListIterator<T> List<T>::find(int id) //searches for a node with given id
{
	if (_head == nullptr)
	{
		return ListIterator<T>(nullptr);
	}

	Node<T>* node = _head;
	do
	{
		if (node->item->id() == id)
		{
			return ListIterator<T>(node);
		}
	} while ((node = node->next) != _head);

	return ListIterator<T>(nullptr);
}

////////////////////
// List functions //
////////////////////

template<typename T> List<T>::List()
: _head(nullptr)
{ }

template<typename T> List<T>::~List()
{
	clear();
}


template<typename T> ListIterator<T> List<T>::begin()
{
	return ListIterator<T>(_head);
}

template<typename T> ListIterator<T> List<T>::end()
{
	return ListIterator<T>(_head ? _head->prev : nullptr);
}

template<typename T> void List<T>::clear() //clears the list
{
	if (!_head) // Nothing to clear
		return;

	Node<T>* next_node = _head;
	do {
		Node<T>* current_node = next_node;
		next_node = current_node->next;
		delete current_node;
	} while (next_node != _head);
	_head = nullptr;
}

template<typename T> void List<T>::print() //prints the list
{
	if (_head == nullptr)
	{
		cout << "List contains no entries." << endl;
		return;
	}

	int ordercounter = 0;

	Node<T>* node = _head;
	do
	{
		ordercounter++;
		cout << "order " << ordercounter << " ";
		node->item->print();
		//cout << "previous: " << node->prev->item->id() << endl;
	} while ((node = node->next) != _head);
}

template<typename T> void List<T>::push_back(Item<T>* newitem) //appends new item to end of the list
{
	Node<T>* newnode = new Node<string>();

	newnode->item = newitem;
	if (_head == nullptr)
	{
		newnode->next = newnode;
		newnode->prev = newnode;
		_head = newnode;
	}
	else
	{
		Node<T>* lastnode = _head->prev;
		lastnode->next = newnode;
		newnode->prev = lastnode;
		newnode->next = _head;
		_head->prev = newnode;
	}
}

template<typename T> void List<T>::push_front(Item<T>* newitem) //inserts new item to beginning of the list
{
	Node<T>* newnode = new Node<string>();

	newnode->item = newitem;
	if (_head == nullptr)
	{
		newnode->prev = newnode;
		newnode->next = newnode;
		_head = newnode;
	}
	else
	{
		Node<T>* lastnode = _head->prev;
		lastnode->next = newnode;
		_head->prev = newnode;
		newnode->next = _head;
		newnode->prev = lastnode;
		_head = newnode;
	}
}

template<typename T> void List<T>::remove(ListIterator<T> it) //removes the given iterator from the list
{
	if (it._node == nullptr)
	{
		cout << "Could not find that id in the tree." << endl;
		return;
	}

	Node<T>* before = it._node->prev;
	Node<T>* after = it._node->next;

	if (after == before)
	{
		_head = nullptr;
		return;
	}

	if (_head == it._node)
	{
		_head = after;
	}

	before->next = after;
	after->prev = before;

	delete it.get();
	delete it._node;

}

template<typename T> void List<T>::put_first(ListIterator<T> it) //puts an given iterator at the head of the list
{
	if (it._node == nullptr)
	{
		cout << "Could not find that id in the tree." << endl;
		return;
	}

	Node<T>* before = it._node->prev;
	Node<T>* after = it._node->next;

	if (it._node == _head)
	{
		return;
	}

	if (before == after)
	{
		_head->prev = it._node;
		_head->next = it._node;
		_head = it._node;
		return;
	}

	before->next = after;
	after->prev = before;

	_head->prev->next = it._node;
	it._node->prev = _head->prev;
	_head->prev = it._node;
	it._node->next = _head;

	_head = it._node;

	cout << "Item with id " << it._node->item->id() << " put in front" << endl;
}

template<typename T> int List<T>::highest_id() //finds the highest id number and returns it, 0 for empty list
{
	if (_head == nullptr) //if list is empty return zero
	{
		return 0;
	}

	int highestid = 0;
	Node<T>* node = _head;
	do
	{
		if (node->item->id() > highestid)
		{
			highestid = node->item->id();
		}
	} while ((node = node->next) != _head);
	return highestid;
}

template<typename T> void List<T>::sort() //sorts the list from low to high id
{
	clock_t tstart = clock();
	int highestid = highest_id();

	for (int i = highestid; i > 0; i--)
	{
		ListIterator<T> it = find(i);

		if (it._node != nullptr)
		{
			put_first(it);
		}

	}

	clock_t tend = clock();

	//cout << "sorted in " << tend - tstart << endl;
}

#endif // _LIST_H_ 