/*
  ┌─────────────────────────────────────────────┐
    FILE:    Command.hpp
    AUTHOR:  Kevin Dong
    DATE:    3/20/24
    PURPOSE: Defines the Command class and 
    gets executed when called in Grammar class.
  └─────────────────────────────────────────────┘
*/

#ifndef _COMMAND_HPP_
#define _COMMAND_HPP_

#include "Block.hpp"
#include <iostream>
#include <iterator>
#include <list>
#include "Variable.hpp"

class Command
{
private:
    std::list<Block> _freeList; //a list of Block objects: represent free spaces in the heap
    std::list<Variable> _varList; //a list of Variables that have been allocated to a Block

public:

    //default constructor
    Command(int size);

    //free command: returns any storage associated with varName
    void free(std::string varName);

    /* prints each allocated variables    ID: address(size)[referenceCounter]
    and then prints each block in freeList  address(size)[referenceCounter] */
    void dump();

    //join adjacent blocks in the freeList
    void compress();

    //use first fit on freeList to allocate request amount of space to a Variable
    void alloc(std::string varName, int amount);

    //take the reference found associated the RHS Variable and associate it with the LHS Variable
    void equal(std::string lhsID, std::string rhsID);

    //sort the _freeList in ascending order by the starting addresses
    void sortFreeList();

    //sort the _varList in ascending order by variable name
    void sortVarList(); 
};

#endif
