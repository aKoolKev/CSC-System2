#ifndef _GRAMMAR_HPP_
#define _GRAMMAR_HPP_


#include <iostream>
#include <fstream>


//expr -> term + expr | term - expr | term
int expr(std::ifstream &);

//term -> factor * term | factor/term | factor
int term(std::ifstream &);

//factor number | (<expr>)
int factor(std::ifstream &);


#endif