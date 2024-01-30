#include "Grammar.hpp"
#include "Token.hpp"

using namespace std;


/* 
    exprlist -> expr | expr COMMA exprlist
*/
std::string exprlist(std::ifstream &)
{

}


/* 
    expr -> simpexpr | simpexpr RELOP simpexpr
*/
std::string expr(std::ifstream &)
{

}


/* 
    factor -> ID | ID LPAREN exprlist RPAREN | 
            NUM_REAL | NUM_INT
            LPAREN expr RPAREN
*/
string factor(std::ifstream &ifile)
{
    //try to read a token for input file
    Token tok;
    tok.get(ifile);

    if (tok.type() == ID) //ID
    {
        //remember the value
        string val = tok.value();
        
        //read next token
        tok.get(ifile); 
        

        if (tok.type() == LPAREN) //ID LPAREN exprlist RPAREN
        {
            string parenVal = exprlist(ifile); //UNSURE!!!

            //read next token
            tok.get(ifile);

            if (tok.type() != RPAREN)
                cerr << "Expected ')', got: " << tok << endl;
            
            //encountered the closing RPAREN

            return parenVal; //UNSURE!!!
        }
        else //ID
        {
            return val;
        }
    }
    //NUM_REAL || NUM_INT
    else if (tok.type() == NUM_REAL || tok.type() == NUM_INT )
    {
        return tok.value();
    }
    //LPAREN expr RPAREN
    else if (tok.type() == LPAREN)
    {
        string parenVal = expr(ifile); //UNSURE!!!

        //read next token
        tok.get(ifile);

        if (tok.type() != RPAREN)
            cerr << "Expected ')', got: " << tok << endl;
            
        //encountered the closing RPAREN

        return parenVal; //UNSURE!!!
    }
    else // invalid token to start a    factor
    {
        cerr << "line: " << tok.lineNumber() << " Unexpected token: " << tok << endl;
    }
}

