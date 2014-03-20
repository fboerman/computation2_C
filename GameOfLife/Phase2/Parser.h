////////////////////////////////////////////////////////////
//
// File: parser.h
// Author: Frank Boerman
// Parsers for files
///////////////////////////////////////////////////////////
#ifndef PARSER_H // this avoids multiple inclusion of this file
#define PARSER_H

#include <string>
#include "dlist.h"

void readEDIF(std::string fname);
std::string findstring(std::string line, std::string* windowname, dlist* DrawList);

#endif