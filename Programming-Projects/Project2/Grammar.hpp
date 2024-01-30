#ifndef _GRAMMAR_HPP_
#define _GRAMMAR_HPP_

#include <iostream>
#include <fstream>


/* 
    program -> declaration program | compound
*/
std::string program(std::ifstream &);


/* 
    declaration -> type idlist SEMICOLON
*/
std::string declaration(std::ifstream &);


/* 
    idlist -> ID | ID COMMA idlist
*/
std::string idlist(std::ifstream &);


/* 
    type -> INTEGER | FLOAT | VOID
*/
std::string type(std::ifstream &);


/* 
    compound -> BEGIN stmtlist END
*/
std::string compound(std::ifstream &);


/* 
    stmt -> ID| ID LPAREN exprlist RPAREN |
            ID ASSIGNOP expr |
            IF expr THEN compound ELSE compound |
            WHILE LPAREN expr RPAREN compound |
            compound
*/
std::string stmt(std::ifstream &);


/* 
    exprlist -> expr | expr COMMA exprlist
*/
std::string exprlist(std::ifstream &);


/* 
    expr -> simpexpr | simpexpr RELOP simpexpr
*/
std::string expr(std::ifstream &);


/* 
    simpexpr -> term | term ADDOP simpexpr
*/
std::string simpexpr(std::ifstream &);


/* 
    term -> factor | factor MULOP term
*/
std::string term(std::ifstream &);


/* 
    factor -> ID | ID LPAREN exprlist RPAREN | 
            NUM_REAL | NUM_INT
            LPAREN expr RPAREN
*/
std::string factor(std::ifstream &);
            

#endif