#ifndef _COMMAND_HPP_
#define _COMMAND_HPP_

#include <iostream>
#include <list>
#include <iterator>
#include "Block.hpp"
#include "Variable.hpp"

class Command
{
private:
    std::list<Block> _freeList;
    std::list<Variable> _varList;

public:

    Command(int size);

    void free(std::string varName);
    void free(Variable &var);

    void dump();

    void compress();

    void alloc (std::string varName, int amount);

    void equal (std::string lhsID, std::string rhsID);

    void sortFreeList();

    void sortVarList(); 
};

#endif
