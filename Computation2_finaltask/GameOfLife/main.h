////////////////////////////////////////////////////////////
//
// File: main.h
// Author: Frank Boerman
//
///////////////////////////////////////////////////////////

#ifndef MAIN_H
#define MAIN_H


//---------------------------------------------------------------------------
#include <string>
#include <sstream>
#include "drawtools.h"
#include <list>
#include "GameOfLife.h"

//    Function declarations
int main(int argc, char* argv[]);
void init();


//global variables
stringstream keytext;
std::list<item*> DrawList = std::list<item*>();
string windowname;
int speed = 1;
int windowheight;
int windowwidth;
bool paused;
bool leftmousedown;
int CELL_DIMENSION = 30; //size of the cell, only parser may change this

// glut callback
void reshape(int w, int h);
void display(void);
void keypressed_debug(unsigned char key, int x, int y);
void printtext(string text, int x, int y);
void mouseClick(int button, int state, int x, int y);
void keypress(unsigned char key, int x, int y);
void Time_Tick(int id);

#endif