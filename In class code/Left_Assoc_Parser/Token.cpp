#include "Token.hpp"

using namespace std;

static int **DFA=nullptr;
static int lineNumber=1;

istream&
Token::read(istream &is)
{
  if (!DFA) // dfa table does not exist yet ...
    {
      DFA = new int*[DIVIDE+1];
      for(int state=NONE; state<=DIVIDE; state++)
	{
	  DFA[state] = new int[256];
	  for (int ch=0; ch<256; ch++)
	    DFA[state][ch] = ERROR;
	}

      // all transitions from start state (NONE or 0)
      for (char ch = '0'; ch<='9'; ch++)
	DFA[NONE][ (int) ch ] = INTEGER;
      
      DFA[NONE][ (int) '(' ] = LPAREN;
      DFA[NONE][ (int) ')' ] = RPAREN;
      DFA[NONE][ (int) '+' ] = PLUS;
      DFA[NONE][ (int) '-' ] = MINUS;
      DFA[NONE][ (int) '*' ] = MULTIPLY;
      DFA[NONE][ (int) '/' ] = DIVIDE;

      // all transitions from integer state (INETGER or 1)
      for (char ch = '0'; ch<='9'; ch++)
	DFA[INTEGER][ (int) ch ] = INTEGER;
    }

  // goal: fill in _value from input file
  //        and _type as token type
  _value="";
  char ch;

  // should probably think about skiping white space here ///
/* neat, quick trick to accomplish task in a dirty manner
  is >> ch;
  if (!is)
    return is;
  is.putback(ch);
*/
  ch = is.get(); // get next char from input
  while (isspace(ch) ) //   || isComment() ...
  {
    if (ch=='\n')
      lineNumber++;
    ch=is.get();
  }
  if (!is)
    return is;
  is.putback(ch);
  



  int curr=NONE;
  int prev=ERROR;

  while(curr!=ERROR)
    {
      ch = is.get(); // get next char from input

      // move to next state based on character read
      prev = curr;
      curr = DFA[curr][(int) ch];

      if (curr!=ERROR) // if caharacter is a valid aprt of token ...
	{
	  _value+=ch;  //   ... add char to lexeme's value
	}
    }

  _type = prev;

  // we read one extra character ... put it back for the next read()
  if (is)
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
