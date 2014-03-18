///////////////////////////////////////////////////
//
// Task Binary Tree
// File: tree.cpp
// Name: Frank Boerman
// Student number: 0802910
//
// Contents: Bodies of member functions of class 'tree'
//
// This file is fully implemented. If you did not
// edit it, you do not need to mail it in.
// Skeleton function by patrick@magma-da.com
///////////////////////////////////////////////////
#include "tree.h"

#include <fstream>
#include <iostream>
#include <string>

using namespace std;

//
// Constructor of the tree class
// Initializes the tree as an empty tree.
//
tree::tree(void)
{
    _root = nullptr;
}

//
// Destructor of the tree class.
// Destroys the entire contents of the tree.
//
tree::~tree(void)
{
    destroy();
}

//
// Adds word to the tree. This member funtion is
// merely a 'wrapper' around the recursive function
// branch::add(word) that does the actual work.
//
void tree::add(string word)
{
    if (!_root) {
        // this is the first branch of the tree: make _root point to it
        _root = new branch(word);
    } else {
        // a root already exists: toss the word into the 'add' member function
        _root->add(word);
    }
}

//
// Deletes all branches of the tree. Again, this is a
// wrapper for the recursive function branch::destroy()
// that wipes out the tree.
//
void tree::destroy(void)
{
    if (_root)
        delete _root; // This calls the destructor of the branch,
    // which recursively destroys all subbranches
    _root = nullptr;
}

//
// Removes a word from the tree. Again its a wrapper
// for a member function of class branch.
//
void tree::remove(string word)
{
    if (!_root)
        return; // If there is no tree, do nothing

    // Removes the branch containing 'word'
    // To enable changing the parent pointer, we give the
    // address of the pointer that is pointing to the object
    // as a argument to the function.
    branch* dead_branch = _root->remove(word, &_root);
    if (dead_branch)
        delete dead_branch;  // delete the branch if it was extracted.
}