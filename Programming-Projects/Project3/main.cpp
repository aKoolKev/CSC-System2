/*******************************************
 * File: main.cpp                          *
 * Author: Kevin Dong                      *
 * Date: 1/29/24                           *
 * PURPOSE:                                *
 *   - Building a parser                   *
 *   - Implementation of a grammar         *
 *   - Writing a “code beautifier          *
 *******************************************/

/*
  ┌─────────────────────────────────────────────────────────────────────────┐
    FILE:    main.cpp
    AUTHOR:  Kevin Dong
    DATE:    3/14/24
    PURPOSE: an interpreter that supports allocating dynamic memory, 
    garbage collection through a simple programming language interpreted 
    through a grammar based recursive descent parser and a lexical analyzer.
  └──────────────────────────────────────────────────────────────────────────┘
*/

#include "Block.hpp"
#include <fstream>
#include "Grammar.hpp"
#include <iostream>
#include <list>
#include "Token.hpp"
#include "Variable.hpp"

using namespace std;

int main(int argc, char *argv[])
{
  //prompt for initial size of the heap
  cout << "Please enter the initial freelist (heap) size: ";
  int initSize=0;
  cin >> initSize;

  //invalid initial size
  if (initSize <= 0)
  {
    cout << "Enter an actual size!\n";
    return -1;
  }

  //prompt for input file
  cout << "Please enter the name of an input file: ";
  string fileName;
  cin >> fileName;

  ifstream ifile(fileName);

  //invalid input files
  if (!ifile)
  {
    cout << "Enter a valid input file name!\n";
    return -1;
  }
 
  //Parser

  //create an initial Token object
  Token tok;

  //start parsing and stores the result (sucessful or failed)
  bool result = prog(tok, ifile, initSize);

  //assuming the grammar will be always be correct
  if(!result)
    cout << "\nInvalid grammar!\n";
 
  return 0;
}
