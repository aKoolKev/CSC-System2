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

#define ERROR -1
#define START 0
#define ALPHA 1
#define NUM_INT 2
#define NUM_REAL 4
#define ADDOP 5
#define MULOP 6

//questionable
#define RELOP 7
#define RELOP 8

#define ASSIGNOP 9
#define LPAREN 10
#define RPAREN 11
#define AND 13
#define OR 15
#define SEMICOLON 16
#define LBRACK 17
#define RBRACK 18
#define COMMA 19

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
    DFA = new int*[19+1];                         ////Unsure on DFA table size... 19 states plus one for start
    
    //initalize table to all ERRORS 
    for (int state=0; state <=16; state++) //row
    {
      DFA[state] = new int[256];                ////Unsure on array size s

      for (int ch=0; ch<256; ch++) // col
        DFA[state][ch] = ERROR; 
    }
    

    // all transitions from start state (DO NOT KNOW THE ENUM)
    DFA[0/*Need to replace*/][ (int) ',' ] = COMMA;         //START -> ,
    DFA[0/*Need to replace*/][ (int) ';' ] = SEMICOLON;     //START -> ;
    DFA[0/*Need to replace*/][ (int) ']' ] = RBRACK;        //START -> ]
    DFA[0/*Need to replace*/][ (int) '[' ] = LBRACK;        //START -> [
    DFA[0/*Need to replace*/][ (int) '(' ] = LPAREN;        //START -> (
    DFA[0/*Need to replace*/][ (int) ')' ] = RBRACK;        //START -> )

    //START -> =
    DFA[0/*Need to replace*/][ (int) '=' ] = ASSIGNOP;
                                                        // ==
                                                        DFA[ASSIGNOP][ (int) '=' ] = RELOP;

    //unsure: START to DIGIT
    for (char ch='0'; ch <= '9'; ch++)
      DFA[0/*Need to replace*/][ (int) ch ] = NUM_INT;  // DIGIT to DIGIT
                                                        for (char ch='0'; ch<='9'; ch++)
                                                          DFA[NUM_INT][ (int) ch] = NUM_INT;
                                                                                              // DIGIT to FLOAT
                                                                                              DFA[NUM_INT][ (int) '.' ] = NUM_REAL; 
                                                                                                                                      // FLOAT to DIGIT
                                                                                                                                      for (char ch='0'; ch<='9'; ch++)
                                                                                                                                        DFA[NUM_REAL][ (int) ch ] = NUM_INT;


    //unsure: START to ALPHA (lowercase)
    for (char ch='a'; ch <= 'z'; ch++)
        DFA[0/*Need to replace*/][ (int) ch ] = ID;     // at ALPHA to ALPHA
                                                        for (char ch='a'; ch <= 'z'; ch++) 
                                                          DFA[ID][ (int) ch ] = ID;

                                                        // at ALPHA to DIGIT
                                                        for(char ch='0'; ch<='9'; ch++) 
                                                          DFA[ID][ (int) ch] = NUM_INT;


    //unsure: START to ALPHA (Uppercase)
    for (char ch='A'; ch <= 'Z'; ch++)
        DFA[0/*Need to replace*/][ (int) ch ] = ID;     // at ALPHA to ALPHA
                                                        for (char ch='A'; ch <= 'Z'; ch++) 
                                                          DFA[ID][ (int) ch ] = ID;

                                                        // at ALPHA to DIGIT
                                                        for(char ch='0'; ch<='9'; ch++) 
                                                          DFA[ID][ (int) ch] = NUM_INT;


    //unsure: START -> '<'
    DFA[0/*Need to replace*/][ (int) '<' ] = RELOP;     // <=
                                                        DFA[RELOP /* '<' */][ (int) '=' ] = RELOP;


    //unsure: START -> '>'  
    DFA[0/*Need to replace*/][ (int) '>' ] = RELOP;     // >=
                                                        DFA[RELOP /* '>' */][ (int) '=' ] = RELOP;

    //unsure
    DFA[0/*Need to replace*/][ (int) '*' ] = MULOP;     //START -> *
    DFA[0/*Need to replace*/][ (int) '/' ] = MULOP;     //START -> /
    DFA[0/*Need to replace*/][ (int) '+' ] = ADDOP;     //START -> +
    DFA[0/*Need to replace*/][ (int) '-' ] = ADDOP;     //START -> -

    //UNSURE!
    DFA[0/*Need to replace*/][ (int) '|' /*'||'*/ ] = OR;        //START -> ||
    DFA[0/*Need to replace*/][ (int) '&' /*'&&'*/ ] = AND;       //START -> &&

  } //finish initializing table
  

  _value = ""; // hold value read from input file
  char ch;

  // ** should probably think about skipping white space here **
  ch = is.get();// get next char from input
  while (isspace(ch)) // || is isComment() ...
  {
    if (ch=='\n') // char read was a new line
      _line_num++;

    ch = is.get();
  }
  if (!is)
    return is; //NEED TO CHANGE

  is.putback(ch); 


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
