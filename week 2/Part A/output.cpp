///////////////////////////////////////////////////
//
// Task Binary Tree
// File: output.cpp
//
// Contents: The printf member functions of the
//           classes tree and branch
//
// This file is fully implemented. If you did not
// edit it, you also do not need to mail it in.
// Skeleton function by patrick@magma-da.com
///////////////////////////////////////////////////

#include "tree.h"

#include <fstream>
#include <iostream>
#include <string>

using namespace std;

//
// Prints the words in alphabetical order.
// Wrapper funtion on class tree:
// calls branch::print_word_alphabetically() on the _root branch.
//
void tree::print_words_alphabetically(void) const
{
    if (_root)
        _root->print_words_alphabetically();
}

//
// Prints the maximum height of the tree.
// Wrapper funtion on class tree: calls
// branch::height() on the _root branch.
//
void tree::print_height(void) const
{
    int height = 0;
    if (_root)
        height = _root->height();

    cout << "The height of the tree is " << height << endl;
}

//
// Prints the number of words (= branches) contained in the tree.
// Wrapper funtion on class tree: calls
// branch::count() on the _root branch.
//
void tree::print_count(void) const
{
    int count = 0;
    if (_root)
        count = _root->count() + 1;

    cout << "The tree contains " << count << " branch" << (count == 1 ? "" : "es") << endl;
}

//
// Prints whether a word exists in the tree.
// Wrapper funtion on class tree: calls
// branch::find(word) on the _root branch.
//
void tree::print_whether_word_exists(string word) const
{
    if (_root && _root->find(word))
        cout << "A branch containing '" << word << "' does indeed exist in the tree." << endl;
    else
        cout << "A branch containing '" << word << "' does not exist in the tree." << endl;
}


//
// Help routine to print 'count' spaces recursively
//
static void print_spaces(int count)
{
    if (count <= 0)
        return;
    cout << " ";
    print_spaces(count - 1);
}

//
// recursively prints a single level of the tree
//
void branch::printlevel(int level, int width) const
{
    int half_width = width / 2;
    if (level == 1) {
        print_spaces(half_width);
        cout << _word;
        print_spaces(half_width - _word.length());
    } else {
        if (_left)
            _left->printlevel(level - 1, half_width);
        else
            print_spaces(half_width);
        if (_right)
            _right->printlevel(level - 1, half_width);
        else
            print_spaces(half_width);
    }
}


//
// Plots the contents of the tree, with the root node in
// the top center of the screen.
// Due to the limited size of a screen, this function will only
// produce nice output on small trees with short words.
//
void tree::print() const
{
    print_count();
    if (!_root)
        return; // tree is empty

    int height = _root->height();
    if (height < 0) {
        cerr << "ERROR: cannot plot tree. You must implement member function branch::height() first. " << endl;
        return;
    }

    // Print out tree, level by level
    for (int i = 1; i <= height; i++) {
        _root->printlevel(i, 64);
        cout << endl;;
    }
}
