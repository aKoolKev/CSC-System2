#include "Token.hpp"

using namespace std;

//static means this variable has scope to this file only
static int **DFA=nullptr; //nullptr is new c++ version of NULL

istream&
Token::read(istream &is)
{
  // add code here to read in next token ...
  if (!DFA) // DFA table does not exists yet ...
  {
    //set size of the DFA table
    DFA = new int*[DIVIDE+1]; //One more the total amount of states
    
    for(int state=NONE/* value of 0*/; state<=DIVIDE /* value of 7*/; state++) // loops through each rows (states)
    {
      DFA[state] = new int[256]; //each index contains an array that holds all possible numbers 1 byte = 8 bits = 2^8 = 256
      for(int ch=0; ch<256; ch++)
        DFA[state][ch] = ERROR;
    } 


    // all transitions from start state (NONE or 0)
    for (char ch = '0'; ch <='9'; ch++)
      DFA[NONE][ (int) ch ] = INTEGER;


    DFA[NONE][(int)'('] = LPAREN;
    DFA[NONE][(int)')'] = RPAREN;
    DFA[NONE][(int)'+'] = PLUS;
    DFA[NONE][(int)'-'] = MINUS;
    DFA[NONE][(int)'*'] = MULTIPLY;
    DFA[NONE][(int)'/'] = DIVIDE;

    // all transitions from integer (INTEGER or 1)
    for (char ch = '0'; ch<='9'; ch++)
      DFA[INTEGER][ (int) ch ] = INTEGER;
    
  }

  //goal fill in _vale from input file
  //  and _type as token type
  _value="";
  char ch;

  // should probably think about skipping white space here ///

  //keep track of the state
  int curr=NONE;
  int prev=ERROR; //at start, no previous state

  
  while(curr!=ERROR)
  {
    ch = is.get(); // get next char from input

    //move next state based on character read
    prev = curr;
    curr = DFA[curr][(int) ch];

    if (curr!=ERROR) // if character is a valid part of token ...
    {
      _value+=ch; // ... add char to lexeme's value
    }
  }

  _type = prev; //hopped out because of error... prev held the correct state

  //we read one extra character ... put it back for the next read()
  if(is)
    is.putback(ch); 

  return is;
}

const string 
Token::stype() const
{
  switch(_type)
  {
    case INTEGER:
      return "INTEGER";
    case LPAREN:
      return "LPAREN";
    case RPAREN:
      return "RPAREN";
    case PLUS:
      return "PLUS";
    case MINUS:
      return "MINUS";
    case MULTIPLY:
      return "MULTIPLY"; 
    case DIVIDE:
      return "DIVIDE"; 
    default: 
      return "ERROR";
  }
}
ostream& Token::print(std::ostream &os) const
{
  os<< _value; 
  return os;
}

ostream& operator<<(ostream &os, const Token &t)
{
  return t.print(os);
}

istream& operator>>(istream &is, Token &t)
{
  return t.read(is);
}
