////////////////////////////////////////////////////////////
//
// File: main.cpp
// Author: Frank Boerman
//
///////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>			 // glut header file. Never include glut before stdlib.h!
#include <sstream>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

#define _USE_MATH_DEFINES	// Signal math.h that we would like defines like M_PI
#include <math.h>			// Might come in usefull for cosine functions and stuff like that

#include "main.h"			// Function declarations and some definitions
#include "dlist.h"			// base class of the drawtools
#include "drawtools.h"      // contains all you need to draw stuff
#include "GameOfLife.h"


//    Global variables
// None! (at the moment)

/*
      void init(void)
      This function is called when glut is initialized. Use it to initialize
      variables and setup things that have to be done BEFORE the actual
      picture is drawn.
*/
void init(int width, int height)
{
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);	// Double buffering, RGB format
	glutInitWindowSize(width, height);				// 1024 x 500 pixel window
	glutInitWindowPosition(0,0);					// Window appear in the top-left corner of the screen
	glutCreateWindow(windowname.c_str());		// Window title
	glutDisplayFunc(display);						// Main display function
	glutReshapeFunc(reshape);						// Reshape function, called when the window resizes
	glutKeyboardFunc(keypressed);					//detects key pressing

	glClearColor(1.0, 1.0, 1.0, 1.0);   // white background
	
	glMatrixMode(GL_PROJECTION);        // Next lines will adjust the projection matrix
	glLoadIdentity();                   // Reset matrix, returns viewport to origin
											// and undo possible rotations

	gluOrtho2D(0.0, (float)WIDTH, 0.0, (float)HEIGHT);

	glMatrixMode(GL_MODELVIEW);

	//float clr[3] = { 0, 0, 0.5 };
	//float p2[2] = { 100, 100 };
	//float p3[2] = { 200, 200 };
	//square* ln = new square(&DrawList, p2, p3, clr);
	//ln->print();

	//float p4[2] = { 300, 300 };
	//float p5[2] = { 400, 400 };
	//ln = new square(&DrawList, p4, p5, clr);
	//ln->flip();
	//ln->print();
	
}

void keypressed(unsigned char key, int x, int y)
{
	keytext.str("");
	keytext.clear();
	keytext << key << ", " << x << ", " << y << endl;
	glutPostRedisplay();
}

void printtext(std::string text, int x, int y)
{
	glRasterPos2f(x, y);
	glColor3f(0, 0, 0);
	for (char& c : text)
	{
		
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
	}
}


//---------------------------------------------------------------------------
//	reshape: Handle window resizing (reshaping) events
//	The reshape callback is called when the window is resized or moved
//	In the current implementation, the drawing grid is extended or truncated 
//	when the window is reshaped
void reshape(int w, int h) 
{	
	glViewport(0.0f, 0.0f, (GLfloat)w, (GLfloat)h);
	glMatrixMode(GL_PROJECTION);  
	glLoadIdentity();
	gluOrtho2D(0.0, (GLdouble)w, 0.0, (GLdouble)h);
}
//---------------------------------------------------------------------------
/*
	void display(void)
	The main display callback. This callback is called when the screen has
	to be redrawn. This is when:
	-The screen first becomes active
	-The screen is resized
	-The screen is repositioned
*/
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);  // clear the backbuffer

	printtext(keytext.str(),50,50); //print text of pressed key
	//iterate through the drawlist
	dlist::ITER iter(&DrawList);
	item* itemptr = 0;
	while (itemptr = iter.next())
	{
		itemptr->draw();
	}

	glFlush();			// Execute all commands waiting to be executed
	glutSwapBuffers();	// Swap the backbuffer and frontbuffer,
						// display what has been drawn
 }
//---------------------------------------------------------------------------
int main(int argc, char* argv[]) //arguments: .exe path, fixed windowsize(0)/fixed gridsize(1), fixed x, fixed y
{
	int gridsize[2];
	int windowsize[2];
	int* pgridsize;
	int* pwindowsize;

	pgridsize = gridsize;
	pwindowsize = windowsize;

	//debug input from commandline
	//window_calc_way = 1;
	//windowsize[0] = WIDTH;
	//windowsize[1] = HEIGHT;
	//gridsize[0] = 30;
	//gridsize[1] = 30;
	//*pgridsize = 10;
	//*(pgridsize + 1) = 10;
	//*(pwindowsize) = WIDTH;
	//*(pwindowsize + 1) = HEIGHT;

	//set window and grid size
	switch ((int)*argv[1] - (int)'0')
	{
	case 0: //windowsize leading for gridsize
		*(pwindowsize) = atoi(argv[2]);
		*(pwindowsize + 1) = atoi(argv[3]);
		pgridsize = calc_grid_size(pwindowsize, CELL_DIMENSION);

		break;
	case 1: //gridsize leading
		*(pgridsize) = atoi(argv[2]);
		*(pgridsize + 1) = atoi(argv[3]);
		pwindowsize = calc_window_size(pgridsize, CELL_DIMENSION);
		break;
	default:
		cout << "False input arguments given" << endl;
		return -1;
	}

	//init the grid
	//for some reason the new commands change the pgridsize and pwindowsizearray, so save the value in a normal int, no idea why
	int x = *pgridsize;
	int y = *(pgridsize + 1);
	int width = *pwindowsize;
	int height = *(pwindowsize + 1);
	GLOBAL_GRID = new cell**[x];
	for (int i = 0; i < x; i++)
	{
		GLOBAL_GRID[i] = new cell*[y];
	}
	*pgridsize = x;
	*(pgridsize + 1) = y;
	fill_grid(&GLOBAL_GRID, &DrawList, x, y);

	// standard GLUT initialization 
	windowname = "Final Task: Game Of Life";
	//readEDIF("test2.edif");
	glutInit(&argc,argv);
	init(width, height);
	printf("starting Glut Loop...\n");
	glutMainLoop();				// Start drawing proceidure
	return 0;
}
//---------------------------------------------------------------------------

string findstring(string line)
{
	int first = line.find_first_of("\"");
	int last = line.find_last_of("\"");
	return line.substr(first, last - first);
}

void readEDIF(string fname)
{
	ifstream file;
	string fline;
	file.open(fname);
	if (file.fail())
	{
		cout << "Error reading file " << fname << endl;
		return;
	}
	cout << "Parsing file " << fname << endl;
	while (getline(file, fline))
	{
		stringstream sstream(fline);
		string command, title, ftext;
		float p1[2], p2[2], clr[3], width;
		pixel* pxl;
		line* ln;
		text* txt;

		if (fline[0] != '.' || fline ==  "")
		{
			//comment line
			continue;
		}

		switch (fline[1])
		{
		case 'd': //change window title
			windowname = findstring(sstream.str());
			cout << "Parsed windowname: " << windowname << endl;
			break;
		case 'p': //draw pixel
			sstream >> command >> p1[0] >> p1[1] >> clr[0] >> clr[1] >> clr[2];
			pxl = new pixel(&DrawList, p1, clr);
			cout << "Parsed a pixel: ";
			pxl->print();
			break;
		case 't': //draw text
			sstream >> command >> p1[0] >> p1[1] >> clr[0] >> clr[1] >> clr[2];
			ftext = findstring(sstream.str());
			txt = new text(&DrawList, ftext, clr, p1[0], p1[1]);
			cout << "Parsed a string: ";
			txt->print();
			break;
		case 'l': //draw line
			sstream >> command >> p1[0] >> p1[1] >> p2[0] >> p2[1] >> clr[0] >> clr[1] >> clr[2] >> width;
			ln = new line(&DrawList, p1, p2, clr, width);
			cout << "Parsed a line: ";
			ln->print();
			break;
		}
	}
}