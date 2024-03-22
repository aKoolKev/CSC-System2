#include "Command.hpp"

using namespace std;


// predicate to sort the freeList by start index 
bool sortStartIndex (const Block &b1, const Block &b2) { return (b1.getStartIndex() < b2.getStartIndex()); }
bool sortVarName (const Variable &v1, const Variable &v2) { return (v1.getName() < v2.getName()); }


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

// void 
// Command::printList()
// {
//     int commaCounter = 0;// don't print a comma at the last item in list

//     cout << "\nFree List:\n";
//     for (Variable v: _varList)
//     {
//         v.printVariableInfo();
//         if (commaCounter < _varList.size()-1)
//             cout << ", ";
//     }

//     commaCounter = 0; //reset counter
//     for (Block b: _freeList)
//     {
//         b.printBlockInfo();
//         if (commaCounter < _freeList.size()-1)
//             cout << ", ";
//     }

//     cout << endl;
// }



//default constructor
Command::Command(int initialSize)
{
    //create the initial block size
    Block initialBlock(initialSize, 0);

    if(initialBlock.isFreeBlock())
        _freeList.push_back(initialBlock);

}



//function: decrement current variable block pointer reference counter by 1
void 
Command::free(string varName)
{

    cout << "\n[   free(" + varName + "   ])\n";

    list<Variable>::iterator del = _varList.begin();

    for(Variable &var: _varList) //find it first
    {
        if (var.getName() == varName)
        {
            //decrement the variable's allocated block by 1
            var.getBlockPtr()->getRefNum()--;

            //must check the block ref counter 
            if ( var.getBlockPtr()->getRefNum() == 0) // block is a free block now
            {
                Block newB(var.getBlockPtr()->getBlockSize(), var.getBlockPtr()->getStartIndex());
                _freeList.push_back(newB);
                sortFreeList();

                _varList.erase(del); //remove it from the varList
            }

            return; //no need to keep searching
        }
        advance(del, 1);
    }
}

//function: decrement current variable block pointer reference counter by 1
void 
Command::free(Variable &var)
{
    list<Variable>::iterator del = _varList.begin();
    
    //decrement the variable's allocated block by 1
    var.getBlockPtr()->getRefNum()--;

    //must check the block ref counter 
    if ( var.getBlockPtr()->getRefNum() == 0) // block is a free block now
    {
        Block newB(var.getBlockPtr()->getBlockSize(), var.getBlockPtr()->getStartIndex());
        _freeList.push_back(newB);
        sortFreeList();

        _varList.erase(del); //remove it from the varList
    }
    advance(del, 1);

}


void
Command::dump()
{
    cout << "\n[   dump()   ]\n";
    cout << "\nVariables:\n";
    for (Variable v: _varList)
        v.printVariableInfo();

    int commaCounter = 0;// don't print a comma at the last item in list
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

void 
Command::compress()
{
    cout << "\n[   compress()   ]\n";
}

void 
Command::alloc (string variableName, int allocAmount)
{
    cout << "\n[   " + variableName + " = alloc(" + to_string(allocAmount) + ")   ]";


    //check if this block have not been allocated ready
    for (Variable var: _varList)
    {
        if (var.getName() == variableName) // this variable has already been allocated
            free(var);
        break; //found, no need to keep searching
    }

    cout << "\nvar: " << variableName << " not allocated \n";


    list<Block>::iterator del = _freeList.begin();
    
    cout << "\nattempting first fit...\n";

    for (Block &b: _freeList) //first fit
    {
        cout << "curr b:"; b.printBlockInfo(); cout << endl;
        if (b.getBlockSize() >= allocAmount)
        {
            cout << "firstFit found: ";
            b.printBlockInfo(); cout << endl;

            int leftOverSize = b.getBlockSize() - allocAmount;

            //figure out if there is any leftovers
            if (leftOverSize > 0) // left over free space
            {
                cout << "\nthere's leftover\n";

                // Block b2(allocAmount, b.getStartIndex());
                // cout << "b2: "; b2.printBlockInfo();
                Block* ptr = new Block(allocAmount, b.getStartIndex());
                Variable v1(variableName, ptr);
                //cout << "v1: "; v1.printVariableInfo();
                _varList.push_front(v1);
                sortVarList();

            
                b.getBlockSize() = leftOverSize; 
                b.getStartIndex() = b.getEndIndex() - (b.getBlockSize() -1);// update new start
                // end index should be the same
                //this free block remains in the freeList

                //update new free block size
                cout << "\nleft over free block:";
                b.printBlockInfo(); cout << endl;

                cout << "\nfirstFit worked!\n";
                return;

            } 
            else if (leftOverSize == 0) //exact fit
            {
                Block* ptr = &b;
                Variable v1(variableName, ptr);
                _varList.push_front(v1);
                sortVarList();

                _freeList.erase(del);
                cout << "\nfirstFit worked!\n";
                return;
            }
            
        }

        advance(del,1);

    }//first fit for-loop
    cout << "No first fit!\n";
}



void 
Command::equal(string lhsID, string rhsID)
{
    cout << lhsID + " = " + rhsID << endl;
    
    //find the lhs and rhs variable
    list<Variable>::iterator lhsPtr = _varList.begin();
    list<Variable>::iterator rhsPtr = _varList.begin();

    bool lhsFound = false;
    bool rhsFound = false;

    cout << "trying to locate the two variable of interests...\n";

    for (Variable var : _varList)
    {
        if (var.getName() == lhsID) //found lhs variable
        {
            lhsFound = true;
            cout << "found LHS!\n";
        }
        else if (var.getName() == rhsID) // found rhs variable
        {
            cout << "found RHS!: ";
            rhsFound = true;
            rhsPtr->printVariableInfo();
            
        }

        if (!lhsFound)
            advance(lhsPtr,1);
        if (!rhsFound)
            advance(rhsPtr,1);
    }



    //free previous lhs variable allocation
    if (lhsPtr != _varList.end())
    {
        free(*lhsPtr); 
        cout << "free previous lhs variable allocation...";
    }


    cout << "create new LHS variable allocation...\n";

    cout << "here0\n";
    //create new LHS variable allocation
    cout << "rhsPtr: "; rhsPtr->printVariableInfo(); cout << endl;
    cout << "here1\n";
    Variable v1(lhsID, rhsPtr->getBlockPtr());

    cout << "here2\n";
    //add it it to the var list 
    _varList.push_front(v1);
    sortVarList();

    cout << "here3\n";
    cout << "done!\n";

    /* algorithm
    

        //yes => 
            free(it)
            create new var 
                points to rhs var's block
                put lhs var into varList
        //no => 
            create new var 
                points to rhs var's block
                put lhs var into varList

    */

    
    
}
