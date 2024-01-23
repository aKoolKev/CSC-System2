#ifndef _GRAMMAR_HPP_
#define _GRAMMAR_HPP_


#include <iostream>
#include <fstream>


//expr -> term expr2
int expr(std::ifstream &);

//expr2 - > + term expr2 | - term expr2 | e
int expr2(std::ifstream &, int lhs);


//term -> factor term2
int term(std::ifstream &);

// * factor term2 | / factor term2
int term2(std::ifstream &, int lhs);

//factor INTEGER | (<expr>)
int factor(std::ifstream &);


#endif