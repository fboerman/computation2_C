////////////////////////////////////////////////////////////
//
// File: drawtools.cpp
// Author: Frank Boerman
///////////////////////////////////////////////////////////

#ifndef DRAWTOOLS_H // this avoids multiple inclusion of this file
#define DRAWTOOLS_H

#include "dlist.h"  // to include the dlist class

class line;          // forward: the classes in this file
class pixel;
class text;
class square;

//help functions

//copies the array of a1 into a2
void copy_array(const float* a1, float* a2);

// class declarations


     ///////////////////////////////////////////////////////
    //
   //  pixel class.
  //
 ///////////////////////////////////////////////////////

class pixel :
             public item    // inherits from item because its part of a DrawList
{
public:
	// constructors. Specify the list of the item, postion and color
	pixel(dlist* list, const float p[2], const float color[3]);
	~pixel(void);

	void print() const;   // print myself in EDIF++ format
	void draw(void);// draw myself to the screen

	// you can add more public members here, if needed

private:
	float _p[2];    // Coordinates
	float _color[3];
};

     ///////////////////////////////////////////////////////
    //
   //  line class.
  //
 ///////////////////////////////////////////////////////

class line :
             public item    // inherits from item because its part of a DrawList
{
public:
	// constructors. Specify the list of the line, postion, color and width
	line(dlist* list, const float p1[2], const float p2[2],
									const float color[3], const float lineWidth);
	~line(void);

	void print() const;   // print myself in EDIF++ format
	void draw(void);// draw myself to the screen

	// you can add more public members here, if needed

private:
	float _p1[2];
	float _p2[2];
	float _color[3];
	float _lineWidth;
};

     ///////////////////////////////////////////////////////
    //
   //  text class.
  //
 ///////////////////////////////////////////////////////

class text :
			public item    // inherits from item because its part of a DrawList
{
public:
	text(dlist* list, const string str, const float color[3], int x, int y);
	~text(void);

	void print() const;   // print myself in EDIF++ format
	void draw(void);// draw myself to the screen

	// you can add more public members here, if needed

private:
	string _str;
	float _color[3];
	int _x;
	int _y;
	// you can add more private members here, if needed

};

///////////////////////////////////////////////////////
//
//  square class.
//
///////////////////////////////////////////////////////

class square :
	public item
{
public:
	square(dlist* list, const float p1[2], const float p2[2], const float color[3]);
	~square(void);

	void print() const;
	void draw(void);
	void flip(void);

private:
	float _p1[2];//actual drawing position
	float _p2[2];
	float _color[3];
	int _filled;
	int _x; //position in the grid
	int _y;
};

#endif