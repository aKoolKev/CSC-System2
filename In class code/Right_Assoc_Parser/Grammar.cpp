#include "Grammar.hpp"
#include "Token.hpp"

using namespace std;

//expr -> term + expr | term - expr | term
int expr(std::ifstream &is)
{
    int lhs = term(is); // all three grammar rule options start with "term"

    int pos = is.tellg(); // number of characters into the stream

    Token pm; // plus or minus
    is >> pm;

    if (pm.type() == PLUS) // +
    {
        return lhs + expr(is);
    }
    else if (pm.type() == MINUS) // -
    {
        return lhs - expr(is);
    }
    else // we shouldn't have read the token !!!
    {
        //unget the token !!!
        is.seekg(pos); // go to this position

        return lhs;
    }
}

//term -> factor * term | factor/term | factor
int term(std::ifstream &is)
{

    int lhs = factor(is); // all three grammar rule starts with factor

    int pos = is.tellg(); // remember the index in the input stream

    Token md; // multiply or divide
    is >> md; // read in the sign

    if (md.type() == MULTIPLY) // *
    {
        return lhs * term(is);
    }
    else if (md.type() == DIVIDE) //   (/)
    {
        return lhs / term(is);
    }
    else // we shouldn't have read the token !!!
    {
        //unget the token !!!
        is.seekg(pos); // go to this position

        return lhs;
    }
    

}

//factor -> NUMBER | (expr)
int factor(std::ifstream &is)
{
    /*
        Just read in the token: 
        no need to type cast because we do not know
        if it will be a number of an expr(ession)
    */
    Token tok;
    is >> tok;
    
    // number 
    if(tok.type()==INTEGER)
    {
        return atoi(tok.value().c_str()); // INTEGER is a string
    }

    // |

    // (expr)
    else if (tok.type()==LPAREN) // (
    {
        int pval = expr(is); // hold the value inside of the parenthesis

        is >> tok;
        
        if (tok.type()!=RPAREN) // stop when we ecounter the closing parenthesis ')'
            cerr << "Expected ')', got: " << tok << endl;

        //finished evaluating the value of the parenthesis
        return pval;
    }
    else // invalid token to start a factor
    {
        cerr << "Unexpected token: " << tok << endl;
        return 0;
    }
}