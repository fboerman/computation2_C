////////////////////////////////////////////////////////////
//
// File: GameOflife.h
// Author: Frank Boerman
// All functions specific for the game of life such as ai etc
///////////////////////////////////////////////////////////

#ifndef GAMEOFLIFE_H // this avoids multiple inclusion of this file
#define GAMEOFLIFE_H

//dimension of the cell on screen
//#define CELL_DIMENSION 30

#include "dlist.h"
#include "drawtools.h"


//global
const float LINECOLOR[3] = { 0.0, 0.5, 0.0 };
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
	cell(square* sqr,const int status);
	~cell(void);

	//flips the buffer
	void flip(void);
	//flips the alive status
	void flip_flush(void);
	//increments the age
	void age(void);
	//flushes the buffer to drawlist
	void flush_buffer(void);
	//checks all the neighbors and updates the whitelines of a square
	void CheckNeighbors(int x, int y);
	//sets a neighbor
	void SetNeighbor(char n);
	//updates the filling colour
	void Calc_Color(void);

	int get_age(void);
	int get_status(void);

private:
	int _age;
	square* _square; //corresponding square on screen
	int _bufferstatus;
};

//help functions


//calculates ammount of cells possible on window with given windowsize and cell dimension
int* calc_grid_size(const int* windowsize, const int cell_dimension);

//calculates the window size with given grid size and cell dimension
int* calc_window_size(const int* gridsize, const int cell_dimension);

//fills give grid with empty cells (dead and with 0 age) and adds it to drawlist
void fill_grid(dlist* drawlist, const int xsize, const int ysize, const int cell_dimension);

//checks the whole grid and advances it one generation
void Tick(void);

//calculates the wraparound coordinate
int calc_wraparound(int index, int maxsize);

//forces a recalculation for all the colours of the grid
void ReCalc_Colours(void);

#endif