/*******************************************
 * File: Token.cpp                         *
 * Author: S. Blythe                       *
 * Date: 12/2023                           *
 * PURPOSE: implementation for Token       *
 *******************************************/

#include "Token.hpp"

#include <fstream>
#include <iomanip>

// the 2d array (table)
static int **DFA = nullptr;

using namespace std;

// the promised global for string equivalents of TokenType enumeration
string TokStr[]=
{ "ERROR", "EOF_TOK", "NUM_INT", "NUM_REAL", "ADDOP", "MULOP", "ID", "RELOP", "ASSIGNOP", "LPAREN", 
  "RPAREN",  "SEMICOLON",  "LBRACK", "RBRACK", "COMMA", "AND", "OR", "INTEGER", "FLOAT", "WHILE", 
  "IF", "THEN", "ELSE", "VOID", "BEGIN", "END"};

// This is a "list" of the keywords. Note that they are in the same order
//   as found in the TokenType enumaration. 
static string reserved[]={"int" , "float", "while", "if", "then", "else", "void", "begin", "end" };



/******************************************************
 *  just prints out the info describing this Token    *
 *    to specified stream                             *
 *                                                    *
 *   os  - the stream to add the Token to             *
 *                                                    *
 *   returns: the updated stream                      *
 ******************************************************/
ostream&
Token::print(ostream& os) const
{
  os
     << "{ Type:"   << left << setw(10) << TokStr[_type] 
     << " Value:"   << left << setw(10) << _value
     << " Line Number:" << _line_num
     << " }";
  return os;
}

/******************************************************
 *  Fills in information about this Token by reading  *
 *    it from specified input stream                  *
 *                                                    *
 *   is  - the stream to read the Token from          *
 *                                                    *
 *   returns: nothing                                 *
 *                                                    *
 *     **** YOU MUST CODE THIS !!!!!! ****            *
 ******************************************************/
void Token::get(istream &is)
{
  // you must write this code !!!!

  //DFA table does not exists yet ...
  if (!DFA)
  {
    DFA = new int*[160];                         ////Unsure on DFA table size... 16 final states 
    
    //initalize table to all ERRORS 
    for (int state=0; state <=16; state++) //row
    {
      DFA[state] = new int[256];                ////Unsure on array size s

      for (int ch=0; ch<256; ch++) // col
        DFA[state][ch] = ERROR; 
    }
    

    // all transitions from start state (DO NOT KNOW THE ENUM)
    DFA[0/*Need to replace*/][ (int) ',' ] = COMMA;
    DFA[0/*Need to replace*/][ (int) ';' ] = SEMICOLON;
    DFA[0/*Need to replace*/][ (int) ']' ] = RBRACK;
    DFA[0/*Need to replace*/][ (int) '[' ] = LBRACK;
    DFA[0/*Need to replace*/][ (int) '(' ] = LPAREN;
    DFA[0/*Need to replace*/][ (int) ')' ] = RBRACK;


    DFA[0/*Need to replace*/][ (int) '=' ] = ASSIGNOP;
                                                        DFA[ASSIGNOP][ (int) '=' ] = RELOP;

    //unsure
    for (char ch='0'; ch <= '9'; ch++)
      DFA[0/*Need to replace*/][ (int) ch ] = NUM_INT;

                                                        DFA[NUM_INT][ (int) '.' ] = NUM_REAL;
                                                                                                for (char ch='0'; ch<='9'; ch++)
                                                                                                  DFA[NUM_REAL][ (int) ch ] = NUM_INT;


    //unsure
    DFA[0/*Need to replace*/][ (int) '<' ] = RELOP;
                                                        DFA[RELOP][ (int) '=' ] = RELOP;


    //unsure
    DFA[0/*Need to replace*/][ (int) '>' ] = RELOP;
                                                        DFA[RELOP][ (int) '=' ] = RELOP;

    //unsure
    DFA[0/*Need to replace*/][ (int) '*' ] = MULOP;
    DFA[0/*Need to replace*/][ (int) '/' ] = MULOP;
    DFA[0/*Need to replace*/][ (int) '+' ] = ADDOP;
    DFA[0/*Need to replace*/][ (int) '-' ] = ADDOP;


    DFA[0/*Need to replace*/][ (int) '|' ] = OR;
    DFA[0/*Need to replace*/][ (int) '&' ] = AND;

  } //finish initializing table
  

  _value = ""; // hold value read from input file
  char ch;

  // ** should probably think about skipping white space here **

  //keep track of the state
  int currState = 0; /*PLACE HOLDER*/          // ** still do not know the number for start state
  int prevState = ERROR;  // at start, no previous state

  //the algorithm
  while(currState!=ERROR)
  {
    ch = is.get(); // get next char from input

    // move to next state based on character read
    prevState = currState;
    currState = DFA[currState][(int) ch];

    if (currState!=ERROR) // if character is a valid part of token ...
      _value+=ch; // ... add char to lexem's value

  }
  
  // hopped out because next 
  _type = TokenType(prevState);       // not sure if I can type cast this 

  // we read an extra character ... put it back for the next read()
  if (is) // insure we are not at the end of the line
    is.putback(ch);
}
