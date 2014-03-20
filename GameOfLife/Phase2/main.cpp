////////////////////////////////////////////////////////////
//
// File: main.cpp
// Author: Frank Boerman
//
///////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h> // glut header file. Never include glut before stdlib.h!
#include <sstream>
#include <string>
#include <fstream>
#include <iostream>
#include <list>

using namespace std;

#define _USE_MATH_DEFINES // Signal math.h that we would like defines like M_PI
#include <math.h> // Might come in usefull for cosine functions and stuff like that

#include "main.h" // Function declarations and some definitions
#include "dlist.h" // base class of the drawtools
#include "drawtools.h" // contains all you need to draw stuff
#include "GameOfLife.h"	
//#include "Parser.h"


//    Global variables
// None! (at the moment)

/*
      void init(void)
      This function is called when glut is initialized. Use it to initialize
      variables and setup things that have to be done BEFORE the actual
      picture is drawn.
*/
void init() //all glut callbacks and initializations
{
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);	// Double buffering, RGB format
	glutInitWindowSize(windowwidth, windowheight);				// 1024 x 500 pixel window
	glutInitWindowPosition(0,0);					// Window appear in the top-left corner of the screen
	glutCreateWindow(windowname.c_str());		// Window title
	glutDisplayFunc(display);						// Main display function
	glutReshapeFunc(reshape);						// Reshape function, called when the window resizes
	//glutKeyboardFunc(keypressed_debug);					//detects key pressing
	glutMouseFunc(mouseClick);						//detects mouse click
	glutTimerFunc(500, Time_Tick, 0);

	glutKeyboardFunc(keypress);
	glutIgnoreKeyRepeat(1);

	glClearColor(1.0, 1.0, 1.0, 1.0);   // white background
	
	glMatrixMode(GL_PROJECTION);        // Next lines will adjust the projection matrix
	glLoadIdentity();                   // Reset matrix, returns viewport to origin
											// and undo possible rotations

	gluOrtho2D(0.0, (float)windowwidth, 0.0, (float)windowheight);

	glMatrixMode(GL_MODELVIEW);
	
}

void Time_Tick(int id)
{

	if (paused)
	{
		return;
	}
	Tick(&GLOBAL_GRID, gridwidth, gridheight);
	glutTimerFunc(500, Time_Tick, 0);
	glutPostRedisplay();
}

//p for pause
//n for 1 iteration
//r for reset
void keypress(unsigned char key, int x, int y) 
{
	switch (key)
	{
	case 'p':
		paused = !paused;
		Time_Tick(0);
		break;
	case 'n':
		Tick(&GLOBAL_GRID, gridwidth, gridheight);
		glutPostRedisplay();
		break;
	case 'r':
		for (int x = 0; x < gridwidth; x++)
		{
			for (int y = 0; y < gridheight; y++)
			{
				if (GLOBAL_GRID[x][y]->get_status())
				{
					GLOBAL_GRID[x][y]->flip_flush();
				}
			}
		}
		glutPostRedisplay();
		break;
	}
	
}

void keypressed_debug(unsigned char key, int x, int y)
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

	//iterate through the drawlist
	dlist::ITER iter(&DrawList);
	item* itemptr = 0;
	while (itemptr = iter.next())
	{
		itemptr->draw(); //and draw the item
	}
	printtext(keytext.str(), 50, 50); //print text of pressed key
	glFlush();			// Execute all commands waiting to be executed
	glutSwapBuffers();	// Swap the backbuffer and frontbuffer,
						// display what has been drawn
 }

void mouseClick(int button, int state, int x, int y)
{
	if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN))
	{
		int xgrid = ceil(x / CELL_DIMENSION);
		int ygrid = ceil((windowheight - y) / CELL_DIMENSION);
		if (xgrid < gridwidth && ygrid < gridheight)
		{
			GLOBAL_GRID[xgrid][ygrid]->flip_flush();
			glutPostRedisplay();
		}
	}
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
	gridwidth = *pgridsize;
	gridheight = *(pgridsize + 1);
	windowwidth = *pwindowsize;
	windowheight = *(pwindowsize + 1);
	GLOBAL_GRID = new cell**[gridwidth];
	for (int i = 0; i < gridwidth; i++)
	{
		GLOBAL_GRID[i] = new cell*[gridheight];
	}

	fill_grid(&GLOBAL_GRID, &DrawList, gridwidth, gridheight);

	// standard GLUT initialization 
	windowname = "Final Task: Game Of Life";
	//readEDIF("test2.edif");
	glutInit(&argc,argv);
	paused = true;
	init();
	printf("starting Glut Loop...\n");
	glutMainLoop();				// Start drawing proceidure
	return 0;
}
//---------------------------------------------------------------------------
