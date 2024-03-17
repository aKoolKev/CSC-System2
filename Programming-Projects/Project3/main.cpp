// #include <iostream>
// #include <fstream>
// #include "Token.hpp"

// using namespace std;

// int main (int argc, char* argv[])
// {
//     int blockSize = -1;

//     cout << "Please enter the initial freelist (heap) size: ";
//     cin >> blockSize;

//     string inputFile;
//     cout << "Please enter the name of an input file: ";
//     cin >> inputFile;

    

//     return 0;
// }


/*******************************************
 * File: main.cpp                          *
 * Author: Kevin Dong                      *
 * Date: 1/29/24                           *
 * PURPOSE:                                *
 *   - Building a parser                   *
 *   - Implementation of a grammar         *
 *   - Writing a “code beautifier          *
 *******************************************/

#include <iostream>
#include <fstream>

#include "Grammar.hpp"
#include "Token.hpp"

using namespace std;

int main(int argc, char *argv[])
{
  // make sure input file was specified
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

  // create an initial Token object
  Token tok;

  // start parsing and stores the result (sucessful or failed)
  bool successful = prog(tok, ifile);

  // parsing was successful...print out the "beautified" code
  if (successful)
  {
    cout << "\n# successful code =========================\n";
    printSourceFile();
  }
  else // parsing failed...just indicate such
    cout << "\n# UNsuccessful code =========================\n";

  return 0;
}
