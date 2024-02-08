//File: Grammar.cpp
//Author: Kevin Dong
//Date: 1/29/24
//Purpose: Define each of the grammars


#include "Grammar.hpp"
#include "Token.hpp"

using namespace std;

// store the beautified code
static string SourceFile; 

// indentation counter
int tabAmount = 0; 

// function that prints the "beautified" code
void printSourceFile()
{
    cout << SourceFile << endl << "\n\n";
}

// function that inserts x numbers of "tabs" into SourceFile, the answer 
void insertTab()
{
    for (int i=0; i < tabAmount; i++)
        SourceFile += "  "; // tab is just two space characters
}


/*  
    program -> declaration program | compound
*/
bool program(Token tok, std::ifstream &ifile)
{
    //read first token...
    int pos = ifile.tellg();
    tok.get(ifile); 

    bool declaration_val = declaration(tok, ifile);

    if (declaration_val) // -> declaration 
    {
        bool program_val = program(tok, ifile); // -> declaration program

        if(program_val)
            return true;
        else // program failed
            return false; 

    }
    else  // declaration failed...
    {
        // declaration was unsuccessful...put back token
        ifile.seekg(pos);

        bool compound_val = compound(tok, ifile);

        if(compound_val) // -> compound
            return true;
        else 
            return false;
    }
}


/* 
    declaration -> type idlist SEMICOLON
*/
bool declaration(Token tok, std::ifstream &ifile)
{
    bool type_val = type(tok);

    if (type_val) // -> type
    {
        bool idlist_val = idlist(tok,ifile);

        if (idlist_val) // -> type idlist
        {
            //read token...
            tok.get(ifile);

            if(tok.type() == SEMICOLON) // -> type idlist SEMICOLON
            {
                SourceFile += tok.value() + "\n"; // add ';'
                return true;
            }
            else // not SEMICOLON
                return false;

        }
        else // idlist failed
            return false;

    } // type failed
    else
        return false;
}


/* 
    idlist -> ID | ID COMMA idlist
*/
bool idlist(Token tok, std::ifstream &ifile)
{
    // read a token...
    int pos = ifile.tellg();
    tok.get(ifile);

    if (tok.type() == ID) // -> ID
    {
        // add the ID
        insertTab();
        SourceFile += tok.value(); 

        //read next token...
        pos = ifile.tellg();
        tok.get(ifile);

        if(tok.type() == COMMA) // -> ID COMMA
        {
            SourceFile += tok.value() + " "; // add ','

            bool idlist_val = idlist(tok,ifile);

            if (idlist_val) // -> ID COMMA idlist
                return true;
            else
                return false;

        }
        else // not a COMMA
        {
            // unget token!
            ifile.seekg(pos);

            return true; // -> ID
        }
    }
    else // not an ID...should reach here
        return false;
}


/* 
    type -> INTEGER | FLOAT | VOID
*/
bool type(Token tok)
{ 
    // check if current token is one of the valid types
    if (tok.type() == INTEGER || tok.type() == FLOAT || tok.type() == VOID)
    {
        //add the type
        SourceFile += tok.value() + " ";
        return true;
    }
    else // invalid type
        return false;
}

/* 
    compound -> BEGIN stmtlist END
*/
bool compound(Token tok, std::ifstream &ifile)
{
    // read a token
    tok.get(ifile);

    if (tok.type() == BEGIN) // -> BEGIN
    {
        // Need a newline on the first BEGIN token
        if ( tabAmount <= 0)
            SourceFile += "\n";

        // add "begin"
        insertTab();
        SourceFile += tok.value() + "\n"; 

        // increment tab counter
        tabAmount++; 
    
        bool stmtlist_val = stmtlist(ifile, tok);

        if (stmtlist_val) // -> BEGIN stmtlist
        {
            //read in next token...
            tok.get(ifile);

            if (tok.type() == END) // -> BEGIN stmtlist END
            {
                //decrement current tab counter
                SourceFile += "\n";
                tabAmount--;
                insertTab();

                // add "end" 
                SourceFile += tok.value();       
               
                return true;
            }
            else // not end 
                return false;

        } // stmtlist failed
        else
            return false;

    }
    else // not BEGIN
        return false;
}

/* 
    stmtlist -> stmt | stmt SEMICOLON stmtlist
*/
bool stmtlist(std::ifstream &ifile, Token tok)
{
    bool stmt_val = stmt(ifile, tok);

    if (stmt_val) // -> stmt
    {
        //read a token
        int pos = ifile.tellg();
        tok.get(ifile);

        if (tok.type() == SEMICOLON) // -> stmt SEMICOLON
        {
            // add ";"
            SourceFile += tok.value() + '\n';

            bool stmtlist_val = stmtlist(ifile, tok);

            if (stmtlist_val) // -> stmt SEMICOLON stmtlist
                return true;
            else
                return false;
        }
        else // not SEMICOLON...unget token
        {
            ifile.seekg(pos);
            return true; // -> stmt
        }
    }
    else // stmt failed
        return false;
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
    // read a token
    int pos = ifile.tellg();
    tok.get(ifile);
    
    if(tok.type() == ID)  // -> ID
    {
        // add ID
        insertTab();
        SourceFile += tok.value();

        // read next token...
        pos = ifile.tellg();
        tok.get(ifile);
        
        if (tok.type() == LPAREN) // -> ID LPAREN
        {
            SourceFile += tok.value() + " "; // adding LPAREN

            bool exprlist_val = exprlist(ifile, tok);

            if (exprlist_val) // -> ID LPAREN exprlist
            {
                //read next token...
                pos = ifile.tellg();
                tok.get(ifile);

                if (tok.type() == RPAREN) // -> ID LPAREN exprlist RPAREN
                {
                    SourceFile += " " + tok.value(); // add RPAREN
                    return true;
                }
                else //not RPAREN
                    return false;           
            }
            else // exprlist failed
                return false; 

        }
        // not LPAREN...check if it is ASSIGNOP
        else if (tok.type() == ASSIGNOP)  // -> ID ASSIGNOP
        {
            // adding '='
            SourceFile += " " + tok.value() + " "; 

            bool expr_val = expr(ifile, tok);
            
            if (expr_val)  // -> ID ASSIGNOP expr
                return true;
            else
                return false;
        }

        //not LPAREN and ASSIGNOP
        ifile.seekg(pos);
    
        return true; // just -> ID
    }
    else if (tok.type() == IF) // -> IF
    {
        // add "if"
        insertTab();
        SourceFile += tok.value() + " "; 

        bool expr_val = expr(ifile, tok);

        if (expr_val) // -> IF expr
        {
            // read next token
            pos = ifile.tellg();
            tok.get(ifile);

            if (tok.type() == THEN) // -> IF expr THEN
            {
                // add "then"
                SourceFile += " " + tok.value() + "\n"; 
                tabAmount++; // increment tab counter

                bool compound_val = compound(tok, ifile);

                if (compound_val) // -> IF expr THEN compound
                {
                    // read next token
                    pos = ifile.tellg();
                    tok.get(ifile);

                    if (tok.type() == ELSE) // -> IF expr THEN compound ELSE
                    {
                        // decrement tab counter
                        SourceFile += '\n'; 
                        tabAmount--;
                        insertTab();

                        // add "else"
                        SourceFile += tok.value() + "\n";

                        // 
                        tabAmount++;

                        compound_val = compound(tok, ifile);
        
                        if (compound_val) // -> IF expr THEN compound ELSE compound
                        {
                            tabAmount--; // account for the previous increment tab
                            return true;
                        }
                        else
                            return false;

                    }
                    else // not ELSE
                        return false;

                }
                else // compound failed
                {
                    ifile.seekg(pos); // unget THEN
                    return false;
                }

            }
            else // did not read a THEN!
            {
                //need to unget !!
                return false;
            }
        } 
        else // expr failed
        {
            return false;
        }
    } // not an IF
    else if (tok.type() == WHILE) // -> WHILE
    {
        // add "while"
        insertTab(); // does not index on while
        SourceFile += tok.value() + " "; 

        // read next token 
        pos = ifile.tellg();
        tok.get(ifile);

        if (tok.type() == LPAREN) // -> WHILE LPAREN
        {
            SourceFile += tok.value() + " "; // add "("

            bool expr_val = expr(ifile, tok);

            if (expr_val) // -> WHILE LPAREN expr
            {
                // read next token...
                pos = ifile.tellg();
                tok.get(ifile);

                if(tok.type() == RPAREN) // -> WHILE LPAREN expr RPAREN
                {
                    SourceFile += " " + tok.value() + '\n'; // add ')'

                    bool compound_val = compound(tok, ifile);

                    if (compound_val) // -> WHILE LPAREN expr RPAREN compound
                        return true;
                    else // compound failed
                        return false;

                }
                else // did not read RPAREN... unget!
                    return false;
            }
            else //expr failed
                return false;

        }
        else // did not read LPAREN
            return false;
    }
    else // none of the TOKEN matched ...unget the token!! 
    {
        ifile.seekg(pos); 

        bool compound_val = compound(tok, ifile);

        if (compound_val)  // -> compound
            return true;
        else // compound failed
            return false;
    }

}


/* 
    exprlist -> expr | expr COMMA exprlist
*/
bool exprlist(std::ifstream &ifile, Token tok)
{
    bool expr_val = expr (ifile, tok);

    if (expr_val) // -> expr
    {
        //read next token...
        int pos = ifile.tellg(); 
        tok.get(ifile);

        if (tok.type() == COMMA) // -> expr COMMA
        {
            SourceFile += ", "; // add comma
        
            bool exprlist_val = exprlist(ifile, tok);

            if (exprlist_val) // -> expr COMMA exprlist
                return true;
            else
                return false;
        }
        else //not a comma...unget!
        {
            ifile.seekg(pos);
            return true; //  -> expr
        }
    
    }
    else
        return false;

}


/* 
    expr -> simpexpr | simpexpr RELOP simpexpr
*/
bool expr(std::ifstream &ifile, Token tok)
{
    bool simpexpr_val = simpexpr(ifile, tok);

    if (simpexpr_val) // -> simpexpr
    {
        // read next token
        int pos = ifile.tellg();
        tok.get(ifile);

        if (tok.type() == RELOP) // -> simpexpr RELOP
        {
            SourceFile += " " + tok.value() + " "; // add RELOP

            simpexpr_val = simpexpr(ifile, tok);

            if (simpexpr_val) // -> simpexpr RELOP simpexpr
                return true;
            else
                return false;
        }
        else //not RELOP...unget
        {
            ifile.seekg(pos);
            return true;    // -> simpexpr
        }

    }
    else
        return false;
}

/* 
    simpexpr -> term | term ADDOP simpexpr
*/
bool simpexpr(std::ifstream &ifile, Token tok)
{
    bool term_val = term(ifile, tok);

    if (term_val) // -> term 
    {
        // read next token...
        int pos = ifile.tellg();
        tok.get(ifile);

        if (tok.type() == ADDOP) // -> term ADDOP
        {
            SourceFile += " " + tok.value() + " "; // add ADDOP

            bool simpexpr_val = simpexpr(ifile, tok);

            if (simpexpr_val) // -> term ADDOP simpexpr
                return true;
            else // simpexpr failed
                return false;
        }
        else // not ADDOP... unget! 
        {
            ifile.seekg(pos);
            return true; // -> term
        }
    }
    else 
        return false;
}

/* 
    term -> factor | factor MULOP term
*/
bool term(std::ifstream &ifile, Token tok)
{
    bool factor_val = factor(ifile, tok);

    if (factor_val) // -> factor
    {
        // read next token...
        int pos = ifile.tellg();
        tok.get(ifile);

        if (tok.type() == MULOP) // -> factor MULOP 
        {
            SourceFile += " " + tok.value() + " "; // adding MULOP

            bool term_val = term(ifile, tok);

            if (term_val) // -> factor MULOP term
                return true;
            else // term failed
                return false;
             
        }
        else // not MULOP...unget token
        {
            ifile.seekg(pos);
            return true; // -> factor
        }

    }
    else // factor failed;
        return false;
}



/* 
    factor -> ID | ID LPAREN exprlist RPAREN | 
            NUM_REAL | NUM_INT
            LPAREN expr RPAREN
*/
bool factor(std::ifstream &ifile, Token tok)
{
    // read a token...
    int pos = ifile.tellg();
    tok.get(ifile);
    
    if (tok.type() == ID) // -> ID
    {
        SourceFile += tok.value(); // add ID

        //read next token
        pos = ifile.tellg();
        tok.get(ifile); 
        
        if (tok.type() == LPAREN) // -> ID LPAREN
        {
            SourceFile += tok.value(); // adding '('
            
            bool exprlist_val = exprlist(ifile, tok);
            
            if (exprlist_val) // -> ID LPAREN exprlist
            {
                //read next token...
                pos = ifile.tellg();
                tok.get(ifile);

                if (tok.type() == RPAREN) // -> ID LPAREN exprlist RPAREN
                {
                    SourceFile += tok.value(); // adding ')'
                    return true;
                }
                else // not RPAREN
                    return false;

            }
            else // exprlist failed
                return false;

        } 
        else //not LPAREN...unget... 
        {
            ifile.seekg(pos);
            return true; // -> ID
        }
    }
    else if (tok.type() == NUM_REAL || tok.type() == NUM_INT ) // ->  NUM_REAL | NUM_INT
    {
        SourceFile += tok.value(); // add the real number
        return true;
    }
    else if (tok.type() == LPAREN) // -> LPAREN
    {
        SourceFile += tok.value(); // adding '('

        bool expr_val = expr(ifile, tok);

        if (expr_val) // -> LPAREN expr
        {
            //read next token...
            tok.get(ifile);

            if (tok.type() == RPAREN) // -> LPAREN expr RPAREN
            {
                SourceFile += tok.value(); // adding RPAREN
                return true;
            }
            else // not RPAREN
                return false;

        }
        else // expr failed
            return false;
       
    }
    else // none of token matches
        return false;
}
