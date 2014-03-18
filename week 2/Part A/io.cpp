////////////////////////////////////////////////////////////////
//
// Task Binary Tree
// File: io.cpp
// Name: Frank Boerman
// Student number: 0802910
//
// Contents: The file IO member functions of the
//           classes tree and branch
//
// You'll need to complete functions in this file.
// Mail this file to computation@ics.ele.tue.nl
// Skeleton functions by patrick@magma-da.com
////////////////////////////////////////////////////////////////
#include "tree.h"

#include <fstream>
#include <iostream>
#include <string>

using namespace std;

//
// Reads all strings from the file 'fname' into
// the tree. This function first opens the file,
// and then reads all words from the file (e.g.
// using fscanf(fp,"%s",word)  )
// Each word is added to the tree using add(word);
// finally, the file stream is closed.
//
void tree::read_from_file(string filename)
{
	string word;
	ifstream fp;

    // If tree is not empty, destroy tree first
    destroy();

    // Open the file
    fp.open(filename, ios::in);
	if (!fp.is_open()) {
	   cerr << "Cannot open file \"" << filename << "\"" << endl;
	   return;
	}

    cout << "Reading file \"" << filename << "\"" << endl;

	cout << word << endl;
    // Continue until the end of file
	while (!fp.eof()) {
		fp >> word;
		if (!fp.eof()) { // prevent last line twice
			add(word);
		}
	}

    // Close the file
	fp.close();
}

//
// Writes the entire contents of the tree into a file
// called fname. The words are saved one per line.
// This functions calls branch::write_pre_order(..)
// on the root branch to output the strings into
// the file.
//
void tree::write_into_file(string fname) const
{
	ofstream stream (fname, ofstream::out);
	_root->write_pre_order(&stream);
	stream.close();
    cout << "Tree written to file '" << fname << "'" << endl;
}

//
// Recursively prints the strings of the tree
// into file. Print them out 'in order', meaning
// that we first print the contents of the branch (followed
// by a '\n'), and then recurse into each of the two
// children _left and _right.
// This function is called from tree::write_into_file(..)
//
void branch::write_pre_order(ofstream* file) const
{
    *file << _word << endl;

    if (_left)
        _left->write_pre_order(file);

    if (_right)
        _right->write_pre_order(file);
}