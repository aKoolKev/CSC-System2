/*******************************************
 * File: main.cpp                          *
 * Author: Kevin Dong                      *
 * Date: 1/29/23                           *
 * PURPOSE: Driver for Project2            *
 *******************************************/

#include <iostream>
#include <fstream>

#include "Grammar.hpp"
#include "Token.hpp"

using namespace std;

int main(int argc, char *argv[])
{
  // check to make sure an input file was specified. 
  if (argc!=2)
  {
    cerr << "USAGE: " << argv[0] << " <file>" << endl;
    return -1;
  }

  // open input file
  ifstream ifile(argv[1]);

  // if open was not successful, let user know. 
  if (!ifile)
  {
    cerr << "ERROR: Could not open file:\"" << argv[1] << "\"" <<endl;
    return -1;
  }

  Token tok;

  //start parsing and stores the result
  bool successful = program(tok, ifile);

  // it worked...print out the "beautified" code
  if (successful)
  {
    cout << "\n# successful code =========================\n";
    printSourceFile();
  }
  else // failed...just indicate such
    cout << "\n# UNsuccessful code =========================\n";

  return 0;
}
