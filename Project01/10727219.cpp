# include <cstdlib>
# include <iostream>
# include <stdio.h>
# include <string>

using namespace std;

enum Type { LPAREN, PAREN, INT, STRING, DOT, FLOAT, NIL, T, QUOTE, SYMBOL };

class Token{
  public:
  void PrintV() {
      
    cout << "vvv" ;
  } // PrintV()
    
  private:
  void PrintA() {
      
    cout << "a" ;
  } // PrintA()
  
};

int main() {
  Token t = Token() ;
  t.PrintV() ;
} // main()
