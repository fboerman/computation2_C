////////////////////////////////////////////////////////////
//
// File: parser.cpp
// Author: Frank Boerman
// Parsers for files
///////////////////////////////////////////////////////////

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include "drawtools.h"
#include "dlist.h"

using namespace std;

string findstring(string line)
{
	int first = line.find_first_of("\"");
	int last = line.find_last_of("\"");
	return line.substr(first, last - first);
}

void readEDIF(string fname, string* windowname, dlist* DrawList)
{
	ifstream file;
	string fline;
	file.open(fname);
	if (file.fail())
	{
		cout << "Error reading file " << fname << endl;
		return;
	}
	cout << "Parsing file " << fname << endl;
	while (getline(file, fline))
	{
		stringstream sstream(fline);
		string command, title, ftext;
		float p1[2], p2[2], clr[3], width;
		pixel* pxl;
		line* ln;
		text* txt;

		if (fline[0] != '.' || fline == "")
		{
			//comment line
			continue;
		}

		switch (fline[1])
		{
		case 'd': //change window title
			*windowname = findstring(sstream.str());
			cout << "Parsed windowname: " << windowname << endl;
			break;
		case 'p': //draw pixel
			sstream >> command >> p1[0] >> p1[1] >> clr[0] >> clr[1] >> clr[2];
			pxl = new pixel(DrawList, p1, clr);
			cout << "Parsed a pixel: ";
			pxl->print();
			break;
		case 't': //draw text
			sstream >> command >> p1[0] >> p1[1] >> clr[0] >> clr[1] >> clr[2];
			ftext = findstring(sstream.str());
			txt = new text(DrawList, ftext, clr, p1[0], p1[1]);
			cout << "Parsed a string: ";
			txt->print();
			break;
		case 'l': //draw line
			sstream >> command >> p1[0] >> p1[1] >> p2[0] >> p2[1] >> clr[0] >> clr[1] >> clr[2] >> width;
			ln = new line(DrawList, p1, p2, clr, width);
			cout << "Parsed a line: ";
			ln->print();
			break;
		}
	}
}