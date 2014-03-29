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



cell::cell(const int age, square* sqr, const int status)
{
	assert(sqr);
	_age = age;
	_square = sqr;
	_bufferstatus = 0;
	if (status)
	{
		_square->flip();
		_bufferstatus = 1;
	}
}

cell::~cell()
{
	delete _square;
}

void cell::flip()
{
	_bufferstatus = !_bufferstatus;
}

void cell::flip_flush()
{
	_square->flip();
	_bufferstatus = _square->get_status();
}

int cell::get_age()
{
	return _age;
}

void cell::age()
{
	_age++;
}

int cell::get_status()
{
	return _square->get_status();
}

void cell::flush_buffer()
{
	if (this->get_status() != _bufferstatus)
	{
		this->flip_flush();
	}
}

//help functions

int* calc_grid_size(const int* windowsize, const int cell_dimension)
{
	int gridsize[2];

	//for x axis
	gridsize[0] = floor(windowsize[0] / cell_dimension);
	//for y axis
	gridsize[1] = floor(windowsize[1] / cell_dimension);

	return gridsize;
}

int* calc_window_size(const int* gridsize, const int cell_dimension)
{
	int windowsize[2];

	//x axis
	windowsize[0] = gridsize[0] * cell_dimension;
	//y axis
	windowsize[1] = gridsize[1] * cell_dimension;

	return windowsize;
}


void fill_grid(cell**** p_DA_GRID, dlist* drawlist, const int xsize, const int ysize, const int cell_dimension)
{
	float clr[3] = { 0, 0, 0.5 };
	cell*** DA_GRID = *p_DA_GRID;
	//iterate trough the grid, grid has format [x][y]
	for (int y = 0; y < ysize; y++)
	{
		float p1[2], p2[2];//p1 is topleft, p2 is bottomright
		//calculate y
		p1[1] = y * cell_dimension;
		p2[1] = (y + 1) * cell_dimension;
		for (int x = 0; x < xsize; x++)
		{
			//calculate x
			p1[0] = x * cell_dimension;
			p2[0] = (x + 1) * cell_dimension;
			//create the cell and coresponding square
			square* sqr = new square(drawlist, p1, p2, LINECOLOR, CELLCOLOR);
			DA_GRID[x][y] = new cell(0, sqr, 0);
		}
	}

}

void Tick(cell**** p_DA_GRID, const int gridwidth, const int gridheight)
{
	//calculate the new generation
	cell*** DA_GRID = *p_DA_GRID;
	for (int x = 0; x < gridwidth; x++)
	{
		for (int y = 0; y < gridheight; y++)
		{
			//use wraparound formula
			//count the neighbours
			int neighbors = DA_GRID[calc_wraparound(x - 1, gridwidth)][calc_wraparound(y + 1, gridheight)]->get_status() + DA_GRID[x][calc_wraparound(y + 1, gridheight)]->get_status() + DA_GRID[calc_wraparound(x + 1, gridwidth)][calc_wraparound(y + 1,gridheight)]->get_status() +
				DA_GRID[calc_wraparound(x - 1, gridwidth)][y]->get_status() + DA_GRID[calc_wraparound(x + 1,gridwidth)][y]->get_status() +
				DA_GRID[calc_wraparound(x - 1, gridwidth)][calc_wraparound(y - 1, gridheight)]->get_status() + 
				DA_GRID[x][calc_wraparound(y - 1, gridheight)]->get_status() + 
				DA_GRID[calc_wraparound(x + 1, gridwidth)][calc_wraparound(y - 1,gridheight)]->get_status();

			//any live cell with fewer than two live neighbours dies, as if by loneliness
			if ((neighbors < 2) && DA_GRID[x][y]->get_status())
			{
				DA_GRID[x][y]->flip();
				continue;
			}
			//Any live cell with more than three live neighbours dies, as if by overcrowding
			if ((neighbors > 3) && DA_GRID[x][y]->get_status())
			{
				DA_GRID[x][y]->flip();
				continue;
			}

			//any dead cell with exactly three live neighbours comes to life
			if ((neighbors == 3) && !DA_GRID[x][y]->get_status())
			{
				DA_GRID[x][y]->flip();
				continue;
			}
			//Any live cell with two or three live neighbours live, unchanged, to the next generation
			//if all others are false than go to next cell

		}
	}
	//flush the buffers
	for (int x = 0; x < gridwidth; x++)
	{
		for (int y = 0; y < gridheight; y++)
		{
			DA_GRID[x][y]->flush_buffer();
		}
	}
}

int calc_wraparound(int index, int maxsize)
{
	return (index + maxsize) % maxsize;
}