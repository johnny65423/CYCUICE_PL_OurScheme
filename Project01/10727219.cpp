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


bool Iswhitespace( char ch ) {
  if ( ch == ' ' ) return true ;
  else if ( ch == '\t' ) return true ;
  else if ( ch == '\n' ) return true ;
  else return false ;

} // Iswhitespace()

bool Isdight( char ch ) {
  if ( ch >= '0' && ch <= '9' ) return true ;
  else return false ;

} // Isdight()

bool Isseparators( char ch ) {
  if ( ch == '(' || ch == ')' ) return true ;
  else if ( ch == ';' || ch == '\"'  || ch == '\'' ) return true ;
  else if ( Iswhitespace( ch ) ) return true ;
  else return false ;
  
} // Isseparators() 


class Scanner{
  public:
  char mch ;  
  
  Scanner() {
    mch = '\0' ;
    
  } // Scanner() 
  
  void Readnwschar() {
    while ( mch == ' ' || mch == '\n' || mch == '\t' || mch == '\0' ) {
      Getchar() ;
    } // while()
    
  } // Readnwschar() 
  
  void Gettokenlist() {
    
    Readnwschar() ;
    string temp = "" ;
    temp = Gettoken() ;
    while ( temp != "\"exit\"" ) {
      
      cout << "Token  " << temp << endl ;
      
      temp = Gettoken() ;
      
    } // while()
    
    cout << "done" ;
  } // Gettokenlist() 
    
  private:

  void Getchar() {
    
    scanf( "%c", &mch ) ;
    gColumn++ ;
    if ( mch == '\n' ) {
      gColumn = 1 ;
      gLine++ ;
    } // if()
  
  } // Getchar() 
  
  string Gettoken() {
    Readnwschar() ;
    
    if ( mch == '\"' ) {
      
      return Getstring( mch ) ;
    } // if()
    else if ( Isseparators( mch ) ) {
      return Getseparators( mch ) ;
    } // else if()
    else {
      return "000" ;
    } // else
  } // Gettoken() 
  
  string Getstring( char & mch ) {
    string temp = "" ;
    temp += mch ;
    Getchar() ;
       
    while ( mch != '\"' ) {
      temp += mch ;
      
      if ( mch == '\\' ) {
        Getchar() ; 
        temp += mch ;
      } // if()
      
      Getchar() ; 
    } // while()
    
    temp += mch ;
    Getchar() ;
    
    return temp ;
  } // Getstring() 

  string Getseparators( char & mch ) {
    string temp = "" ; 
    temp += mch ;
    Getchar() ;
    
    return temp ;
  } // Getseparators() 
  
};

int main() {
  Scanner scanner = Scanner() ;
  scanner.Gettokenlist() ;
} // main()
