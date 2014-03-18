//////////////////////////////////////////////////////////////////
//
// Task Binary Tree
// File: tree.h
//
// Contents: definitions of tree and branch classes
//
// This file is fully implemented. If you did not
// edit it, you also do not need to mail it in.
// Skeleton functions by patrick@magma-da.com
/////////////////////////////////////////////////////////////////

#include <fstream>
#include <iostream>
#include <string>

using namespace std;

class branch;    // forward declaration of the class branch

//
// The tree class owns a binary tree of words.
// As data member it contains a pointer to the root
// of the tree (which is of type 'branch').
//
class tree {
public:
    tree();  // Constructor: makes an empty tree
    ~tree(); // Destructor: destroys all branches of the tree

    // Empties the tree: destroys all branches it contains
    void destroy();

    // Adds a word to the tree
    void add(string word);
       
    // Removes a word from the tree
    void remove(string word);

    // Prints all words in alphabetical order
    void print_words_alphabetically() const;

    // Prints whether or not a word exists in the tree
    void print_whether_word_exists(string word) const;
    
    // Prints the maximum height of the tree
    void print_height() const;

    // Prints the number of words in the tree
    void print_count() const;

    // Plots the contents of the tree
    void print() const;

    // Writes the contents of the tree into a file called filename
    void write_into_file(string filename) const;

    // Reads words from a file called filename
    void read_from_file(string filename);

private:
    // pointer to the root branch of the tree
    branch* _root;  // if this is nullptr (0), the tree is empty
};

//
// The class 'branch' contains the nodes (branches) of a binary
// tree. Each node contains a string and 2 pointers
// to the left and right subtrees.
//
class branch {
public:
    branch(string word);    // Constructor: makes a branch containing 'word'
    ~branch();          // Destructor: calls destroy

    // Recursively destroys the left and right subtrees
    void destroy();

    // Returns the word contained in this branch
    string word() { return _word; }

    // Adds a word to the subtree at this branch
    void add(string word);

    // Removes the branch containing word from the tree
    branch* remove(string word, branch** ptr_ptr);

    // Prints the words of this subtree alphabetically
    void print_words_alphabetically() const;

    // Finds a word. Returns nullptr (0) if the word does not exist
    branch* find(string word);

    // Returns the maximum height of the subtree below this branch
    int height() const;

    // Returns the number of words in the subtree below this
    int count() const;

    // A print help routine: prints all words at the specified level
    void printlevel(int level, int width) const;

    // Writes the subtree out pre-order into file
    void write_pre_order(ofstream* file) const;

	//finds the parent of given word
	branch* FindParent(string word);

	//returns the word that is hold in the branch
	string GetWord() const;

	//removes on of the pointers
	void RemoveChild(char child);

	//reassignes all the items under the current branch
	void Reassign(branch* root);

	//add function, same as the standard one but this one puts an existing branch in instead of creating a new one
	void add_rel(branch* newBranch);

	//reshuffles the right and left branches of the current
	void branch::Reshuffle();

private:
    // Help routine for the remove() member function
    branch* find_branch_with_largest_word();

    string _word;   // the word that this branch contains
    branch* _left;  // the left subtree
    branch* _right; // the right subtree
};


