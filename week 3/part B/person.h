//Frank Boerman 0802910 ©2014
#ifndef _PERSON_H_
#define _PERSON_H_

#include "item.h"
#include <iostream>
class Person : public Item
{

public:
	Person(std::string name);
	virtual ~Person();
	virtual void print();

private:
	std::string _name;
};

#endif // _PERSON_H_ 