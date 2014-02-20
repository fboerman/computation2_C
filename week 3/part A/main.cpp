//Frank Boerman 0802910 ©2014
#include <iostream>

#include "list.h"
#include "item.h"

using namespace std;

char get_command()
{
    char c;
    cout << "Type command: ";
    cin >> c;
    return c;
}

int get_int() //gets integer from commandline, returns NULL when invalid input is given
{
	int input;
	cin >> input;
	if (cin.fail() || !cin)
	{
		cout << "Invalid input given!" << endl;
		cin.clear();
		return NULL;
	}
	return input;
}

void main()
{
	List* my_list = new List();
	ListIterator it;
	int id;
    // Define your my_list here
    cout << "-------- Ring-style linked list base class by Frank Boerman. --------" << endl << endl;

    char command;
    while ((command = get_command()) != 'q') {
        switch (command) {
        case 'a': // append
			my_list->push_back(new Item());
            break;
        case 'i': // insert
			my_list->push_front(new Item());
            break;
        case 'd': // delete
			cout << "Please give the id of the item to delete (int): ";
			id = get_int();
			if (id == NULL)
			{
				continue;
			}
			my_list->remove(my_list->find(id));
            break;
        case 'f': // put first
			cout << "Please give the id of the item to put first (int): ";
			id = get_int();
			if (id == NULL)
			{
				continue;
			}
			my_list->put_first(my_list->find(id));
            break;
        case 'p': // print
			my_list->print();
            break;
        case 's': // sort
			my_list->sort();
			cout << "List sorted!" << endl;
            break;
        case 'x': // destroy list
			it = my_list->begin();
			do
			{
				delete it.get();
				it.next();
			} while (it != my_list->begin());
			my_list->clear();
            break;
        case 'q':
            break;
        default:
            printf("Command unknown!\n");
            break;
        }
    }
	my_list->clear();
    cout << "Bye bye!" << endl;
}
