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




// helper function that indicate if a provided character is a '#'
bool isComment(char ch)
{
  bool val = false;

  if (ch == '#')
    val = true;

  return val;
}

// helper function that returns the Token Type of a given final state number (based on graph)
TokenType getTokenTypeByNum (int finalStateNum)
{
  switch (finalStateNum)
  {
    case 1: return ID;
    case 2: return NUM_INT;
    case 4: return NUM_REAL;
    case 5: return ADDOP;
    case 6: return MULOP;
    case 7: return RELOP;
    case 8: return RELOP;
    case 9: return ASSIGNOP;
    case 10: return LPAREN;
    case 11: return RPAREN;
    case 13: return AND;
    case 15: return OR;
    case 16: return SEMICOLON;
    case 17: return LBRACK;
    case 18: return RBRACK;
    case 19: return COMMA;
  }

  // DEBUG
  cout << "ERROR: " << finalStateNum << " is invalid!\n\n"; 

  // sole purpose is to get read of warning for not returning something in the function
  // (will not reach here...should not)
  return (TokenType) finalStateNum;
}

TokenType getReservedTokenType(int index)
{
  
  

  // if ( val == reserved[0]) return INTEGER;
  // else if (val == reserved[1]) return FLOAT;
  // else if (val == reserved[2]) return WHILE;
  // else if (val == reserved[3]) return IF;
  // else if (val == reserved[4]) return THEN;
  // else if (val == reserved[5]) return ELSE;
  // else if (val == reserved[6]) return VOID;
  // else if (val == reserved[7]) return BEGIN;
  // else if (val == reserved[8]) return END;

  switch (index)
  {
    case 0: return INTEGER;
    case 1: return FLOAT;
    case 2: return WHILE;
    case 3: return IF;
    case 4: return THEN;
    case 5: return ELSE;
    case 6: return VOID;
    case 7: return BEGIN;
    case 8: return END;
  }

  //DEBUG
  cout << "ERROR: Could not find a match!\n\n";
}


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
    DFA = new int*[19+1]; // 19 states + the start state
    
    // setting up DFA table
    for (int state=0; state <=20; state++) //row
    {
      DFA[state] = new int[256]; // new column at each row (state) that contains all the possible characters

      //initalize each column to ERROR
      for (int ch=0; ch<256; ch++)
        DFA[state][ch] = -1; // -1 = ERROR
    }
    
    // Mapping out graph 

    DFA[0][ (int) ',' ] = 19;   // START (0) -> , (19)
    DFA[0][ (int) ';' ] = 16;   // START (0) -> ; (16)
    DFA[0][ (int) ']' ] = 18;   // START (0) -> ] (18)
    DFA[0][ (int) '[' ] = 17;   // START (0) -> [ (17)
    DFA[0][ (int) '(' ] = 10;   // START (0) -> ( (10)
    DFA[0][ (int) ')' ] = 11;   // START (0) -> ) (11)


    DFA[0][ (int) '*' ] = 6;     // START (0) -> * (6)
    DFA[0][ (int) '/' ] = 6;     // START (0) -> / (6)
    DFA[0][ (int) '+' ] = 5;     // START (0) -> + (5)
    DFA[0][ (int) '-' ] = 5;     // START (0) -> - (5)

  
    DFA[0][ (int) '=' ] = 9;    // START (0) -> = (9)
      DFA[9][ (int) '=' ] = 8;    // = (9) -> = (8)


    DFA[0][ (int) '<' ] = 7;    // START (0) -> < (7)
      DFA[7][ (int) '=' ] = 8;    // < (7) -> = (8)

    DFA[0][ (int) '>' ] = 7;    // START (0) -> > (7)
      DFA[7] [ (int) '=' ] = 8;   // > (7) -> = (8)


    DFA[0][ (int) '|' ] = 14;   // START (0) -> | (14)
      DFA[14][ (int) '|' ] = 15;    // | (14) -> | (15)
    
    DFA[0][ (int) '&' ] = 12;   // START (0) -> & (12)
      DFA[12] [ (int) '&' ] = 13;    //START (0) -> & (13)


    // could optimize...
    for (char ch='0'; ch<='9'; ch++)
      DFA[0][ (int) ch ] = 2;   // START (0) -> 0,1,...,9 denoted are DIGIT = 2

      for (char ch='0'; ch<='9'; ch++)
        DFA[2][ (int) ch ] = 2;   // DIGIT (2) -> DIGIT (2) 

      for (char ch='0'; ch<='9'; ch++)
        DFA[2] [ (int) '.' ] = 3;   // DIGIT (2) -> . (3)

          for (char ch='0'; ch<='9'; ch++)
            DFA[3][ (int) ch ] = 4;   // . (3) -> DIGIT (4)

            for (char ch='0'; ch<='9'; ch++)
              DFA[4][ (int) ch ] = 4;   // DIGIT (4) -> DIGIT (4)

                                                                                                                            
    // could optimize... (lowercase letters)
    for (char ch='a'; ch<='z'; ch++)
      DFA[0][ (int) ch ] = 1;   // START (0) -> ALPHA (1)
    
      for (char ch='a'; ch<='z'; ch++)
        DFA[1][ (int) ch ] = 1;   // ALPHA (1) -> ALPHA (1)
    
      for (char digit='0'; digit<='9'; digit++)
        DFA[1][ (int) digit ] = 1;  //ALPHA (1) -> DIGIT (1)


    // could optimize... (uppercase letters)
    for (char ch='A'; ch<='Z'; ch++)
      DFA[0][ (int) ch ] = 1;   // START (0) -> ALPHA (1)
    
      for (char ch='A'; ch<='Z'; ch++)
        DFA[1][ (int) ch ] = 1;   // ALPHA (1) -> ALPHA (1)
    
      for (char digit='0'; digit<='9'; digit++)
        DFA[1][ (int) digit ] = 1;  //ALPHA (1) -> DIGIT (1)

  } //finish initializing table
  

  // keep track of the state
  int currState = 0; /*PLACE HOLDER*/          // ** still do not know the number for start state
  int prevState = -1;  // at start, no previous state


  _value = ""; // hold value read from input file
  char ch;

  // ** should probably think about skipping white space here **
  ch = is.get();// get next char from input

  //DEBUG
  // cout << "ch: " << ch << ".\n\n";
  
  //_line_num is initalized to zero, but file number starts at 1
  if (_line_num == 0)
    _line_num++;

  //NEED TO HANDLE IF IT IS A COMMENT #
  while (isspace(ch) || isComment(ch)) // || is isComment() ...
  {
    //DEBUG
    //cout << "Token.cpp: isspace    ch: " << ch << ".\n\n";

    if (ch=='\n') // char read was a new line
      _line_num++;


    //check if it is a comment
    if (isComment(ch))
    {
      _line_num++;
      // cout << "Encountered a comment!\n\n";
      string commentStmt;
      getline(is, commentStmt); // "skip the values of the comment"
      // cout << "commentStmt: " << commentStmt << endl;
    }

    ch = is.get();
    // cout << "ch: " << ch << "\n\n";

  }

  if (!is)
  {
    _type = EOF_TOK;    
    return;                //NEED TO FIX
  }

  is.putback(ch); 
  //cout << "Token.cpp: isspace putback    ch: " << ch << "\n\n";


  //the algorithm
  while(currState!=-1) // -1 denotes ERROR
  {
    ch = is.get(); // get next char from input

    //DEBUG
    //cout << "Token.cpp: while()    ch: " << ch << "\n\n";

    // move to next state based on character read
    prevState = currState;
    currState = DFA[currState][(int) ch];

    if (currState!=-1) // if character is a valid part of token ...
      _value+=ch; // ... add char to lexem's value


    // need to conssider end of file token...

  }
  
  // hopped out because we have encountered a valid state (ERROR)

  
  _type = getTokenTypeByNum(prevState); 

  // make sure ID is not a reserved word
  if (_type == getTokenTypeByNum(1) /* 1=ID */)
  {
    for (int i=0; i<9; i++) // check if it is a reserved keyword
    {
      if (_value == reserved[i]) // it is a reserved keyword
        _type = getReservedTokenType(i); // change the type to the reserve token type
    }
  }

  // we read an extra character ... put it back for the next read()
  if (is) // insure we are not at the end of the line
    is.putback(ch);
}
