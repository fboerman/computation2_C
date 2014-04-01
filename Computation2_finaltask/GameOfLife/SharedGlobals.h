////////////////////////////////////////////////////////////
//
// File: SharedGlobals.h
// Author: Frank Boerman
// definition of shared globals for multiple files
///////////////////////////////////////////////////////////

#ifndef SHAREDGLOBAL_H
#define SHAREDGLOBAL_H
#include "GameOfLife.h"

extern cell*** GLOBAL_GRID; //2d array with format [x][y] of pointers to cell class
extern int gridwidth;
extern int gridheight;
extern bool colourmode; //boolean if fancy colours need to be displayed
extern int colourscheme; //which colourscheme is choosen
#endif