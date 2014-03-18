////////////////////////////////////////////////////////////////
//
// Task Binary Tree
// File: branch.cpp
// Name: Frank Boerman
// Student number: 0802910
//
// Contents: Bodies of member functions of class 'branch'
//
// You'll need to implement member functions in this file.
// Mail this file to computation@ics.ele.tue.nl
// Skeleton function by patrick@magma-da.com
////////////////////////////////////////////////////////////////
#include "tree.h"

#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

//
// Constructor of a branch tree element
// This will allocate memory to save the word.
// It sets the _word pointer, and copies the word into it.
// It also initializes the _left and _right
// pointers to nullptr (meaning the branch is a leaf).
//
branch::branch(string word)
{
    _word = word;
    _left = _right = nullptr;
}

//
// The destructor of a branch tree element.
// Frees the memory that was allocated to store the word.
// It does NOT delete the _left and _right child branches.
// Therefore this destructor is non-recursive.
//
branch::~branch()
{
    destroy();
}

//
// Destroys and deletes this branch of the tree and everything below it.
// This means that it recursively destroys the subtrees that _left
// and _right point to.
// Finally, it self-destructs this branch object by
// calling delete(this);
//
void branch::destroy()
{
    if (_left)
        delete _left;
    if (_right)
        delete _right;
}

//returns the word hold in the branch
string branch::GetWord() const
{
	return _word.data();
}

//
// Adds a word to this tree element recursively.
//
// The tree is maintained such that the words are
// sorted alphabetically in case the tree is traversed in order.
// You can use the function str2->compare(str1). This function returns a
// number < 0 in case str1 is alphabetically before str2, and
// a number > 0 in case str2 is after str1 in the dictionary.
// In case str2->compare(str1) returns 0, the strings are equal.
// The main idea is to build the tree such that for each branch
// the _left child-tree contains words that are alphabetically to the
// left of the word, and the _right subtree the words that are
// alphabetically after the word of the branch.
//
// Your target: About 12 lines of code (and use recursion!)
//
void branch::add(string word)
{
    // Implement something along these lines:
    // compare word with _word using word->compare(_word->data());
    // if word is before _word then add word to left childtree
    // if word is after _word then add word to right childtree
    // if word is equal to _word then do nothing because the word
    //   already exists.

    // In case a subtree already exists, recursively call this
    // member function on the _left or _right subtree.
    // In case there is no such subtree, make a new leaf-branch.

	if (word.compare(_word.data()) < 0) // word needs to go on the left
	{
		if (_left == nullptr)
		{
			_left = new branch(word);
			cout << "Word '" << word << "' added to the tree." << endl;
		}
		else
		{
			_left->add(word);
		}
	}
	else if (word.compare(_word.data()) > 0) // word needs to go the the right
	{
		if (_right == nullptr)
		{
			_right = new branch(word);
			cout << "Word '" << word << "' added to the tree." << endl;
		}
		else
		{
			_right->add(word);
		}
	}
	else
	{
		cout << "Word '" << word << "' already in the tree." << endl;
	}
}

//
// Returns the length of the longest path from any
// branch to the root.
// This is a recursive routine as well.
//
// Your target: 8 lines of code.
//
int branch::height() const
{
	
	if (_right == nullptr)
	{
		if (_left == nullptr)
		{
			return 1;
		}
		else
		{
			return _left->height() + 1;
		}
	}
	else
	{
		if (_left == nullptr)
		{
			return _right->height() + 1;
		}
		else
		{
			return max(_left->height(), _right->height()) + 1;
		}
	}
	

    //cout << "The function branch::height is yet to be implemented!" << endl;
    //return -1;
}

//
// Count the number of words (branches) of the subtree.
//
int branch::count() const
{
	if (_left == nullptr)
	{
		if (_right == nullptr)
		{
			return 1;
		}
		else
		{
			return _right->count() + 1;
		}
	}
	else
	{
		if (_right == nullptr)
		{
			return _left->count() + 1;
		}
		else
		{
			return _right->count() + _left->count();
		}
	}
}

//
// Returns a pointer to the branch that contains the word
// Will return nullptr in case the word is not in this
// (sub-)tree.
//
// Target: some 15 lines (recursive)
//
branch* branch::find(string word)
{
	if (word.compare(_word.data()) < 0)
	{
		if (_left != nullptr)
		{
			return _left->find(word);
		}
		else
		{
			return 0;
		}
	}
	else if (word.compare(_word.data()) > 0)
	{
		if (_right != nullptr)
		{
			return _right->find(word);
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return this;
	}
}

//
// Prints all words of the subtree of this branch in
// alphabetical order.
// Since we maintain the tree such that it is always
// alphabetically sorted, we only need to traverse the
// tree in-order (recursively) and print the elements.
//
void branch::print_words_alphabetically() const
{
    if (_left)
        _left->print_words_alphabetically();

    cout << _word << endl;

    if (_right)
        _right->print_words_alphabetically();

}

/////////////////////////////////////////////////////////////////
//
// Implementing the functions below is optional.
// Doing so will earn you kudos.
//
/////////////////////////////////////////////////////////////////

//
// Removes a word from the tree, and returns a
// pointer to the branch that contains the deleted word. It does
// not delete the branch itself.
// ptr_to_branch_ptr is the pointer of the parent branch that should be 
// modified to update the structure of the tree.
// Remember to maintain the relative ordering of the words in the tree!
// This can be quite tricky
//
branch* branch::remove(string word, branch** ptr_to_branch_ptr)
{
	branch* victim = (*ptr_to_branch_ptr)->find(word);
	branch* parent = (*ptr_to_branch_ptr)->FindParent(word);

	parent->Reshuffle();
	return victim;
}

void branch::add_rel(branch* newBranch)
{
	if (newBranch->GetWord().compare(_word.data()) < 0) // word needs to go on the left
	{
		if (_left == nullptr)
		{
			_left = newBranch;
		}
		else
		{
			_left->add_rel(newBranch);
		}
	}
	else if (newBranch->GetWord().compare(_word.data()) > 0) // word needs to go the the right
	{
		if (_right == nullptr)
		{
			_right = newBranch;
		}
		else
		{
			_right->add_rel(newBranch);
		}
	}
}

void branch::Reshuffle()
{
	branch* right_old = _right;
	branch* left_old = _left;
	_right = nullptr;
	_left = nullptr;

	if (right_old != nullptr)
	{
		right_old->Reassign(this);
	}
	if (left_old != nullptr)
	{
		left_old->Reassign(this);
	}
}

void branch::Reassign(branch* root)
{
	branch* oldright = _right;
	branch* oldleft = _left;
	_right = nullptr;
	_left = nullptr;

	if (oldright != nullptr)
	{
		oldright->Reassign(root);
		if (oldleft != nullptr)
		{
			oldleft->Reassign(root);
		}

	}
	if (oldleft != nullptr)
	{
		oldleft->Reassign(root);
	}
	root->add_rel(this);
}

void branch::RemoveChild(char child)
{
	switch (child)
	{
		case 'r' :
			_right = nullptr;
			break;
		case 'l':
			_left = nullptr;
			break;
		case 'b':
			_left = nullptr;
			_right = nullptr;
			break;
	}
}

//function that finds the parent of given word
branch* branch::FindParent(string word)
{
	string rightword = "";
	string leftword = "";

	if (_right != nullptr)
	{
		rightword = _right->GetWord();
	}

	if (_left != nullptr)
	{
		leftword = _left->GetWord();
	}

	if (word == leftword || word == rightword)
	{
		return this;
	}
	else if (word.compare(_word.data()) < 0)
	{
		return _left->FindParent(word);
	}
	else if (word.compare(_word.data()) > 0)
	{
		return _right->FindParent(word);
	}
}

//
// Help routine for branch::remove
// this recursive function finds the 'alphabetically largest'
// word in (sub-)tree and return a pointer to the branch that
// contains it.
//
branch* branch::find_branch_with_largest_word()
{
    if (_right)
        return _right->find_branch_with_largest_word();
    else
        return this;
}


