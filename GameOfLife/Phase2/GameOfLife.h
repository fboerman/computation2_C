////////////////////////////////////////////////////////////
//
// File: GameOflife.h
// Author: Frank Boerman
// All functions specific for the game of life such as ai etc
///////////////////////////////////////////////////////////

#ifndef GAMEOFLIFE_H // this avoids multiple inclusion of this file
#define GAMEOFLIFE_H

//dimension of the cell on screen
#define CELL_DIMENSION 30

#include "dlist.h"

//forward declaration
class cell;

// class definitions:

	///////////////////////////////////////////////////////
   //
  // cell class.
 //
///////////////////////////////////////////////////////

class cell
{
public:
	cell(int age, int alive);
	~cell(void);

	//flips the alive status
	void flip(void);
	//increments the age
	void age(void);
	//returns age
	int get_age(void);

private:
	int _age;
	int _alive;
};

//help functions

//calculates ammount of cells possible on window with given windowsize and cell dimension
int* calc_grid_size(int* windowsize, int cell_dimension);

//calculates the window size with given grid size and cell dimension
int* calc_window_size(int* gridsize, int cell_dimension);

//fills give grid with empty cells (dead and with 0 age) and adds it to drawlist
void fill_grid(cell**** p_DA_GRID, dlist* drawlist, const int xsize, const int ysize);

#endif