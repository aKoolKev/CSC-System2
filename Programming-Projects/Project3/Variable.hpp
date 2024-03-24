/*
  ┌───────────────────────────────────────┐
    File: Variable.hpp
    Author: Kevin Dong
    Date: 3/20/24
    Purpose: Defines the Variable class.
  └───────────────────────────────────────┘
*/

#ifndef _VARIABLE_HPP_
#define _VARIABLE_HPP_

#include "Block.hpp"
#include <iostream>

class Variable
{
private:
    std::string _varID; // the name of the variable. 
    Block* _usedBlock; // the block allocated to the variable.

public:

    //default constructor
    Variable(std::string id, Block* usedBlock);


    //accessor & modifier methods
    std::string getName() const {return _varID;}
    std::string& getName() {return _varID;}

    Block* getBlockPtr() const {return _usedBlock;}
    Block*& getBlockPtr() {return _usedBlock;}


    //print Variable private variables
    void printVariableInfo();

    //update the amount of total references of the block this variable is pointing to.
    void increRefCounter() {_usedBlock->getRefNum()+=1;}

};
#endif
