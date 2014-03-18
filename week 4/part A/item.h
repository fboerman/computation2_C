//Frank Boerman 0802910 ©2014
#ifndef _ITEM_H_
#define _ITEM_H_
#include <iostream>
using namespace std;

template<typename T> class Item
{
public:
    Item(T p);
    virtual ~Item();

    int id();

	virtual void print();
	void resetcounter();

protected:
    int _id;

private:
	static int _idcounter;
	T _payload;
};




template<typename T> Item<T>::Item(T p)
{
	_idcounter++;
	_id = _idcounter;
	_payload = p;
}

template<typename T> Item<T>::~Item()
{ }

template<typename T> int Item<T>::id()
{
	return _id;
}

template<typename T> int Item<T>::_idcounter = 0;

template<typename T> void Item<T>::print()
{
	cout << "item id " << _id << " '" << _payload << "'" << endl;
}

template<typename T> void Item<T>::resetcounter() //resets the id counter
{
	_idcounter = 0;
}

#endif // _ITEM_H_ 