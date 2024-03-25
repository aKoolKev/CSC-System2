/*
  ┌────────────────────────────────────────────┐
    FILE:    Variable.cpp
    AUTHOR:  Kevin Dong
    DATE:    3/20/24
    PURPOSE: Definition of the Variable class.
  └────────────────────────────────────────────┘
*/

#include <iostream>
#include "Variable.hpp"

using namespace std;

//default constructor
Variable::Variable(std::string id, Block* usedBlock)
{
    _varID = id;                                                                           
    _usedBlock = usedBlock;

    // increase the reference counter of the block this variable is pointing to                                                              
    increRefCounter();
} 

//print Variable private variables.
void 
Variable::printVariableInfo()
{
    cout << getName() << ":" << _usedBlock->getStartIndex() << "(" << _usedBlock->getBlockSize() << ") [" << _usedBlock->getRefNum() << "]" << endl;
}
