#include <fstream>
#include <iostream>
#include <string>

#include "Token.hpp"
#include "Grammar.hpp"

using namespace std;

int main(int argc, char *argv[])
{
  ifstream ifile(argv[1]);
  

  //expr -> term + expr | term - expr | term
  cout << expr(ifile) << endl;


  return 0;
}
