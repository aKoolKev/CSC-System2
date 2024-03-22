#ifndef _BLOCK_HPP_
#define _BLOCK_HPP_

class Block
{
private:
    unsigned int _blockSize;    // the size of the block
    unsigned int _startIndex;   // start index of the block
    unsigned int _endIndex;     // end index of the block
    int _referenceNum; // number references to the block

public:

    Block(unsigned int size, unsigned int startIndex): _blockSize(size), 
                                                    _startIndex(startIndex),
                                                    _endIndex(_startIndex+_blockSize-1),
                                                    _referenceNum(0)
                                                    {};

    //accessor and modifier methods
    unsigned int getStartIndex() const {return _startIndex;}
    unsigned int& getStartIndex() {return _startIndex;}

    unsigned int getEndIndex() const {return _endIndex;}
    unsigned int& getEndIndex() {return _endIndex;}

    unsigned int getBlockSize() const {return _blockSize;}
    unsigned int& getBlockSize() {return _blockSize;}

    int getRefNum() const {return _referenceNum;}
    int& getRefNum() {return _referenceNum;}

    //print out block info
    void printBlockInfo();

    bool isFreeBlock() {return (_referenceNum == 0);}
};
#endif
