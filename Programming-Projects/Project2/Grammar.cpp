#include "Grammar.hpp"
#include "Token.hpp"

using namespace std;


/* 
    program -> declaration program | compound
*/
string program(std::ifstream &ifile)
{
    string declaration_val = declaration(ifile);
    string program_val = program(ifile);
    string compound_val = compound(ifile);
}


/* 
    declaration -> type idlist SEMICOLON
*/
string declaration(std::ifstream &ifile )
{
    string type_val = type(ifile);
    string idlist_val = idlist(ifile);

    //read token
    Token tok;
    tok.get(ifile);

    if(tok.type() != SEMICOLON)
        cout << "ERROR! Expected: SEMICOLON, got: " << tok << endl;
    
    //encountered SEMICOLON
    
    //WHAT TO RETURN??
    return type_val + idlist_val;
}


/* 
    idlist -> ID | ID COMMA idlist
*/
std::string idlist(std::ifstream &ifile)
{
    Token tok; 
    //read token
    tok.get(ifile);

    if (tok.type() == ID)
    {
        //remember index in input stream
        int pos = ifile.tellg();

        //read next token...
        tok.get(ifile);

        if(tok.type() == COMMA)
        {
            string idlist_val = idlist(ifile);

            //UNSURE RETURN VALUE!!!
            return idlist_val;
        }
        else // just ID
        {
            //unget token!!
            ifile.seekg(pos);

            //UNSURE RETURN VALUE!
            return tok.value();
        }

    }
}


/* 
    type -> INTEGER | FLOAT | VOID
*/
std::string type(std::ifstream &ifile)
{
    //read a token
    Token tok;
    tok.get(ifile);

    if (tok.type() == INTEGER || tok.type() == FLOAT || tok.type() == VOID)
    {
        return tok.value();
    }
    else
    {
        cout << "ERROR! EXPECTED {INTEGER, FLOAT, VOID}, got: " << tok << endl;
    }
}

/* 
    compound -> BEGIN stmtlist END
*/
string compound(std::ifstream &ifile)
{
    Token tok;
    //read in token
    tok.get(ifile);

    if (tok.type() == BEGIN)
    {
        string stmtlist_val = stmtlist(ifile);

        //read in next token...
        tok.get(ifile);

        if (tok.type() != END)
            cout << "ERROR! Expected: \"END,\" got: " << tok.value() << endl;
        
        //got END

        //UNSURE RETURN VAL
        return stmtlist_val;
    }
}

/* 
    stmtlist -> stmt | stmt SEMICOLON stmtlist
*/
std::string stmtlist(std::ifstream &ifile)
{
    string stmt_val = stmt(ifile);

    //remember index pos in input stream
    int pos = ifile.tellg();

    //read token...
    Token tok;
    tok.get(ifile);

    if (tok.type() == SEMICOLON)
    {
        string stmtlist_val = stmtlist(ifile);

        //UNSURE RETURN VALUE
        return stmtlist_val;
    }
    else
    {
        //unget token!!
        ifile.seekg(pos);

        //UNSURE return value!!
        return stmt_val;
    }
}


/* 
    stmt -> ID| ID LPAREN exprlist RPAREN |
            ID ASSIGNOP expr |
            IF expr THEN compound ELSE compound |
            WHILE LPAREN expr RPAREN compound |
            compound
*/
std::string stmt(std::ifstream &ifile)
{

    int pos = ifile.tellg();

    //get a token
    Token tok;
    tok.get(ifile);
    
    if(tok.type() == ID)
    {
        //see if there is anything else after ID
        tok.get(ifile);
        pos = ifile.tellg();

        if (tok.type() == LPAREN)
        {
            string exprlist_val = exprlist(ifile);

            pos = ifile.tellg();
            //read next token
            tok.get(ifile);

            //expect RPAREN
            if (tok.type() != RPAREN)
            {
                cout << "Expected ')', got : " << tok << endl;

                //unget the token!
                ifile.seekg(pos);
            }

            return tok.value();
            //UNSURE what to return here???
        }
        // ID ASSIGNOP expr 
        else if (tok.type() == ASSIGNOP) 
        {
            tok.get(ifile);
            string expr_val = expr(ifile);
            //do I return a value here?
            return expr_val;
        }
        else // just an ID
        {
            //unget token!
            ifile.seekg(pos);
            
            //UNSURE return value!!!
            return tok.value();
        }
    }
    //IF expr THEN compound ELSE compound |
    else if (tok.type() == IF)
    {
        string expr_val = expr(ifile);

        //read next token
        tok.get(ifile);

        if (tok.type() == THEN)
        {
            string compound_val = compound(ifile);
        }
        else //did not get THEN
        {
            cout << "ERROR! Expected \" THEN \" , got: " << tok;
        }
       

    }
    //WHILE LPAREN expr RPAREN compound |
    else if (tok.type() == WHILE)
    {
        //read next token
        tok.get(ifile);

        if (tok.type() == LPAREN)
        {
            string expr_val = expr(ifile);

            //read next token
            tok.get(ifile);

            if(tok.type() != RPAREN)
                cerr << "Expected ')', got: " << tok << endl;
            
            //got a RPAREN
            string compound_val = compound(ifile);

            //UNSURE RETURN VALUE!!!
            return compound_val;
        }

    }
    //compound  ...UNSURE!!!
    else 
    {
        string compound_val = compound(ifile);
        // UNSURE...return value??
        return compound_val;
    }

}


/* 
    exprlist -> expr | expr COMMA exprlist
*/
std::string exprlist(std::ifstream &ifile)
{
    string expr_val = expr (ifile);

    // remember index in the input stream
    int pos = ifile.tellg(); 

    //try to read next token
    Token commaTok;
    commaTok.get(ifile);

    if(commaTok.type() == COMMA)
    {
        string exprlist_val = exprlist(ifile); //UNSURE

        //UNSURE RETURN VALUE
        return exprlist_val;
    }
    else // just expr
    {
        //unget the token at this position !!
        ifile.seekg(pos);
        
        //UNSURE what to return or if I need to return
        return expr_val;
    }

}


/* 
    expr -> simpexpr | simpexpr RELOP simpexpr
*/
std::string expr(std::ifstream &ifile)
{
    string simpexpr_val = simpexpr(ifile);

    //remember index position in the input file
    int pos = ifile.tellg();

    Token relopTok;
    relopTok.get(ifile);

    if (relopTok.type() == RELOP)
    {
        simpexpr_val = simpexpr(ifile);        
    }
    else // unget 
    {
        ifile.seekg(pos);
        //unsure should I return a value?
    }

}

/* 
    simpexpr -> term | term ADDOP simpexpr
*/
std::string simpexpr(std::ifstream &ifile)
{
    string term_val = term(ifile);

    //remember pos
    int pos = ifile.tellg();

    //read next token
    Token addopTok;
    addopTok.get(ifile);

    if (addopTok.type() == ADDOP)
    {
        string simpexpr_val = simpexpr(ifile);

        //UNSURE RETURN VALUE
        return simpexpr_val;
    }
    else //just     term
    {
        //undo get!!
        ifile.seekg(pos);

        return term_val;
    }
}

/* 
    term -> factor | factor MULOP term
*/
std::string term(std::ifstream &ifile)
{
    string factor_val = factor(ifile);

    //remember location
    int pos = ifile.tellg();

    Token mulopTok;
    mulopTok.get(ifile);

    if (mulopTok.type() == MULOP)
    {
        string term_val = term(ifile);
    }
    else //unget token!
    {
        ifile.seekg(pos);
        //unsure what to return
    }

    
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
