#ifndef _GRAMMAR_HPP_
#define _GRAMMAR_HPP_

#include "Token.hpp"
#include <iostream>
#include <fstream>

//mine ...testing
void printSourceFile();

/* 
    program -> declaration program | compound
*/
bool program(Token, std::ifstream &);


/* 
    declaration -> type idlist SEMICOLON
*/
bool declaration(Token, std::ifstream &);


/* 
    idlist -> ID | ID COMMA idlist
*/
bool idlist(Token, std::ifstream &);


/* 
    type -> INTEGER | FLOAT | VOID
*/
bool type(Token);


/* 
    compound -> BEGIN stmtlist END
*/
bool compound(Token, std::ifstream &);


/* 
    stmtlist -> stmt | stmt SEMICOLON stmtlist
*/
bool stmtlist(std::ifstream &, Token);


/* 
    stmt -> ID| ID LPAREN exprlist RPAREN |
            ID ASSIGNOP expr |
            IF expr THEN compound ELSE compound |
            WHILE LPAREN expr RPAREN compound |
            compound
*/
bool stmt(std::ifstream &, Token);


/* 
    exprlist -> expr | expr COMMA exprlist
*/
bool exprlist(std::ifstream &, Token);


/* 
    expr -> simpexpr | simpexpr RELOP simpexpr
*/
bool expr(std::ifstream &, Token);


/* 
    simpexpr -> term | term ADDOP simpexpr
*/
bool simpexpr(std::ifstream &, Token);


/* 
    term -> factor | factor MULOP term
*/
bool term(std::ifstream &);


/* 
    factor -> ID | ID LPAREN exprlist RPAREN | 
            NUM_REAL | NUM_INT
            LPAREN expr RPAREN
*/
bool factor(std::ifstream &);
            

#endif
