/*
*File: A.cpp
*Student: FJL Boerman
*student id: 0802910
*/
#include <iostream>
using namespace std;

// enter your global variables here
// (1) an array of strings with the 12 months of the year
char* months[12] = {"january", "february", "march", "april", "may", "june", "july", "august", "september", "october", "november", "december"};
//months[0] = "januari";

// enter the functions reverse_string_array fourght and get_value here
void reverse_string_array(char* array[], int size)
{
	for (int i = 0; i < (size/2); i++)
	{
		char* temp = nullptr;
		temp = array[i];
		array[i] = array[size - i - 1];
		array[size - i - 1] = temp;
	}
}

double fourth(int x)
{
	return x*x*x*x;
}

int get_value(int* num)
{
	return *num;
}

void main(void)
{
	// part A
	int i = 5;
	int* pi = &i;
	int** ppi = &pi;

	//(5) points to ber_string on "ber" in september of months
	char* ber_string = &months[8][6];

	// part B
	//reverse the order of the words in the string array months
	reverse_string_array(months, 12);

	//printf("%s, %s, %s and %s are the months \nof the year that end with '%s'.\n\n",
	//	months[3], months[2], months[1], months[0],
	//	ber_string);
	cout << months[3] << "," << months[2] << "," << months[1] << "," << months[0] << 
		" are the months " << endl << "of the year that and with '" << ber_string << "'" << endl;


	//printf("fourth(%d) = %.0f\n", **ppi, fourth(get_value(*ppi)));
	cout << "fourth(" << **ppi << " = " << fourth(get_value(*ppi)) << ")" << endl;
}