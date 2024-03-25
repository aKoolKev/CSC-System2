/*
  ┌───────────────────────────────────────┐
    FILE:    Grammar.hpp
    AUTHOR:  Kevin Dong
    DATE:    3/14/24
    PURPOSE: Definitions of the grammars
  └───────────────────────────────────────┘
*/

#ifndef _GRAMMAR_HPP_
#define _GRAMMAR_HPP_

#include "Command.hpp"
#include <fstream>
#include <iostream>
#include <list>
#include "Token.hpp"


// THE GRAMMARS BELOW:

/*
    <prog> -> <slist>
*/
bool prog(Token, std::ifstream &, int);

/* 
    <slist> -> <stmt> SEMICOLON <slist> | ϵ
*/
bool slist(Token, std::ifstream &, Command&);

/* 
    <stmt> -> ID LPAREN ID RPAREN |
              ID LPAREN RPAREN |
              ID ASSIGNOP <rhs>
*/
bool stmt(Token, std::ifstream &, Command&);

/* 
    <rhs> -> ID LPAREN NUM_INT RPAREN |
              ID
*/
bool rhs(Token, std::ifstream &, std::string, Command&);

#endif
