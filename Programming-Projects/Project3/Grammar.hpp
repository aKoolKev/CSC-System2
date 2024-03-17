//File: Grammar.hpp
//Author: Kevin Dong
//Date: 3/14/24
//Purpose: Definitions of the grammars


#ifndef _GRAMMAR_HPP_
#define _GRAMMAR_HPP_

#include "Token.hpp"
#include <iostream>
#include <fstream>


void printSourceFile();

// THE GRAMMARS BELOW:

/*
    <prog> -> <slist>
*/
bool prog(Token, std::ifstream &);


/* 
    <slist> -> <stmt> SEMICOLON <slist> | ϵ
*/
bool slist(Token, std::ifstream &);


/* 
    <stmt> -> ID LPAREN ID RPAREN |
              ID LPAREN RPAREN |
              ID ASSIGNOP <rhs>
*/
bool stmt(Token, std::ifstream &);

/* 
    <rhs> -> ID LPAREN NUM_INT RPAREN |
              ID
*/
bool rhs(Token, std::ifstream &, std::string);




#endif
