//File: Grammar.cpp
//Author: Kevin Dong
//Date: 1/29/24
//Purpose: Define each of the grammars


#include "Grammar.hpp"
#include "Token.hpp"
#include "Command.hpp"


using namespace std;

// store the beautified code
static string SourceFile; 

// function that prints the "beautified" code
void printSourceFile()
{
    cout << SourceFile << endl << "\n\n";
}

/*
    <prog> -> <slist>
*/
bool prog(Token tok, std::ifstream &ifile, int initSize)
{
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
            SourceFile += tok.value() + " ";

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
    bool freeCommand = false;
    bool dumpCommand = false;
    bool compressCommand = false;

    //read token
    tok.get(ifile);

    if (tok.type() == ID) // -> ID
    {
        //store the ID value for rhs()
        string ID_Val = tok.value();


        SourceFile += tok.value() + " ";

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
            SourceFile += tok.value() + " ";

            //read next token...
            tok.get(ifile);
            
            if (tok.type() == ID) // -> ID LPAREN ID
            {
                SourceFile += tok.value() + " ";

                string variableName = tok.value();

                //read next token...
                tok.get(ifile); 
                
                if (tok.type() == RPAREN) // -> ID LPAREN ID RPAREN 
                {
                    SourceFile += tok.value() + " ";

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

                SourceFile += tok.value() + " ";
                return true;
            }
            else // Neither ID or RPAREN
                return false;
        }
        else if (tok.type() == ASSIGNOP) // -> ID ASSIGNOP
        {
            SourceFile += tok.value() + " ";

            if (rhs(tok, ifile, ID_Val, cmd)) // -> ID ASSIGNOP rhs
            {
                return true;
            }
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
    int allocAmount = -7;
    string rhsID;

    //read token...
    int pos = ifile.tellg();
    tok.get(ifile);

    if (tok.type() == ID) // -> ID
    {
        rhsID = tok.value();

        if (tok.value() == "alloc")
        {
            allocCommand = true;
        }
        
        SourceFile += tok.value() + " ";

        //read next token...
        pos = ifile.tellg();
        tok.get(ifile);

        if (tok.type() == LPAREN) // -> ID LPAREN
        {
            SourceFile += tok.value() + " ";

            //read next token...
            tok.get(ifile);
            
            if (tok.type() == NUM_INT) // -> ID LPAREN NUM_INT
            {
                if (allocCommand)
                    allocAmount = stoi(tok.value());

                SourceFile += tok.value() + " ";

                //read next token...
                tok.get(ifile);
                
                if(tok.type() == RPAREN) // -> ID LPAREN NUM_INT RPAREN
                {
                    if (allocCommand)
                        cmd.alloc(lhsID, allocAmount);

                    SourceFile += tok.value() + " ";
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
