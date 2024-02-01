#include "Grammar.hpp"
#include "Token.hpp"

using namespace std;

static string SourceFile;

void printSourceFile()
{
    cout << "[\n" << SourceFile << endl << "]\n\n";
}


/*  
    program -> declaration program | compound
*/
bool program(Token tok, std::ifstream &ifile)
{
    cout << "try    declaration...\n";
    int pos = ifile.tellg();

    bool declaration_val = declaration(tok, ifile);

    if (declaration_val)
    {
        cout << "declaration Y\n";
        printSourceFile(); //debug

        cout << "try    program...\n";
        bool program_val = program(tok, ifile);

        if(!program_val)
        {
            cout << "program N!\n";
            return false;
        }
        else
            cout << "program Y!\n";
            return true;
    }
    else 
    {
        //declaration was unsuccessful, undo the move
        ifile.seekg(pos);

        cout << "try compound...";
        bool compound_val = compound(tok, ifile);
        if(compound_val)
        {
            cout << "compound Y\n";
            return true;
        }
        else 
            cout << "compound N\n";
            return false;
    }
    
}


/* 
    declaration -> type idlist SEMICOLON
*/
bool declaration(Token tok, std::ifstream &ifile)
{
    tok.get(ifile);
    cout << "try    type...\n";
    bool type_val = type(tok);

    if (type_val)
    {
        cout << "try Y\n";

        cout << "try idlist...\n";
        bool idlist_val = idlist(tok,ifile);

        if (idlist_val)
        {
            printSourceFile();

            //read token
            //Token tok;
            tok.get(ifile);
            cout << "Read: " << tok.value() << endl;

            if(tok.type() == SEMICOLON)
            {
                //encountered SEMICOLON
                cout << "adding " << tok.value() << endl;
                SourceFile += tok.value() + "\n"; //add ';'
            
                return true;
            }
            else
            {
                return false;
            }
        }
        else
            return false;

    }
    else
        return false;

   

}


/* 
    idlist -> ID | ID COMMA idlist
*/
bool idlist(Token tok, std::ifstream &ifile)
{ //a,b,c;

    //read token
    tok.get(ifile);
    cout << "idlist read: " << tok.value() << endl;

    if (tok.type() == ID)
    {
        SourceFile += " " + tok.value(); //add the ID

        //remember index in input stream
        int pos = ifile.tellg();

        //read next token...
        tok.get(ifile);

        if(tok.type() == COMMA)
        {
            SourceFile += tok.value(); //add ','

            bool idlist_val = idlist(tok,ifile);

            //UNSURE RETURN VALUE!!!
            return idlist_val;
        }
        else
        {
            //unget token!!
            cout << "Unget token!\n";
            ifile.seekg(pos);

            //Just ID
            return true;
        }


    }
    else //not an ID ... don't really need
        return false;

}


/* 
    type -> INTEGER | FLOAT | VOID
*/
bool type(Token tok)
{
    cout << "read from type: " << tok << endl;

    if (tok.type() == INTEGER || tok.type() == FLOAT || tok.type() == VOID)
    {
        //debug
        // cout << "type_val: " << tok.value() << endl;
        cout << "adding: " << tok.value() << endl;
        SourceFile += tok.value();

        //return tok.value();
        return true;
    }
    else
    {
        cout << "ERROR! EXPECTED {INTEGER, FLOAT, VOID}, got: " << tok << endl;
        return false;
    }
}

/* 
    compound -> BEGIN stmtlist END
*/
bool compound(Token tok, std::ifstream &ifile)
{
    //Token tok;
    //read in token
    tok.get(ifile);

    if (tok.type() == BEGIN)
    {
        SourceFile += "\n\n" + tok.value() + "\n";

        bool stmtlist_val = stmtlist(ifile, tok);

        //read in next token...
        tok.get(ifile);

        if (tok.type() != END)
        {
            cout << "ERROR! Expected: \"END,\" got: " << tok.value() << endl;
            return false;
        }
        
        //got END

        //UNSURE RETURN VAL
        return stmtlist_val;
    }
    else
        return false;
}

/* 
    stmtlist -> stmt | stmt SEMICOLON stmtlist
*/
bool stmtlist(std::ifstream &ifile, Token tok)
{

    bool stmt_val = stmt(ifile, tok);

    if (stmt_val)
    {
        SourceFile += tok.value() + " ";

        //remember index pos in input stream
        int pos = ifile.tellg();
        //read token...
        tok.get(ifile);

        if (tok.type() == SEMICOLON)
        {
            bool stmtlist_val = stmtlist(ifile, tok);

            //UNSURE RETURN VALUE
            return stmtlist_val;
        }
        else
        {
            //unget token!!
            cout << "unget token!\n";
            ifile.seekg(pos);
            
            return true; // Just    stmt
        }
    }

}


/* 
    stmt -> ID| ID LPAREN exprlist RPAREN |
            ID ASSIGNOP expr |
            IF expr THEN compound ELSE compound |
            WHILE LPAREN expr RPAREN compound |
            compound
*/
bool stmt(std::ifstream &ifile, Token tok)
{
    //might have to undo move if unsucessful
    int pos = ifile.tellg();

    //get a token
    tok.get(ifile);
    
    if(tok.type() == ID)
    {
        SourceFile += tok.value() + " "; // ID

        //see if there is anything else after ID
        tok.get(ifile);
        pos = ifile.tellg();

        if (tok.type() == LPAREN)
        {
            bool exprlist_val = exprlist(ifile, tok);

            pos = ifile.tellg();
            //read next token
            tok.get(ifile);

            //expect RPAREN
            if (tok.type() == RPAREN)
                return true;
            else
            {
            
                //unget the token!
                ifile.seekg(pos);
                return false;
            }

        }

            // return tok.value();
            return true;
            //UNSURE what to return here???
    }
    // ID ASSIGNOP expr 
    else if (tok.type() == ASSIGNOP) 
    {
        tok.get(ifile);
        bool expr_val = expr(ifile, tok);
        //do I return a value here?
        return expr_val;
    
    
        //unget token!
        ifile.seekg(pos);
        
        //UNSURE return value!!!
        // return tok.value();
        return true;
    }
    //IF expr THEN compound ELSE compound |
    else if (tok.type() == IF)
    {
        bool expr_val = expr(ifile, tok);

        //read next token
        tok.get(ifile);

        if (tok.type() == THEN)
        {
            bool compound_val = compound(tok,ifile);
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
            bool expr_val = expr(ifile, tok);

            //read next token
            tok.get(ifile);

            if(tok.type() != RPAREN)
                cerr << "Expected ')', got: " << tok << endl;
            
            //got a RPAREN
            bool compound_val = compound(tok, ifile);

            //UNSURE RETURN VALUE!!!
            return compound_val;
        }

    }
    //compound  ...UNSURE!!!
    else 
    {
        bool compound_val = compound(tok, ifile);
        // UNSURE...return value??
        return compound_val;
    }

}


/* 
    exprlist -> expr | expr COMMA exprlist
*/
bool exprlist(std::ifstream &ifile, Token tok)
{
    bool expr_val = expr (ifile, tok);

    // remember index in the input stream
    int pos = ifile.tellg(); 

    //try to read next token
    tok.get(ifile);

    if(tok.type() == COMMA)
    {
        bool exprlist_val = exprlist(ifile, tok); //UNSURE

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
bool expr(std::ifstream &ifile, Token tok)
{
    bool simpexpr_val = simpexpr(ifile, tok);

    //remember index position in the input file
    int pos = ifile.tellg();

    
    tok.get(ifile);

    if (tok.type() == RELOP)
    {
        simpexpr_val = simpexpr(ifile, tok);        
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
bool simpexpr(std::ifstream &ifile, Token tok)
{
    bool term_val = term(ifile);

    //remember pos
    int pos = ifile.tellg();

    //read next token
    tok.get(ifile);

    if (tok.type() == ADDOP)
    {
        bool simpexpr_val = simpexpr(ifile, tok);

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
bool term(std::ifstream &ifile)
{
    bool factor_val = factor(ifile);

    //remember location
    int pos = ifile.tellg();

    Token mulopTok;
    mulopTok.get(ifile);

    if (mulopTok.type() == MULOP)
    {
        bool term_val = term(ifile);
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
bool factor(std::ifstream &ifile)
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
            bool parenVal = exprlist(ifile, tok); //UNSURE!!!

            //read next token
            tok.get(ifile);

            if (tok.type() != RPAREN)
                cerr << "Expected ')', got: " << tok << endl;
            
            //encountered the closing RPAREN

            return parenVal; //UNSURE!!!
        }
        else //ID
        {
            return true;
            // return val;
        }
    }
    //NUM_REAL || NUM_INT
    else if (tok.type() == NUM_REAL || tok.type() == NUM_INT )
    {
        return true;
        // return tok.value();
    }
    //LPAREN expr RPAREN
    else if (tok.type() == LPAREN)
    {
        bool parenVal = expr(ifile, tok); //UNSURE!!!

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
