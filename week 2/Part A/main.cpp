
///////////////////////////////////////////////////
//
// Task Binary Tree
// Name: Frank Boerman
//
// File: main.cpp
//
// Contents: The main() of the binary tree function
//           function with the command parser
//
// This file is fully implemented. Do not mail it in
// if you left it unchanged.
///////////////////////////////////////////////////

#include "tree.h"      // contains class definitions
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

// prototypes of some help functions we need below
void print_help(void);
char read_command(void);
string read_word(void);

//
// Main routine of our binary tree based string database.
// This contains just a simple command parser.
// It instantiates as single object of class tree (my_tree)
// that maintains the tree structure.
// The commands in the case switch call the individual member
// functions on the object my_tree.
//
// Hint: implement the missing commands in the order of the
// switch() statement below.
//

void main()
{
   //
   // We're instantiating a single object of type tree
   // The root is a stack object, so at the end of thins
   // main function, the destructor for the tree elements will be
   // called.
   //
   tree my_tree;

   cout << "            ----  Welcome to the word tree -----" << endl << endl;

   /*
    * Our 'endless' command parsing loop
    */
   while(1) {
      switch(read_command()) {  // gets a single-character command from the command line

      case 'a':   /* a - Adds a string to the database. */
         my_tree.add(read_word());
         break;
      case 'l':   /* l - Lists the database alphabetically. */
         my_tree.print_words_alphabetically();
         cout << endl;
         break;
      case 'h':   /* h - Prints the maximum height of the tree. */
         my_tree.print_height();
         break;
      case 'p':   /* p - Plots the tree containing the database. */
         my_tree.print();
         break;
      case 'f':   /* f - Finds a string in the database. */
         my_tree.print_whether_word_exists(read_word());
         break;
      case 'd':   /* d - Destroys all strings in the tree database */
         my_tree.destroy();
         my_tree.print_count();
         break;
      case 'c':   /* c - Counts & prints the number of entries in the tree. */
         my_tree.print_count();
         break;
      case 's':   /* s - Saves the database to a file. */
         my_tree.write_into_file(read_word());
         break;
      case 'o':   /* o - Opens a file: read all strings from a file. */
         my_tree.read_from_file(read_word());
         my_tree.print_count();
         break;
      case 'r':   /* r - Removes a string from the database. */
         my_tree.remove(read_word());
         break;
      case 'q':   /* q - Quits the program. */
         cout << "Bye bye" << endl;
         // Note: the death of my_tree will remove the objects!
      	 return; // done
      default:
         cout << "Unknown command" << endl;
         // no break statement here: always print help in this case
      case '?':   /* ? - Prints help. */
         print_help();
         break;
      }
   }
}


//
// Read a single word from the input
//
string read_word()
{
   string s;
   printf("Enter string: ");
   cin >> s;
   return s;
}

//
// Read a one-character command from the input.
// Function will return a char
//
char read_command(void)
{
   char val;
   cout << "Command [Add|Remove|Destroy|Count|Find|List|Plot|Height|Save|Open|Quit]: ";
   cin >> val;
   return val;
}

//
// Print help command.
//
void print_help(void)
{
   cout << "Command list:" << endl;
   cout << "a - Add a string to the database." << endl;
   cout << "f - Find a string in the database." << endl;
   cout << "r - Remove a string from the database." << endl;
   cout << "c - Clear all strings in the database." << endl;
   cout << "l - List the database alphabetically." << endl;
   cout << "p - Plot the tree containing the database." << endl;
   cout << "h - Height: print the maximum height of the tree." << endl;
   cout << "c - Count: print the number of words in the tree." << endl;
   cout << "s - Save the database into a file." << endl;
   cout << "o - Open a file: read all words from a file." << endl;
   cout << "q - Quit the program." << endl;
   cout << "? - Print this help." << endl;
}
