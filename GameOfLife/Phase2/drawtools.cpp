////////////////////////////////////////////////////////////
//
// File: drawtools.cpp
// Author: Frank Boerman
///////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string>
#include <iostream>
using namespace std;

#include <GL/glut.h>
#include "drawtools.h"

//help functions

//copies the array of a1 into a2
void copy_array(const float* a1, float* a2)
{
	int size = sizeof *a1;

	for (int i=0; i < size; i++)
	{
		*(a2 + i) = *(a1 + i);
	}
}

// class definitions:

     ///////////////////////////////////////////////////////
    //
   //  pixel class.
  //
 ///////////////////////////////////////////////////////
pixel::pixel(dlist* list, const float p[2], const float color[3])
	: item(list)    // call the ctor of item and chain into list
{
	assert(p);   // if you crash here then you called this with no coordinates
	copy_array(p, _p);

	if(!color) {      // When a null pointer is passed to this function
		_color[0] = 0;// black is used
		_color[1] = 0;
		_color[2] = 0;
	}
	else {
		copy_array(color, _color);
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
	copy_array(p1, _p1);
	copy_array(p2, _p2);


	if (!color) {      // When a null pointer is passed to this function
		_color[0] = 0;// black is used
		_color[1] = 0;
		_color[2] = 0;
	}
	else { //else copy it over
		copy_array(color, _color);
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
		copy_array(color, _color);
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

///////////////////////////////////////////////////////
//
//  square class.
//
///////////////////////////////////////////////////////

square::square(dlist* list, const float p1[2], const float p2[2], const float linecolor[3], const float fillcolor[3])
	:item(list)
{
	assert(p1);
	assert(p2);
	
	copy_array(p1, _p1);
	copy_array(p2, _p2);

	if (!linecolor)
	{
		_linecolor[0] = 0;
		_linecolor[1] = 0;
		_linecolor[2] = 0;
	}
	else
	{
		copy_array(linecolor, _linecolor);
	}

	if (!linecolor)
	{
		_linecolor[0] = 0;
		_linecolor[1] = 0;
		_linecolor[2] = 0;
	}
	else
	{
		copy_array(linecolor, _linecolor);
	}

	if (!fillcolor)
	{
		_fillcolor[0] = 0;
		_fillcolor[1] = 0;
		_fillcolor[2] = 0;
	}
	else
	{
		copy_array(fillcolor, _fillcolor);
	}

	_filled = 0;

}

square::~square()
{

}

void square::print() const
{
}


void square::draw()
{
	float p3[2], p4[2];
	//p1 is topleft, p2 is bottomright, p3 is topright, p4 is bottomleft
	//////////
	//p1	p3
	//p4	p2
	//////////
	p4[0] = _p1[0];
	p4[1] = _p2[1];
	p3[0] = _p2[0];
	p3[1] = _p1[1];

	glLineWidth(1);

	if (_filled)
	{
		glBegin(GL_QUADS);
			glColor3fv(_fillcolor);
			glVertex2fv(_p1);
			glVertex2fv(p3);
			glVertex2fv(_p2);
			glVertex2fv(p4);
		glEnd();
	}
	else
	{
		glBegin(GL_LINES);
			glColor3fv(_linecolor);
			glVertex2fv(_p1);
			glVertex2fv(p3);
			glVertex2fv(p3);
			glVertex2fv(_p2);
			glVertex2fv(_p2);
			glVertex2fv(p4);
			glVertex2fv(p4);
			glVertex2fv(_p1);
		glEnd();
	}
}

void square::flip()
{
	_filled = !_filled;
}

int square::get_status()
{
	return _filled;
}