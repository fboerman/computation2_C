////////////////////////////////////////////////////////////
//
// File: GameOflife.cpp
// Author: Frank Boerman
// All functions specific for the game of life such as ai etc
///////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string>
#include <iostream>
#include <math.h>
using namespace std;

#include <GL/glut.h>
#include "GameOfLife.h"
#include "dlist.h"
#include "drawtools.h"

cell::cell(int age, int alive)
{
	_age = age;
	_alive = alive;
}

cell::~cell()
{

}

void cell::flip()
{
	_alive = !_alive;
}

int cell::get_age()
{
	return _age;
}

void cell::age()
{
	_age++;
}

//help functions

int* calc_grid_size(int* windowsize, int cell_dimension)
{
	int gridsize[2];

	//for x axis
	gridsize[0] = floor(windowsize[0] / cell_dimension);
	//for y axis
	gridsize[1] = floor(windowsize[1] / cell_dimension);

	return gridsize;
}

int* calc_window_size(int* gridsize, int cell_dimension)
{
	int windowsize[2];

	//x axis
	windowsize[0] = gridsize[0] * cell_dimension;
	//y axis
	windowsize[1] = gridsize[1] * cell_dimension;

	return windowsize;
}


void fill_grid(cell**** p_DA_GRID, dlist* drawlist, const int xsize, const int ysize)
{
	float clr[3] = { 0, 0, 0.5 };
	cell*** DA_GRID = *p_DA_GRID;
	//iterate trough the grid, grid has format [x][y]
	for (int y = 0; y < ysize; y++)
	{
		float p1[2], p2[2];//p1 is topleft, p2 is bottomright
		//calculate y
		p1[1] = y * CELL_DIMENSION;
		p2[1] = (y + 1) * CELL_DIMENSION;
		for (int x = 0; x < xsize; x++)
		{
			//calculate x
			p1[0] = x * CELL_DIMENSION;
			p2[0] = (x + 1) * CELL_DIMENSION;
			//create the cell and coresponding square
			new square(drawlist, p1, p2, clr);
			DA_GRID[x][y] = new cell(0, 0);
		}
	}

}