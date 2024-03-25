/*
  ┌─────────────────────────────────────────────┐
    FILE:    Command.cpp
    AUTHOR:  Kevin Dong
    DATE:    3/20/24
    PURPOSE: Definition of the Command class.
  └─────────────────────────────────────────────┘
*/

#include "Command.hpp"
#include <iterator>
#include <list>

using namespace std;

// predicates to help sort the _freeList and _varList by ascending start index or variable name, respectively
bool sortStartIndex (const Block &b1, const Block &b2) { return (b1.getStartIndex() < b2.getStartIndex()); }
bool sortVarName (const Variable &v1, const Variable &v2) { return (v1.getName() < v2.getName()); }


//default constructor
Command::Command(int initialSize)
{
    //create the initial block size
    Block initialBlock(initialSize, 0);

    if(initialBlock.isFreeBlock())
        _freeList.push_back(initialBlock);
}


//returns any storage associated with varName
void 
Command::free(string varToFind) 
{
    //the variable to remove from the _varList
    list<Variable>::iterator del = _varList.begin();

    //find the variable
    for(Variable &var: _varList) 
    {
        if (var.getName() == varToFind) //found
        {
            //decrement the variable's associated block by 1
            var.getBlockPtr()->getRefNum()--;

            //check if the block is now a "free block"
            if (var.getBlockPtr()->isFreeBlock()) 
            {
                Block newB(var.getBlockPtr()->getBlockSize(), var.getBlockPtr()->getStartIndex());
                _freeList.push_back(newB);
                sortFreeList();
            }

            //deallocted the variable
            _varList.erase(del); //free the var from the var list

            return; //no need to keep searching...done
        }
        advance(del, 1);
    }

    //error handling
    cout << "Error: " + varToFind + " was not found!\n";
}


 /* prints each allocated variables    ID: address(size)[referenceCounter]
    and then prints each block in freeList  address(size)[referenceCounter] */
void
Command::dump()
{
    //print list of Variable
    cout << "Variables:\n";
    for (Variable v: _varList)
        v.printVariableInfo();

    int commaCounter = 0; //don't print a comma at the last item in list

    //print list of Blocks
    cout << "Free List:\n";
    for (Block b: _freeList)
    {
        b.printBlockInfo();
        if (commaCounter < _freeList.size()-1)
        {
            cout << ", ";
            commaCounter++;
        }
    }

    cout << "\n===============================================================\n"; 
}

//join adjacent blocks in the freeList
void 
Command::compress()
{
    //base case: nothing to compress
    if(_freeList.size() <= 1)
        return; 

    //guarantee there are at least 2 blocks in the free list to compress

    //two pointer approach
    list<Block>::iterator currBlock = _freeList.begin();
    list<Block>::iterator nextBlock = _freeList.begin(); advance(nextBlock,1);

    //prevent looking beyond the list. Stop when next Block from current Block is at the end of the list
    bool stop = false;

    //loop through the list
    while(currBlock != prev(_freeList.end(),1) && nextBlock!= _freeList.end())
    {

        //stop looking after next Block is at the end of the list
        if (nextBlock == prev(_freeList.end(),1))
            stop = true;

        //adjacent!
        if (currBlock->getStartIndex() + currBlock->getBlockSize() ==  nextBlock->getStartIndex()  ) 
        {
            //Block1Size = Block1Size + Block2Size
            currBlock->getBlockSize() += nextBlock->getBlockSize();

            //recalculate Block1 endIndex
            currBlock->getEndIndex() = (currBlock->getStartIndex() + currBlock->getBlockSize() - 1);

            advance(nextBlock,1); // advance the nextBlock iterator before it gets deleted by the erase()s
            
            //remove the 2nd block since it was "combined"
            list<Block>::iterator del = next(currBlock,1);
            _freeList.erase(del);
        }
        else //blocks are NOT adjacent, move to next block
        {
            advance(currBlock,1);
            advance(nextBlock,1);
        }

        if(stop)
            return; // done with compression
    }
}

//use first fit on freeList to allocate request amount of space to a Variable
void 
Command::alloc (string variableName, int allocAmount)
{
    //check if this variable has been allocated previously
    for (Variable var: _varList)
    {
        if (var.getName() == variableName) // has been previously allocated
        {
            free(variableName); //free it
            break; //no need to keep searching
        }
    }

    //remove the block from free list if it was an exact fit
    list<Block>::iterator del = _freeList.begin();
    

    //first fit
    for (Block &b: _freeList)
    {
        //found a candidate
        if (b.getBlockSize() >= allocAmount)
        {
            int leftOverSize = b.getBlockSize() - allocAmount;

            //handle according to the amount of left over space after allocation

            if (leftOverSize > 0) // left over free space
            {        
                Block* ptr = new Block(allocAmount, b.getStartIndex()); //the allocated amount
                Variable var(variableName, ptr); //point variable to the block
                _varList.push_front(var);
                sortVarList();

                //the left over block that remains in the free list
                b.getBlockSize() = leftOverSize; 
                b.getStartIndex() = b.getEndIndex() - (b.getBlockSize() -1); // update new start

                return;
            } 
            else if (leftOverSize == 0) //exact fit
            {
                Block* ptr = new Block(allocAmount, b.getStartIndex()); //allocate the exact size
                Variable var(variableName, ptr);
                _varList.push_front(var);
                sortVarList();

                //since exact fit, this block is no longer in the free list
                _freeList.erase(del);
                return;
            }
            
        }

        advance(del,1);
    } //first fit for-loop

    cout << "\nNo first fit!\n";
}


//take the reference found associated the RHS Variable and associate it with the LHS Variable
void 
Command::equal(string lhsID, string rhsID)
{
    // base case: LHS = RHS (essentially do "nothing")
    if (lhsID == rhsID)
        return;

    std::list<Variable>::iterator lhsVar = _varList.begin();
    std::list<Variable>::iterator rhsVar= _varList.begin();

    bool lhsVarFound = false;
    bool rhsVarFound = false;

    //try to locate LHS and RHS variable
    for (Variable var : _varList)
    {
        string varName = var.getName();

        if (varName == lhsID) //found lhs var
            lhsVarFound = true;
        else if (varName == rhsID) // found rhs var
            rhsVarFound = true;
        
        //only advance lhs and rhs iterator if it has not been found 
        if (!lhsVarFound)
            advance(lhsVar, 1);
        if (!rhsVarFound)
            advance(rhsVar, 1);

        if (rhsVarFound && lhsVarFound) //found both, no need to keep searching
            break;
    }

    // RHS exists
    if (rhsVarFound)
    {
        if (lhsVarFound) //LHS var exists
        {
            free(lhsID);  //free previous allocation

            lhsVar->getBlockPtr() = rhsVar->getBlockPtr(); // now points to RHS
            _varList.push_front(*lhsVar);
            sortVarList();
        }
        else // LHS var does NOT exists
        {
            //create a new LHS var and points it to RHS
            Variable newLHSVar(lhsID, rhsVar->getBlockPtr());

            _varList.push_front(newLHSVar);
            sortVarList();
        }
    }
    else // error handling case where RHS variable does not exists
    {
        cout << "RHS does not exists. Does not make sense!!!\n";
    }
}

//sort the _freeList in ascending order by the start addresses
void
Command::sortFreeList()
{
    _freeList.sort(sortStartIndex);
}

//sort the _varList in ascending order by variable name
void
Command::sortVarList()
{
    _varList.sort(sortVarName);
}
