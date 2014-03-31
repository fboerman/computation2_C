////////////////////////////////////////////////////////////
//
// File: SharedGlobals.h
// Author: Frank Boerman
// definition of shared globals for multiple files
///////////////////////////////////////////////////////////

#ifndef SHAREDGLOBAL_H
#define SHAREDGLOBAL_H
#include "GameOfLife.h"

extern cell*** GLOBAL_GRID;
extern int gridwidth;
extern int gridheight;
extern bool colourmode;
extern int colourscheme;
#endif