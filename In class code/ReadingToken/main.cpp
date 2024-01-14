#include <fstream>
#include <iostream>
#include <string>

#include "Token.hpp"

using namespace std;

int main(int argc, char *argv[])
{
  //create a file from command line
  ifstream ifile(argv[1]);
  
  //prime reading in a token from file
  Token t;
  ifile >> t;

  //keep reading until no more token
  while (ifile)
  {
    cout << t << ": " << t.stype() << "(" << t.type() << ")" << endl;
    ifile >> t;
  }

  return 0;
}
