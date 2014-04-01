////////////////////////////////////////////////////////////
//
// File: parser.h
// Author: Frank Boerman
// Parsers for files
///////////////////////////////////////////////////////////
#ifndef PARSER_H // this avoids multiple inclusion of this file
#define PARSER_H

#define WHITEBAND 2 //how many rows the parsers leaves at the borders

#include <string>
#include "dlist.h"
#include "GameOfLife.h"

//parses gol file and puts it in the grid and returns widht and height of the grid in pixels, if it doesnt fit than it resizes the cell dimension
int* GOLParser( std::string filename, int maxwidht, int maxheight, dlist* Drawlist, int* cell_dimension);

//old parser for edif files
void readEDIF(std::string fname, std::string* windowname, dlist* DrawList);
//find a string within a string contained between double quotes
std::string findstring(std::string line, std::string* windowname, dlist* DrawList);

#endif