#include "Grammar.hpp"
#include "Token.hpp"

using namespace std;


int expr(std::ifstream &is)
{
    int termVal = term(is);

    int expr2Val = expr2(is, termVal); // does the computation

    return expr2Val;
}

int expr2(std::ifstream &is, int incomingValue)
{
    int pos = is.tellg();
    Token pm;
    is >> pm;

    int result = incomingValue; 

    int termVal = term(is);

    if (pm.type() == PLUS)
    {
        result += termVal;
    }
    else if (pm.type() == MINUS)
    {
        result -= termVal;
    }
    else // epsilon case
    {
        is.seekg(pos);
        return incomingValue;
    }

    return expr2(is, result);
}

int term(std::ifstream &is)
{
    int termVal = factor(is);

    int term2Val = term2(is, termVal); // does the computation

    return term2Val;
}

int term2(std::ifstream &is, int incomingValue)
{
    int pos = is.tellg();
    Token md;
    is >> md;

    int result = incomingValue; 

    int termVal = term(is);

    if (md.type() == MULTIPLY)
    {
        result *= termVal;
    }
    else if (md.type() == DIVIDE)
    {
        result /= termVal;
    }
    else // epsilon case
    {
        is.seekg(pos);
        return incomingValue;
    }

    return term2(is, result);
}

//factor number | (expr)
int factor(std::ifstream &is)
{
    Token tok;
    is >> tok;
    
    if(tok.type()==INTEGER)
    {
        return atoi(tok.value().c_str()); // INTEGER is a string
    } 
    else if (tok.type()==LPAREN)
    {
        int pval = expr(is);

        is >> tok;
        
        if (tok.type()!=RPAREN)
            cerr << "Expected ')', got: " << tok << endl;

        return pval;
    }
}