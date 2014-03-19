////////////////////////////////////////////////////////////
//
// File: drawtools.cpp
// Author: Sven Goossens
// Purpose:
//      Extending the dlist and item classes to provide a set of 'drawtools'
//      Contains an implementation of the pixel class, and skeleton for the
//      Line class
//
///////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string>
#include <iostream>
using namespace std;

#include <GL/glut.h>
#include "drawtools.h"

// class definitions:

     ///////////////////////////////////////////////////////
    //
   //  pixel class.
  //
 ///////////////////////////////////////////////////////
/*
      Please note that the pixel class is an incredibly inefficient way to draw
      1(!!) pixel to the screen. This class is mearly ment as an example of
      how to use the DrawList. 
*/
pixel::pixel(dlist* list, const float p[2], const float color[3])
	: item(list)    // call the ctor of item and chain into list
{
	assert(p);   // if you crash here then you called this with no coordinates
	_p[0] = p[0];// Store location of the pixel in the member variable _p
	_p[1] = p[1];

	if(!color) {      // When a null pointer is passed to this function
		_color[0] = 0;// black is used
		_color[1] = 0;
		_color[2] = 0;
	}
	else {
		_color[0] = color[0];
		_color[1] = color[1];
		_color[2] = color[2];
	}
}
//---------------------------------------------------------------------------
//
// draw myself on the screen
//
void pixel::draw(void)
{
	glBegin(GL_POINTS);            // Start drawing mode

	// If a color was passed to the function, set it
	glColor3f(_color[0],_color[1], _color[2]);
	glVertex2fv(this->_p);        // Draw the pixel at (_p[0], _p[1])

	glEnd();
}
//---------------------------------------------------------------------------
//
// Print myself in ELIF format
//
void pixel::print(void) const
{
	printf(".pixel %.0f %.0f %.5f %.5f %.5f\n", _p[0], _p[1], _color[0],
															_color[1], _color[2]);
}
//---------------------------------------------------------------------------
//
// destructor of a pixel
//
pixel::~pixel(void)
{

}
//---------------------------------------------------------------------------
     ///////////////////////////////////////////////////////
    //
   //  line class.
  //
 ///////////////////////////////////////////////////////
line::line(dlist* list, const float p1[2], const float p2[2],
                        const float color[3], const float lineWidth)
	: item(list)    // call the ctor of item and chain into list
{
	//check coordinates
	assert(p1);
	assert(p2);
	//copy it into the private members
	_p1[0] = p1[0];
	_p1[1] = p1[1];
	_p1[2] = p1[2];
	_p2[0] = p2[0];
	_p2[1] = p2[1];
	_p2[2] = p2[2];

	if (!color) {      // When a null pointer is passed to this function
		_color[0] = 0;// black is used
		_color[1] = 0;
		_color[2] = 0;
	}
	else { //else copy it over
		_color[0] = color[0];
		_color[1] = color[1];
		_color[2] = color[2];
	}

	_lineWidth = lineWidth;
}
//---------------------------------------------------------------------------
//
// destructor of a line
//
line::~line(void)
{

}
//---------------------------------------------------------------------------
//
// Print myself in EDIF++ format
//
void line::print(void) const
{
	cout << ".line " << _p1[0] << " " << _p1[1] << " " << _p2[0] << " " << _p2[1] << " " << _color[0] << " " << _color[1] << " " << _color[2] << " " << _lineWidth << endl;
}
//---------------------------------------------------------------------------
//
// draw myself on the screen
//
void line::draw(void)
{
	glLineWidth(_lineWidth);
	glBegin(GL_LINES);
	glColor3f(_color[0], _color[1], _color[2]);
	glVertex2fv(_p1);
	glVertex2fv(_p2);
	glEnd();
}

     ///////////////////////////////////////////////////////
    //
   //  text class.
  //
 ///////////////////////////////////////////////////////

text::text(dlist* list, const string str, const float color[3], int x, int y)
	:item(list)
{	
	if (str == "")
	{
		assert(0);
	}

	_str = str;

	_x = x;
	_y = y;

	if (!color) {      // When a null pointer is passed to this function
		_color[0] = 0;// black is used
		_color[1] = 0;
		_color[2] = 0;
	}
	else { //else copy it over
		_color[0] = color[0];
		_color[1] = color[1];
		_color[2] = color[2];
	}
}

text::~text()
{

}

void text::print() const
{
	cout << ".text " << _x << " " << _y << " " << _color[0] << " " << _color[1] << " " << _color[2] << " \"" << _str << "\"" << endl;
}

void text::draw()
{
	glColor3f(_color[0], _color[1], _color[2]);
	glRasterPos2f(_x, _y);
	for (char& c : _str)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
	}
}