#ifndef MAIN_H
#define MAIN_H
//---------------------------------------------------------------------------
/*
      The following defines determine the amount of points on the drawing grid.
*/
#define WIDTH 1024
#define HEIGHT 768
//---------------------------------------------------------------------------
#include <string>
#include <sstream>
#include "dlist.h"
#include "GameOfLife.h"

//    Function declarations
int main(int argc, char* argv[]);
//windowsize = { widht, height }
void init(int width, int heigth);
void readEDIF(string fname);
string findstring(string line);

//global variables
stringstream keytext;
dlist DrawList;
string windowname;
cell*** GLOBAL_GRID; //2d array with format [x][y] of pointers to cell class

// glut callback
void reshape(int w, int h);
void display(void);
void keypressed(unsigned char key, int x, int y);
void printtext(string text, int x, int y);


#endif