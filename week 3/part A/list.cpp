//Frank Boerman 0802910 ©2014

#include <iostream>

#include "list.h"
#include "item.h"
#include <time.h>

using namespace std;

////////////////////////////
// ListIterator functions //
////////////////////////////
// These should not have to be altered

ListIterator::ListIterator()
    : _node(nullptr)
{ }

ListIterator::ListIterator(Node* node)
    : _node(node)
{ }

Item* ListIterator::get()
{
    return _node ? _node->item : nullptr;
}

void ListIterator::next()
{
	_node = _node->next;
}

bool ListIterator::operator== (ListIterator other)
{
    return _node == other._node;
}

bool ListIterator::operator!= (ListIterator other)
{
    return _node != other._node;
}

ListIterator List::find(int id) //searches for a node with given id
{
	if (_head == nullptr)
	{
		return ListIterator(nullptr);
	}

	Node* node = _head;
	do
	{
		if (node->item->id() == id)
		{
			return ListIterator(node);
		}
	} while ((node = node->next) != _head);

	return ListIterator(nullptr);
}

////////////////////
// List functions //
////////////////////

List::List()
    : _head(nullptr)
{ }

List::~List()
{
    clear();
}


ListIterator List::begin()
{
    return ListIterator(_head);
}

ListIterator List::end()
{
    return ListIterator(_head ? _head->prev : nullptr);
}

void List::clear() //clears the list
{
    if (!_head) // Nothing to clear
        return;

    Node* next_node = _head;
    do {
        Node* current_node = next_node;
        next_node = current_node->next;
        delete current_node;
    } while (next_node != _head);
    _head = nullptr;
}

void List::print() //prints the list
{
	if (_head == nullptr)
	{
		cout << "List contains no entries." << endl;
		return;
	}

	int ordercounter = 0;

	Node* node = _head;
	do
	{
		ordercounter++;
		cout << "order " << ordercounter << " ";
		node->item->print();
		//cout << "previous: " << node->prev->item->id() << endl;
	} while ((node = node->next) != _head);
}

void List::push_back(Item* newitem) //appends new item to end of the list
{
	Node* newnode = new Node();

	newnode->item = newitem;
	if (_head == nullptr)
	{
		newnode->next = newnode;
		newnode->prev = newnode;
		_head = newnode;
	}
	else
	{
		Node* lastnode = _head->prev;
		lastnode->next = newnode;
		newnode->prev = lastnode;
		newnode->next = _head;
		_head->prev = newnode;
	}
}

void List::push_front(Item* newitem) //inserts new item to beginning of the list
{
	Node* newnode = new Node();

	newnode->item = newitem;
	if (_head == nullptr)
	{
		newnode->prev = newnode;
		newnode->next = newnode;
		_head = newnode;
	}
	else
	{
		Node* lastnode = _head->prev;
		lastnode->next = newnode;
		_head->prev = newnode;
		newnode->next = _head;
		newnode->prev = lastnode;
		_head = newnode;
	}
}

void List::remove(ListIterator it) //removes the given iterator from the list
{
	if (it._node == nullptr)
	{
		cout << "Could not find that id in the tree." << endl;
		return;
	}

	Node* before = it._node->prev;
	Node* after = it._node->next;

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

void List::put_first(ListIterator it) //puts an given iterator at the head of the list
{
	if (it._node == nullptr)
	{
		cout << "Could not find that id in the tree." << endl;
		return;
	}

	Node* before = it._node->prev;
	Node* after = it._node->next;

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

int List::highest_id() //finds the highest id number and returns it, 0 for empty list
{
	if (_head == nullptr) //if list is empty return zero
	{
		return 0;
	}

	int highestid = 0;
	Node* node = _head;
	do
	{
		if (node->item->id() > highestid)
		{
			highestid = node->item->id();
		}
	} while ((node = node->next) != _head);
	return highestid;
}

void List::sort() //sorts the list from low to high id
{
	clock_t tstart = clock();
	int highestid = highest_id();

	for (int i = highestid; i > 0; i--)
	{
		ListIterator it = find(i);

		if (it._node != nullptr)
		{
			put_first(it);
		}

	}

	clock_t tend = clock();

	//cout << "sorted in " << tend - tstart << endl;
}