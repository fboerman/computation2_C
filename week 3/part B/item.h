//Frank Boerman 0802910 �2014
#ifndef _ITEM_H_
#define _ITEM_H_

class Item
{
public:
    Item();
    virtual ~Item();

    int id();

	virtual void print();
	void resetcounter();

protected:
    int _id;
	static int _idcounter;

};

#endif // _ITEM_H_ 
