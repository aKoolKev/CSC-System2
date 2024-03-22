#include <iostream>
#include "Block.hpp"

using namespace std;


//print out block info
void 
Block::printBlockInfo()
{
    cout << getStartIndex() << "(" << getBlockSize() << ") [" << getRefNum() << "]";
}
