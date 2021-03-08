# include <cstdlib>
# include <iostream>
# include <stdio.h>
# include <string>

using namespace std;

enum Type { LPAREN, PAREN, INT, STRING, DOT, FLOAT, NIL, T, QUOTE, SYMBOL };

int gLine = 1 ;              // 「下一個要讀進來的字元」所在的line number
int gColumn = 1 ;            // 「下一個要讀進來的字元」所在的column number

struct Token {
  string str ;
  int line ;
  int column ;
  Type type ;
};

class Scanner{
  public:
  void Print() {
    string str = "aaa" ;
    str+= Getchar() ;
    cout << str ;
  } // PrintV()
    
  private:
  void PrintA() {
    
    cout << "a" ;
  } // PrintA()
  
  char Getchar(){
    char ch = '\0' ;
    
    scanf( "%c", &ch ) ;
    gColumn++ ;
    if ( ch == '\n' ) {
      gColumn = 1 ;
      gLine++ ;
    }
    return ch ;
  
  }
  
  void Gettokenlist() {
    string temp = "" ;
    temp = Gettoken() ;
    while( temp != "exit" ) {
      
    }
    
  }
  
};

int main() {
  Scanner scanner = Scanner() ;
  scanner.Print() ;
} // main()
