#include "Grammar.hpp"
#include "Token.hpp"

using namespace std;

//expr -> term expr2
int expr(std::ifstream &is)
{
    int termVal = term(is);

    int expr2Val = expr2(is, termVal); // does the computation

    return expr2Val;
}

//expr2 -> + term expr2 | - term expr2 | e
int expr2(std::ifstream &is, int incomingValue)
{
    int pos = is.tellg();
    Token pm;
    is >> pm;

    int result = incomingValue; 

    if (pm.type() == PLUS)
    {
        int termVal = term(is);
        result += termVal;
    }
    else if (pm.type() == MINUS)
    {
        int termVal = term(is);
        result -= termVal;
    }
    else // epsilon case
    {
        is.seekg(pos);
        return incomingValue;
    }

    return expr2(is, result);
}

//term -> factor term2
int term(std::ifstream &is)
{
    int termVal = factor(is);

    int term2Val = term2(is, termVal); // does the computation

    return term2Val;
}

// * factor term2 | / factor term2
int term2(std::ifstream &is, int incomingValue)
{
    int pos = is.tellg();
    Token md;
    is >> md;

    int result = incomingValue; 

    if (md.type() == MULTIPLY)
    {
        int factorVal = factor(is);
        result *= factorVal;
    }
    else if (md.type() == DIVIDE)
    {
        int factorVal = factor(is);
        result /= factorVal;
    }
    else // epsilon case
    {
        is.seekg(pos);
        return incomingValue;
    }

    return term2(is, result);
}

//factor INTEGER | (<expr>)
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