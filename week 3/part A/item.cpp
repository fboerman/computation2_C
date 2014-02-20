//Frank Boerman 0802910 ©2014
#include <iostream>
#include "item.h"

using namespace std;

Item::Item()
{ 
	_idcounter++;
	_id = _idcounter;
}

Item::~Item()
{ }

int Item::id()
{
    return _id;
}

int Item::_idcounter = 0;

void Item::print()
{
	cout << "item id " << _id << endl;
}

void Item::resetcounter() //resets the id counter
{
	_idcounter = 0;
}