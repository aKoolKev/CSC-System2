#include <iostream>
#include "Variable.hpp"

using namespace std;


//print out Variable info
void 
Variable::printVariableInfo()
{
    cout << getName() << ":" << _usedBlock->getStartIndex() << "(" << _usedBlock->getBlockSize() << ") [" << _usedBlock->getRefNum() << "]" << endl;
}
