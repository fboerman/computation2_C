////////////////////////////////////////////////////////////
//
// File: parser.cpp
// Author: Frank Boerman
// Parsers for files
///////////////////////////////////////////////////////////
//#pragma warning(disable : 4244) //converting double to int is needed when calculating gridsize, so disable that warning
//#pragma warning(disable : 4018)//same goes for signed/unsigned mismatch (due to if statements)
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include "drawtools.h"
#include "dlist.h"
#include "GameOfLife.h"
#include "List.h"
#include "Parser.h"
#include <limits>
#include <sstream>
#include "SharedGlobals.h"

using namespace std;

int test = numeric_limits<int>::max();


int* GOLParser(std::string filename, int maxwidht, int maxheight, dlist* DrawList, int* cell_dimension)
{
	int gridsize[2];
	ifstream file;
	string fline;
	file.open(filename);
	if (file.fail())
	{
		cout << "Error reading file " << filename << endl;
		return nullptr;
	}
	
	List<string>* parsedfile = new List<string>();
	int leftline = numeric_limits<int>::max(); //holds the position of most left live cell
	while (getline(file, fline)) //fetch line per line
	{
		if (fline.find("O") != string::npos)
		{
			stringstream parsedline;
			for (int i = 0; i < (int)fline.size(); i++)//iterate through the line
			{
				if (fline[i] == 'O')
				{
					if (i < leftline)
					{
						leftline = i;
					}
				}
				parsedline << fline[i];
				//if (fline[i] == 'O' || parsedline.tellp() > 0) 
				//{
				//	parsedline << fline[i];
				//	//line_list->append(fline[i]); 
				//}
			}
			if (parsedline.tellp() > 0)
			{
				//parsedfile->append(*line_list);
				parsedfile->append(parsedline.str());
			}
		}
	}
	//find the largest line and also correct the line to drop all not needed dead cells after last living one, and all not needed dead cells before the most left living cell in the figure
	int maxsizex = 0;
	for (int i = 0; i < parsedfile->len(); i++)
	{
		string line = parsedfile->Get_String(i);
		string line_corrected = line.substr(0, line.find_last_of('O')+1).substr(leftline,string::npos);
		parsedfile->Set(i, line_corrected);
		int line_length = line_corrected.size();
		if (line_length > maxsizex)
		{
			maxsizex = line_length;
		}
	}

	//calculate minimal height and width
	int minheight = (2 * WHITEBAND + parsedfile->len())* *cell_dimension;
	int minwidth = (2 * WHITEBAND + maxsizex)* *cell_dimension;
	//check against the constraints
	if (minheight > maxheight)
	{
		*cell_dimension = (int)floor(maxheight/parsedfile->len());
		if (minwidth > maxwidht)
		{
			if (floor(maxwidht / maxsizex) < *cell_dimension)
			{
				*cell_dimension = (int)floor(maxwidht / maxsizex);
			}
			gridsize[0] = *cell_dimension * (2 * WHITEBAND + maxsizex);
		}
		else
		{
			gridsize[0] = maxwidht;
		}
		gridsize[1] = *cell_dimension * (2 * WHITEBAND + parsedfile->len());
	}
	else if (minwidth > maxwidht)
	{
		*cell_dimension = (int)floor(maxwidht / maxsizex);
		gridsize[0] = *cell_dimension * (2 * WHITEBAND + maxsizex);
		gridsize[1] = maxheight;
	}
	else
	{
		gridsize[0] = maxwidht;
		gridsize[1] = maxheight;
	}
	//create the width row
	int gridheight = gridsize[1] / *cell_dimension;
	int gridwidth = (gridsize[0] / *cell_dimension);
	GLOBAL_GRID = new cell**[gridsize[0] / *cell_dimension];
	float p1[2], p2[2];
	int gridy = gridsize[1] / *cell_dimension;
	int gridx = gridsize[0] / *cell_dimension;
	for (int x = 0; x < gridx;x++)
	{

		p1[0] = x * (int)*cell_dimension;
		p2[0] = (x + 1) * (int)*cell_dimension;
		GLOBAL_GRID[x] = new cell*[gridsize[1] / *cell_dimension];//create the height row
		
		for (int y = 0; y < gridy; y++)
		{
			p1[1] = y * (int)*cell_dimension;
			p2[1] = (y + 1) * (int)*cell_dimension;
			//create the new square
			square* sqr = new square(DrawList, p1, p2, LINECOLOR, CELLCOLOR);
			//check if were outside the sidebands
			if (x >= WHITEBAND && y >= WHITEBAND && (y-WHITEBAND) < parsedfile->len() && x < maxsizex){
				//check if x is in reange of the parsed line
				string line = parsedfile->Get_String((parsedfile->len() - 1) - (y - WHITEBAND));
				if ((x - WHITEBAND) >= (int)line.size())
				{
					GLOBAL_GRID[x][y] = new cell(0, sqr, 0);
				}
				else
				{
					//create the new cell
					//change the y because grid is drawn bottom up, while file is parsed top down
					//cout << "DEBUG: x: " << x << " y:" << y << endl;
					if (line[x - WHITEBAND] == 'O')
					{
						GLOBAL_GRID[x][y] = new cell(0, sqr, 1);
					}
					else
					{
						GLOBAL_GRID[x][y] = new cell(0, sqr, 0);
					}
				}
			}
			else
			{
				//if not than just insert empty cell
				GLOBAL_GRID[x][y] = new cell(0, sqr, 0);
			}
		}
	}
	cout << "dimension calculated: " << *cell_dimension << " gridwidth: " << gridwidth << " gridheight: " << gridheight << endl;
	//return the size
	return gridsize;
}


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