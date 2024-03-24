/*
  ┌─────────────────────────────────────────┐
    File: Block.hpp
    Author: Kevin Dong
    Date: 3/20/24
    Purpose: Definitions of the Block class
  └─────────────────────────────────────────┘
*/

#ifndef _BLOCK_HPP_
#define _BLOCK_HPP_

#include <iostream>

using namespace std;

class Block
{
private:
    unsigned int _blockSize;    // the size of the block
    unsigned int _startIndex;   // start index of the block
    unsigned int _endIndex;     // end index of the block
    int _referenceNum;          // number Variable references to the block

public:

    //default constructor
    Block(unsigned int size, unsigned int startIndex): _blockSize(size), 
                                                       _startIndex(startIndex),
                                                       _endIndex(_startIndex+_blockSize-1),
                                                       _referenceNum(0){};
    
    //print out Block private variables
    void printBlockInfo() {cout << getStartIndex() << "(" << getBlockSize() << ") [" << getRefNum() << "]";};

    //indicate if Block is a "free block" (zero references to it)
    bool isFreeBlock() {return (_referenceNum == 0);}

    //accessor and modifier methods

    //start index
    unsigned int getStartIndex() const {return _startIndex;}
    unsigned int& getStartIndex() {return _startIndex;}

    //end index
    unsigned int getEndIndex() const {return _endIndex;}
    unsigned int& getEndIndex() {return _endIndex;}

    //block size
    unsigned int getBlockSize() const {return _blockSize;}
    unsigned int& getBlockSize() {return _blockSize;}

    //number of references
    int getRefNum() const {return _referenceNum;}
    int& getRefNum() {return _referenceNum;}

};
#endif
