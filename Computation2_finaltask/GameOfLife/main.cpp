////////////////////////////////////////////////////////////
//
// File: main.cpp
// Author: Frank Boerman
//
///////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <GL\freeglut\freeglut.h>
#include <sstream>
#include <string>
#include <fstream>
#include <iostream>
#include <list>
#include "SharedGlobals.h"
#include "Colours.h"

using namespace std;

//import the globals from sharedglobals headeer
cell*** GLOBAL_GRID;
int gridwidth;
int gridheight;
bool colourmode;
int colourscheme;

#define _USE_MATH_DEFINES // Signal math.h that we would like defines like M_PI
#include <math.h> // Might come in usefull for cosine functions and stuff like that

#include "main.h" // Function declarations and some definitions
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
	glutInitWindowSize(windowwidth, windowheight);				// sepcified in globals size window
	glutInitWindowPosition(0,0);					// Window appear in the top-left corner of the screen
	glutCreateWindow(windowname.c_str());		// Window title
	glutDisplayFunc(display);						// Main display function
	glutReshapeFunc(reshape);						// Reshape function, called when the window resizes
	glutMouseFunc(mouseClick);						//detects mouse click
	glutTimerFunc(500, Time_Tick, 0);

	glutKeyboardFunc(keypress); //handles all keypressing events
	glutIgnoreKeyRepeat(1); //tells glut to wait for a key to go up again, to prevent spamming of buttons

	glClearColor(1.0, 1.0, 1.0, 1.0);   // white background
	
	glMatrixMode(GL_PROJECTION);        // Next lines will adjust the projection matrix
	glLoadIdentity();                   // Reset matrix, returns viewport to origin
											// and undo possible rotations

	gluOrtho2D(0.0, (float)windowwidth, 0.0, (float)windowheight);

	glMatrixMode(GL_MODELVIEW);
	
}

void Time_Tick(int id) //timer that is hit when current generation time has ended
{

	if (paused) //check if the game is pause
	{
		return; //if so cancel the time process
	}
	Tick(); //calculate next generation (GameOfLife.h)
	int time;
	switch (speed) //check which time setting is currently selected and reactivate the time with that time
	{
	case 1:
		time = 500;
		break;
	case 2:
		time = 250;
		break;
	case 3:
		time = 100;
		break;
	case 4:
		time = 50;
		break;
	default:
		time = 500;
		break;
	}
	glutTimerFunc(time, Time_Tick, 0);
	glutPostRedisplay();
}

//p for pause
//n for 1 iteration
//r for reset
// q w e for colourschemes
void keypress(unsigned char key, int x, int y)  //handles all the hotkeys, fires the events binded to these keys
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
	case 'r': //all other options have dedicated functions except this one because its so easy
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
	case '1':
		speed = 1;
		break;
	case '2':
		speed = 2;
		break;
	case '3':
		speed = 3;
		break;
	case '4':
		speed = 4;
		break;
	case 'q':
		colourscheme = 1;
		ReCalc_Colours();
		break;
	case 'w':
		colourscheme = 2;
		ReCalc_Colours();
		break;
	case 'e':
		colourscheme = 3;
		ReCalc_Colours();
		break;
	}
	
}

void keypressed_debug(unsigned char key, int x, int y) //old function from previous part
{
	keytext.str("");
	keytext.clear();
	keytext << key << ", " << x << ", " << y << endl;
	glutPostRedisplay();
}

void printtext(std::string text, int x, int y) //same as above
{
	glRasterPos2f(x, y);
	glColor3f(0, 0, 0);
	for (char& c : text)
	{
		
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
	}
}


//---------------------------------------------------------------------------

//reshape
void reshape(int w, int h)
{

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
		//cout << "No change" << endl;
		return;
	}
	//cout << w << " " << h << endl;
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
		//cout << gridwidth << " " << gridheight << endl;
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
	else if ((gridwidth < old_gridwidth) && (gridheight >= old_gridheight))
	{
		cout << gridwidth << " " << gridheight << endl;
		//x is smaller but y is larger
		for (int x = 0; x < old_gridwidth; x++)
		{
			float p1[2], p2[2];
			p1[0] = x * CELL_DIMENSION;
			p2[0] = (x + 1) * CELL_DIMENSION;
			for (int y = 0; y < gridheight; y++)
			{
				p1[1] = y * CELL_DIMENSION;
				p2[1] = (y + 1) * CELL_DIMENSION;
				if (x < gridwidth && y < old_gridheight)
				{
					GLOBAL_GRID[x][y] = GRID_OLD[x][y];
				}
				else if (y >= old_gridheight && x < gridwidth)
				{
					square* sqr = new square(&DrawList, p1, p2, LINECOLOR, Blue);
					GLOBAL_GRID[x][y] = new cell(sqr, 0);
				}
				else if (x >= gridwidth && y < old_gridheight)
				{
					delete GRID_OLD[x][y];
				}
			}
		}
	}
	else if ((gridwidth >= old_gridwidth) && (gridheight < old_gridheight))
	{
		//x is larger but y is smaller
		for (int x = 0; x < gridwidth; x++)
		{
			float p1[2], p2[2];
			p1[0] = x * CELL_DIMENSION;
			p2[0] = (x + 1) * CELL_DIMENSION;
			for (int y = 0; y < old_gridheight; y++)
			{
				p1[1] = y * CELL_DIMENSION;
				p2[1] = (y + 1) * CELL_DIMENSION;
				if (x < old_gridwidth && y < gridheight)
				{
					GLOBAL_GRID[x][y] = GRID_OLD[x][y];
				}
				else if (x > old_gridwidth && y < gridheight)
				{
					square* sqr = new square(&DrawList, p1, p2, LINECOLOR, Blue);
					GLOBAL_GRID[x][y] = new cell(sqr, 0);
				}
				else
				{
					delete GLOBAL_GRID[x][y];
				}
			}
		}

	}
	else if ((gridwidth >= old_gridwidth) && (gridheight >= old_gridheight))
	{
		//both x and y are larger
		for (int x = 0; x < gridwidth; x++)
		{
			float p1[2], p2[2];
			p1[0] = x * CELL_DIMENSION;
			p2[0] = (x + 1) * CELL_DIMENSION;
			for (int y = 0; y < gridheight; y++)
			{
				p1[1] = y * CELL_DIMENSION;
				p2[1] = (y + 1) * CELL_DIMENSION;
				if (x < old_gridwidth && y < old_gridheight)
				{
					GLOBAL_GRID[x][y] = GRID_OLD[x][y];
				}
				else
				{
					square* sqr = new square(&DrawList, p1, p2, LINECOLOR, Blue);
					GLOBAL_GRID[x][y] = new cell(sqr, 0);
				}
			}
		}
	}
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
	-A redraw has been requested by a function after calculations
	*/
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);  // clear the backbuffer

	//iterate through the drawlist
	for (list<item*>::iterator it = DrawList.begin(), end = DrawList.end(); it != end; ++it)
	{
		(*it)->draw();
	}
	printtext(keytext.str(), 50, 50); //print text of pressed key
	glFlush();			// Execute all commands waiting to be executed
	glutSwapBuffers();	// Swap the backbuffer and frontbuffer,
	// display what has been drawn
}

void mouseClick(int button, int state, int x, int y) //is called when user clickes with the mouse on the grid
{
	if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN)) //only if leftclick is down
	{
		leftmousedown = true;
		//calculate position on the grid
		int xgrid = ceil(x / CELL_DIMENSION);
		int ygrid = ceil((windowheight - y) / CELL_DIMENSION);
		if (xgrid < gridwidth && ygrid < gridheight) //if clicked within the gird
		{
			//flip that cell, recalculate the neighbor chars and color of the cell. lastly redraw the window
			GLOBAL_GRID[xgrid][ygrid]->flip_flush();
			GLOBAL_GRID[xgrid][ygrid]->CheckNeighbors(xgrid, ygrid);
			GLOBAL_GRID[xgrid][ygrid]->Calc_Color();
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
	colourmode = true; //debug switch
	colourscheme = 1; //default colourscheme
	int gridsize[2];
	int windowsize[2];
	int* pgridsize;
	int* pwindowsize;
	int fileload;
	string filename;

	gridheight = 0;
	gridwidth = 0;
	//check the input arguments
	if (argc < 4) //not enough
	{
		cout << "Not enough input arguments" << endl;
		return -1;
	}
	else if (argc < 6)//no file load
	{
		fileload = 0;
	}
	else
	{
		fileload = atoi(argv[4]);
		filename = argv[5];
	}


	if (fileload) //if fileload is actived, call the parser
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
		if (size == nullptr) //if parser failed, exit the program
		{
			return -1;
		}
		//set the width and height of the grid in the globals
		windowwidth = *(size);
		windowheight = *(size + 1);
	}
	else
	{
		//if not fileload, calculate the necesarry formats
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
