/*
*File: c.cpp
*Student: FJL Boerman
*student id: 0802910
*/

#include <iostream>
#include <string>
using namespace std;

void main(void)
{
	string input1;
	string input2;
	cout << "Enter the first word: ";
	cin >> input1;
	cout << "Enter the second word: ";
	cin >> input2;
	if (input1 == input2)
	{
		cout << input1 << " and " << input2 << " are equal." << endl;
	}
	else
	{
		cout << "Concatenated: " << input1 + input2 << endl;
	}

}