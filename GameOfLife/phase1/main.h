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
#include "dlist.h"

//    Function declarations
int main(int argc, char* argv[]);
void init(void);
void readEDIF(string fname);
string findstring(string line);

//global variables
stringstream keytext;
dlist DrawList;
string windowname;

// Place your glut callback declarations here:
void reshape(int w, int h);
void display(void);
void alarm(int alarmnumber);
void keypressed(unsigned char key, int x, int y);
void printtext(string text, int x, int y);


#endif