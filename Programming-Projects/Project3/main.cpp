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

#include "Block.hpp"
#include "Variable.hpp"

#include <list> // STL list

using namespace std;




void addToList(list<Block> &freeList, Block insertBlock)
{
  if(insertBlock.getRefNum() == 0) // no references to this block thus a free block 
    freeList.push_back(insertBlock);
  else
    cout << "ERROR: Failed to add to freeList. Block reference count: " << insertBlock.getRefNum() << endl;
}

void printList(list<Block> &freeList)
{
  cout << "Free List:\n";

  int commaCounter = 0;// don't print a comma at the last item in list

  for (Block b: freeList)
  {
    b.printBlockInfo();
    if (commaCounter < freeList.size()-1)
      cout << ", ";
  }
}



int main(int argc, char *argv[])
{
  //prompt for initial size of the heap
  cout << "Please enter the initial freelist (heap) size: ";
  int initSize=0;
  cin >> initSize;

  //invalid initial size
  if (initSize <= 0)
    cerr << "Enter an actual size!";


  //prompt for input file
  cout << "Please enter the name of an input file: ";
  string fileName;
  cin >> fileName;

  ifstream ifile(fileName);

  //invalid input files
  if (!ifile)
    cerr << "Enter a valid input file name!";
 
  // //a list containing free blocks (blocks with zero reference count)
  // list<Block> freeList;
  
  // //create the inital free block of specified size
  // Block initialBlock (initSize, 0);

  // addToList(freeList, initialBlock);

  // //debug
  // printList(freeList);

  // create an initial Token object
  Token tok;

  // start parsing and stores the result (sucessful or failed)
  bool successful = prog(tok, ifile, initSize);

  // parsing was successful...print out the "beautified" code
  if (successful)
  {
    cout << "\n# successful code =========================\n";
    //printSourceFile();
  }
  else // parsing failed...just indicate such
    cout << "\n# UNsuccessful code =========================\n";



  // cout << "Created b1\n";
  // Block b1(478, 34+478-1);
  // b1.printBlockInfo();
  // cout << "\n\n";

  // //testing Variable class
  // cout << "Created v0\n";
  // Variable v0("a", 34, 0, b1);
  // v0.printVariableInfo(); 
  // cout << endl;
  // b1.printBlockInfo();

  // cout << "Created v1\n";
  // Variable v1("b", 7, 90, b1);
  // v1.printVariableInfo(); 
  // cout << endl;
  // b1.printBlockInfo();


  // cout << "Created v2\n";
  // Variable v2("c", 7, 90, b1);
  // v2.printVariableInfo(); 
  // cout << endl;
  // b1.printBlockInfo();
  // cout << "\n\n\n";

 

  

  //debug
  // Block b2(17,17+17-1);
  // Block b3(14,20+14-1);

  
  //list<Block> freeList; // a list containing free blocks (blocks with zero reference count)

  //debug
  // freeList.push_back(b1);
  // freeList.push_back(b2);
  // freeList.push_back(b3);


  // cout << "Not sorted:\n";
  // for (Block b: freeList)
  // {
  //   b.printBlockInfo();
  //   cout << ", ";
  // }
  // cout << '\n';

  // cout << "\n\nSorted:\n";

  // freeList.sort(sortStartIndex);
  // for (Block b: freeList)
  // {
  //   b.printBlockInfo();
  //   cout << ", ";
  // }
  // cout << '\n';

  

  

  

  return 0;
}
