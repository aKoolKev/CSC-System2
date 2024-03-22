#ifndef _VARIABLE_HPP_
#define _VARIABLE_HPP_

#include <iostream>
#include "Block.hpp"

class Variable
{
private:
    std::string _varID; // the name of the variable 
    Block* _usedBlock; // the block allocated to the variable

public:
    //default constructor
    Variable(std::string id, Block* usedBlock)
    {
        _varID = id;                                                                           
        _usedBlock = usedBlock;
        increRefCounter(); // increase the reference counter of the block this variable is pointing to                                                              
    } 
                                                                                           
    //accessor & modifier methods
    std::string getName() const {return _varID;}
    std::string& getName() {return _varID;}

    Block* getBlockPtr() const {return _usedBlock;}
    Block*& getBlockPtr() {return _usedBlock;}

    //print Variable info
    void printVariableInfo();

    //update the amount of total references of the block this variable is pointing to
    void increRefCounter() {_usedBlock->getRefNum()+=1;}

};
#endif
