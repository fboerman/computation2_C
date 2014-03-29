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
#include "SharedGlobals.h"

using namespace std;
cell*** GLOBAL_GRID;
int gridwidth;
int gridheight;

#define _USE_MATH_DEFINES // Signal math.h that we would like defines like M_PI
#include <math.h> // Might come in usefull for cosine functions and stuff like that

#include "main.h" // Function declarations and some definitions
#include "dlist.h" // base class of the drawtools
#include "drawtools.h" // contains all you need to draw stuff
#include "GameOfLife.h"	
#include "Parser.h"


//cell*** GLOBAL_GRID;
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
	//glutReshapeFunc(reshape);						// Reshape function, called when the window resizes
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
	Tick();
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
		Tick();
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

//reshape help functions
void reshape(int w, int h) 
{
	cout << "resizing" << endl;
	int gridsize[2];
	int windowsize[2];
	int* pgridsize;
	int* pwindowsize;
	pgridsize = gridsize;
	pwindowsize = windowsize;

	//copy current grid pointer and properties
	cell*** GRID_OLD = GLOBAL_GRID;
	int old_gridwidth = gridwidth;
	int old_gridheight = gridheight;

	 //redo all the init things from main() in the "windowsize leading" mode
	glViewport(0.0f, 0.0f, (GLfloat)w, (GLfloat)h);
	glMatrixMode(GL_PROJECTION);  
	glLoadIdentity();
	gluOrtho2D(0.0, (GLdouble)w, 0.0, (GLdouble)h);

	*(pwindowsize) = w;
	*(pwindowsize + 1) = h;
	pgridsize = calc_grid_size(pwindowsize, CELL_DIMENSION);
	if ((gridwidth == *pgridsize) && (gridheight == *(pgridsize + 1)))
	{
		cout << "No change" << endl;
		return;
	}
	gridwidth = *pgridsize;
	gridheight = *(pgridsize + 1);
	windowwidth = w;
	windowheight = h;

	GLOBAL_GRID = new cell**[gridwidth];
	for (int i = 0; i < gridwidth; i++)
	{
		GLOBAL_GRID[i] = new cell*[gridheight];
	}

	//copy pionters of old grid, delete and create cells where necesarry
	if ((gridwidth < old_gridwidth) && (gridheight < old_gridheight))
	{
		//both x and y are smaller
		for (int x = 0; x < old_gridwidth; x++)
		{
			for (int y = 0; y < old_gridheight; y++)
			{
				if (x < gridwidth && y < gridheight)
				{
					GLOBAL_GRID[x][y] = GRID_OLD[x][y];
				}
				else
				{
					delete GRID_OLD[x][y];
				}
			}
		}
	}
	else if ((gridwidth < old_gridwidth) && (gridheight > old_gridheight))
	{
		//x is smaller but y is larger
		for (int x = 0; x < old_gridwidth; x++)
		{
			float p1[2], p2[2];
			p1[0] = x * CELL_DIMENSION;
			p2[0] = (x + 1) * CELL_DIMENSION;
			for (int y = 0; y < old_gridheight; y++)
			{
				p1[1] = y * CELL_DIMENSION;
				p2[1] = (y + 1) * CELL_DIMENSION;
				if (x < gridwidth && y < old_gridheight)
				{
					GLOBAL_GRID[x][y] = GRID_OLD[x][y];
				}
				else if (x < gridwidth)
				{
					square* sqr = new square(&DrawList, p1, p2, LINECOLOR, CELLCOLOR);
					GLOBAL_GRID[x][y] = new cell(0, sqr, 0);
				}
				else if (x > gridwidth)
				{
					delete GRID_OLD[x][y];
				}
			}
		}
	}
	else if (gridwidth < old_gridwidth)
	{
		//x is smaller but y stays the same
		for (int x = 0; x < old_gridwidth; x++)
		{
			for (int y = 0; y < old_gridheight; y++)
			{
				if (x < gridwidth)
				{
					GLOBAL_GRID[x][y] = GRID_OLD[x][y];
				}
				else
				{
					delete GRID_OLD[x][y];
				}
			}
		}
	}
	//else if ((gridwidth > old_gridwidth) && )
	glutPostRedisplay();

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
		leftmousedown = true;
		int xgrid = ceil(x / CELL_DIMENSION);
		int ygrid = ceil((windowheight - y) / CELL_DIMENSION);
		if (xgrid < gridwidth && ygrid < gridheight)
		{
			GLOBAL_GRID[xgrid][ygrid]->flip_flush();
			GLOBAL_GRID[xgrid][ygrid]->CheckNeighbors(xgrid, ygrid);
			glutPostRedisplay();
		}
	}
	else if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_UP))
	{
		leftmousedown = false;
	}
}

//---------------------------------------------------------------------------
int main(int argc, char* argv[]) //arguments: .exe path, fixed windowsize(0)/fixed gridsize(1), fixed x, fixed y, fileload(0/1), filename
{
	int gridsize[2];
	int windowsize[2];
	int* pgridsize;
	int* pwindowsize;

	gridheight = 0;
	gridwidth = 0;
	if (argc < 6)
	{
		cout << "Not enough input arguments" << endl;
		return -1;
	}
	int fileload = atoi(argv[4]);
	string filename = argv[5];

	if (fileload)
	{
		int* size;
		if (atoi(argv[1]))
		{
			size = GOLParser(filename, atoi(argv[2])*CELL_DIMENSION, atoi(argv[3])*CELL_DIMENSION, &DrawList, &CELL_DIMENSION);
		}
		else
		{
			size = GOLParser(filename, atoi(argv[2]), atoi(argv[3]), &DrawList, &CELL_DIMENSION);
		}
		windowwidth = *(size);
		windowheight = *(size + 1);
	}
	else
	{
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

		fill_grid(&DrawList, gridwidth, gridheight,CELL_DIMENSION);
	}
	//float clr[3] = { 0.5, 0, 0 };
	//new text(&DrawList, "TESTSTRING", clr, 100, 100);
	// standard GLUT initialization 
	windowname = "Final Task: Game Of Life";
	//readEDIF("test2.edif");
	glutInit(&argc,argv);
	paused = true;
	leftmousedown = false;
	init();
	cout << "starting Glut Loop..." << endl;
	glutMainLoop();				// Start drawing proceidure
	return 0;
}
//---------------------------------------------------------------------------
