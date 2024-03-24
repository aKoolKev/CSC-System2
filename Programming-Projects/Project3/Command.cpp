#include "Command.hpp"
#include <iterator>
#include <list>

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
Command::free(string varToFind) 
{

    cout << "\n[   free(" + varToFind + "   )]\n";

    list<Variable>::iterator del = _varList.begin();

    //find it first
    for(Variable &var: _varList) 
    {
        if (var.getName() == varToFind) //found
        {
            //decrement the variable's allocated block by 1
            var.getBlockPtr()->getRefNum()--;

            //must check the block ref counter 
            // var.getBlockPtr()->isFreeBlock();
            if (var.getBlockPtr()->isFreeBlock()) // block is a free block now
            {
                Block newB(var.getBlockPtr()->getBlockSize(), var.getBlockPtr()->getStartIndex());
                _freeList.push_back(newB);
                sortFreeList();
            }

            _varList.erase(del); //free the var from the var list
            return; //no need to keep searching
        }
        advance(del, 1);
    }

    cout << "Error: " + varToFind + " was not found!\n";
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

    /*
     ⚬ Loop through the freeList
                ⚬ Since sorted in ascending order, we traverse through each node (freeBlock) in the list
                ⚬ Compress two blocks if
                    startIndexBlock1 + Block1Size = startIndexBlock2
                        if true: adjacent!

                            Block1Size = Block1Size + Block2Size
                            startIndexBlock1 remains the same, but endIndex will change

                            remove the Block2 (since it has been compressed)
                            * could delete the memory leak here 
    */

    //base case: nothing to compress
    if(_freeList.size() <= 1)
        return; 
    //guarenteed there are at least 2 blocks in the free list to compress


    list<Block>::iterator currBlock = _freeList.begin();
    list<Block>::iterator nextBlock = _freeList.begin();  advance(nextBlock,1);
    
    
    //advance(nextBlock,1); // what it means to be the next block

    // int currCounter = 0;
    bool flag = false;

    // //loop through the list
    // while (currBlock != _freeList.end())
    // {
            
    //    //prevent looking beyond our list when we are at the last element
    //         if (currBlock!=_freeList.end())
    //         {
    //             advance (nextBlock,1);
    //             if (nextBlock == _freeList.end())
    //             {
    //                 cout << "\nflag\n";
    //                 flag = true;
    //             }
    //             cout << "\n\ncurrBlock: "; currBlock->printBlockInfo(); 
    //             cout << "\nNextBlock: "; nextBlock->printBlockInfo(); cout << "\n\n";
    //         }
    //         else 
    //         {
    //             cout << "\nat the end\n";
    //             break;
    //         }

        
    //         //blocks are ADJACENT!!
    //         if (currBlock->getStartIndex() + currBlock->getBlockSize() ==  nextBlock->getStartIndex()  ) 
    //         {
    //             cout << "adjacent blocks: "; currBlock->printBlockInfo(); cout << ", "; nextBlock->printBlockInfo(); cout << endl; 
    //             //adjacent! Thus can compress
    //             //Block1Size = Block1Size + Block2Size
    //             currBlock->getBlockSize() += nextBlock->getBlockSize();
    //             //recalculate Block1 endIndex
    //             currBlock->getEndIndex() = (currBlock->getStartIndex() + currBlock->getBlockSize() - 1);


    //             cout << "after compressing..."; currBlock->printBlockInfo(); cout << endl;
                
    //             //remove the 2nd block since it was "combined"
    //             list<Block>::iterator del = next(currBlock,1);
    //             _freeList.erase(del);  

    //         }
    //         else //blocks are NOT adjacent, move to next element
    //         {
    //             cout << "NOT adjacent: "; currBlock->printBlockInfo(); cout << ", "; nextBlock->printBlockInfo(); cout << endl; 
    //             cout << "Advancing currBlock \n";
    //             advance(currBlock,1);
    //         }

    //         if (flag)
    //         {    
    //             cout << "\nactivaited\n";
    //             return;
    //         }


            
    //         cout << "end: \n";

    //         cout << "currBlock: "; currBlock->printBlockInfo(); 
    //         cout << "\nNextBlock: "; nextBlock->printBlockInfo(); cout << "\n\n";
                
            
    // }
    while(currBlock != prev(_freeList.end(),1) && nextBlock!= _freeList.end())
    {

        if (nextBlock == prev(_freeList.end(),1))
        {    cout << "\nflag set\n";
            flag = true;
        }

        cout << "currBlock: "; currBlock->printBlockInfo(); cout << ", nextBlock: "; nextBlock->printBlockInfo(); cout << endl; 
        cout << "end:"; prev(_freeList.end())->printBlockInfo(); cout << endl;

        if (currBlock->getStartIndex() + currBlock->getBlockSize() ==  nextBlock->getStartIndex()  ) 
        {
            cout << "adjacent blocks: "; currBlock->printBlockInfo(); cout << ", "; nextBlock->printBlockInfo(); cout << endl; 
            //adjacent! Thus can compress
            //Block1Size = Block1Size + Block2Size
            currBlock->getBlockSize() += nextBlock->getBlockSize();
            //recalculate Block1 endIndex
            currBlock->getEndIndex() = (currBlock->getStartIndex() + currBlock->getBlockSize() - 1);


            cout << "after compressing..."; currBlock->printBlockInfo(); cout << endl;
            
            //remove the 2nd block since it was "combined"
        
            cout << "here1\n";
            list<Block>::iterator del = next(currBlock,1);
            advance(nextBlock,1);
            cout << "here2\n";
            _freeList.erase(del);

            cout << "here3\n";  
            //advance(nextBlock,1);
            cout << "here4\n";

        }
        else //blocks are NOT adjacent, move to next element
        {
            cout << "NOT adjacent: "; currBlock->printBlockInfo(); cout << ", "; nextBlock->printBlockInfo(); cout << endl; 
            cout << "Advancing currBlock \n";
            advance(currBlock,1);
            advance(nextBlock,1);
        }
        cout << "here5\n";

        if(flag)
        {
            cout << "flag triggered";
            break;
        }

            cout << "here6\n";

    }
    cout << "here7\n";
    cout << "\ndone with compress()\n";
    cout << "here8\n";

}

void 
Command::alloc (string variableName, int allocAmount)
{
    cout << "\n[   " + variableName + " = alloc(" + to_string(allocAmount) + ")   ]";

    for (Variable var: _varList)
    {
        if (var.getName() == variableName) // this variable has already been allocated
        {
            free(variableName);
            break; //found, no need to keep searching
        }
    }

    // if (newVar)

    // cout << "\nvar: " << variableName << " not allocated \n";


    list<Block>::iterator del = _freeList.begin();
    
    cout << "\nattempting first fit...\n"; //debug

    for (Block &b: _freeList) //first fit
    {
        cout << "curr b:"; b.printBlockInfo(); cout << endl; //debug

        //found a candidate
        if (b.getBlockSize() >= allocAmount)
        {
            cout << "firstFit found: "; //debug
            b.printBlockInfo(); cout << endl; //debug

            int leftOverSize = b.getBlockSize() - allocAmount;

            //handle alloc to the amount of left over space accordingly
            if (leftOverSize > 0) // left over free space
            {
                cout << "\nthere's leftover\n"; //debug

                // Block b2(allocAmount, b.getStartIndex());
                // cout << "b2: "; b2.printBlockInfo();
                
                Block* ptr = new Block(allocAmount, b.getStartIndex());
                Variable var(variableName, ptr);
                _varList.push_front(var);
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
                cout << "\nexact fit!\n";
                Block* ptr = new Block(allocAmount, b.getStartIndex());
                Variable var(variableName, ptr);
                _varList.push_front(var);
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

    // base case: LHS = RHS (essentially do "nothing")
    if (lhsID == rhsID)
        return;

    /* New Algorithm 
        - try to locate LHS and RHS  (done)
        - If LHS does not exists (done)
            - create new LHS var (done)
            - point LHS var to RHS (done)
            - put LHS var into list (done)

        - If LHS exists (LHS != RHS) 
            - free(LHS)
            - points LHS to RHS 
            - put LHS into list
        - If LHS exists (LHS = RHS) (done)
            - do nothing (done)
    */

    std::list<Variable>::iterator lhsVar = _varList.begin();
    std::list<Variable>::iterator rhsVar= _varList.begin();

    bool lhsVarFound = false;
    bool rhsVarFound = false;

    // try to find LHS and RHS 
    for (Variable &var : _varList)
    {
        string varName = var.getName();

        if (varName == lhsID) //found lhs var
        {
            cout << "found lhsVar1: " << lhsVar->getName() << endl;
            lhsVarFound = true;
        }
        else if (varName == rhsID) // found rhs var
        {
            cout << "found rhsVar1: " << rhsVar->getName() << endl;
            rhsVarFound = true;
        }
        
    
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
        //debugging
        cout << "lhsVar: "; lhsVar->getName();
        cout << "rhsVar: "; rhsVar->getName();

        if (lhsVarFound) //LHS var exists
        {
            cout << "\nLHS var exists\n";
            free(lhsID); 
            lhsVar->getBlockPtr() = rhsVar->getBlockPtr();
            _varList.push_front(*lhsVar);
            sortVarList();
            // - free(LHS)
            // - points LHS to RHS 
            // - put LHS into list
        }
        else // LHS var does not exists
        {
            cout << "\nLHS var does not exists\n";
            //create a new LHS var
            cout << "create new LHS var\n";

            Variable newLHSVar(lhsID, rhsVar->getBlockPtr());
            //put new var into varList
            _varList.push_front(newLHSVar);
            sortVarList();
        }


        // //create new lhs var and points it to the rhs block
        // Variable newLHS (lhsID, rhsVar->getBlockPtr());
        // //add lhs var into varlist and re-sort the list
        // _varList.push_front(newLHS);
        // sortVarList();

        // if (lhsVarFound) //free old LHS value
        //     free(lhsID);
    
    }
    else // error handling case where RHS variable does not exists
    {
        cout << "RHS does not exists. Does not make sense!!!\n";
    }

   
}
