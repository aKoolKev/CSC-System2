/*
  ┌───────────────────────────────────────┐
    File: Grammar.cpp
    Author: Kevin Dong
    Date: 3/14/24
    Purpose: Define each of the grammars
    and call Command class to execute it.
  └───────────────────────────────────────┘
*/

#include "Command.hpp"
#include "Grammar.hpp"
#include "Token.hpp"

using namespace std;


/*
    <prog> -> <slist>
*/
bool prog(Token tok, std::ifstream &ifile, int initSize)
{
    //object that handles all the command execution when respective grammar matches.
    Command cmd (initSize); 

    if( slist(tok, ifile, cmd) )
        return true;
    else
        return false;
}

/* 
    <slist> -> <stmt> SEMICOLON <slist> | ϵ
*/
bool slist(Token tok, std::ifstream &ifile, Command &cmd)
{
    if (stmt(tok, ifile, cmd))
    {
        //get token...
        tok.get(ifile);

        if (tok.type() == SEMICOLON) // -> <stmt> SEMICOLON
        {

            if (slist(tok, ifile, cmd)) // -> <stmt> SEMICOLON <slist>
                return true;
            else
                return false;
        }
        else
            return false;
    }
    else
        return true; // -> ϵ
}


/* 
    <stmt> -> ID LPAREN ID RPAREN |
              ID LPAREN RPAREN |
              ID ASSIGNOP <rhs>
*/
bool stmt(Token tok, std::ifstream &ifile, Command &cmd)
{
    //determine if reserved command ID name was called
    bool freeCommand = false;
    bool dumpCommand = false;
    bool compressCommand = false;

    //read token
    tok.get(ifile);

    if (tok.type() == ID) // -> ID
    {
        //store the ID value for rhs()
        string ID_Val = tok.value();

        //check if ID match the reserved command names
        if(ID_Val == "free") // -> free...
            freeCommand = true;
        else if (ID_Val == "dump") // -> dump...
            dumpCommand = true; 
        else if (ID_Val == "compress") // -> compress...
            compressCommand = true;
        

        //read next token...
        tok.get(ifile);
        
        if(tok.type() == LPAREN) // -> ID LPAREN
        {        
            //read next token...
            tok.get(ifile);
            
            if (tok.type() == ID) // -> ID LPAREN ID
            {
                //save the paramater ID before reading next token
                string variableName = tok.value(); 

                //read next token...
                tok.get(ifile); 
                
                if (tok.type() == RPAREN) // -> ID LPAREN ID RPAREN 
                {
                    if (freeCommand) // -> free(ID)
                        cmd.free(variableName);

                    return true;
                }
                else
                    return false;
            }
            else if (tok.type() == RPAREN) // -> ID LPAREN RPAREN
            {    
                if (dumpCommand) // -> dump()
                    cmd.dump();
                else if (compressCommand) // -> compress()
                    cmd.compress();

                return true;
            }
            else // Neither ID or RPAREN
                return false;
        }
        else if (tok.type() == ASSIGNOP) // -> ID ASSIGNOP
        {

            if (rhs(tok, ifile, ID_Val, cmd)) // -> ID ASSIGNOP rhs
                return true;
            else
                return false;
        }
        else // Neither LPAREN or ASSIGNOP
            return false;
    }
    else
        return false;
}
 
/* 
    <rhs> -> ID LPAREN NUM_INT RPAREN |
             ID
*/
bool rhs(Token tok, std::ifstream &ifile, string lhsID /* ID = ... */, Command &cmd)
{
    bool allocCommand = false;
    int allocAmount = 0;
    string rhsID;

    //read token...
    int pos = ifile.tellg();
    tok.get(ifile);

    if (tok.type() == ID) // -> ID
    {
        rhsID = tok.value(); //save rhs ID before reading next token

        if (tok.value() == "alloc") //alloc...
            allocCommand = true;

        //read next token...
        pos = ifile.tellg();
        tok.get(ifile);

        if (tok.type() == LPAREN) // -> ID LPAREN
        {
            //read next token...
            tok.get(ifile);
            
            if (tok.type() == NUM_INT) // -> ID LPAREN NUM_INT
            {
                if (allocCommand)
                    allocAmount = stoi(tok.value());

                //read next token...
                tok.get(ifile);
                
                if(tok.type() == RPAREN) // -> ID LPAREN NUM_INT RPAREN
                {
                    if (allocCommand) // ID = alloc(INT);
                        cmd.alloc(lhsID, allocAmount);
                    return true; 
                }
                else
                    return false;
            }
            else
                return false;
        }
        else
        {
            //UNGET TOKEN
            ifile.seekg(pos);

            cmd.equal(lhsID, rhsID);// -> ID = ID
            
            return true; // -> ID   
        }
    }
    else
        return false;
}
