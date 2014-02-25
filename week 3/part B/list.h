//Frank Boerman 0802910 ©2014
#ifndef _LIST_H_
#define _LIST_H_

class Item; // Forward declaration

// A simple data structure that forms the core of List
struct Node
{
    Node* next;
    Node* prev;
    Item* item;
};

// A helper class used to efficiently access individual elements of List
// Its advantage is that it does not expose the Node
class ListIterator
{
    friend class List; // Give List full access to ListIterator
                       // Only use when classes are very tightly related

public:
    ListIterator();

    // Returns the item of the node that this iterator refers to
    Item* get();

    // Proceed to the next node
    void next();

    // Operator overloading! These allow comparing.
    bool operator==(ListIterator other);
    bool operator!=(ListIterator other);

private:
    ListIterator(Node* node);

    Node* _node;
};

// This is what it's all about
// A double-linked circular list of nodes containing pointers to items.
class List
{
public:
    List();
    ~List();

    ListIterator begin();
    ListIterator end();
	ListIterator find(int id);

    void clear();
	void print();
	void push_back(Item*);
	void push_front(Item*);
	void remove(ListIterator it);
	void put_first(ListIterator it);
	void sort();

private:
    Node* _head;
	int highest_id();
};

#endif // _LIST_H_ 
