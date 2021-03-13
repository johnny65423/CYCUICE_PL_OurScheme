# include <cstdlib>
# include <iostream>
# include <stdio.h>
# include <string>
# include <map>

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
    printf( "> " ) ;
    Readnwschar() ;
    Token temp ;
    temp = Gettoken() ;
    while ( temp.str != "\"exit\"" ) {
      
      cout << temp.str << endl ;
      printf( "> " ) ;
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
  
  Token Gettoken() {
    Token retoken ;
    retoken.column = gColumn ;
    retoken.line = gLine ;
    //retoken.str = Setstr( Gettokenstr() ) ;
    //retoken.type = Gettype() ;
    return retoken ;
  } // Gettoken()
  
  string Setstr( string str ) {
    
    ;
    
  } // Setstr()
  
  
  
  string Gettokenstr() {
    Readnwschar() ;
    
    if ( mch == '\"' ) {
      
      return Getstring( mch ) ;
    } // if()
    else if ( Isseparators( mch ) ) {
      return Getseparators( mch ) ;
    } // else if()
    else {
      return Getothers( mch ) ;
    } // else
  } // Gettokenstr() 
  
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
  
  string Getothers( char & mch ) {
    string temp = "" ; 
    temp += mch ;
    Getchar() ;
    
    while ( !Isseparators( mch ) ) {
      temp += mch ;
      
      Getchar() ; 
      
    } // while()
    
    return temp ;
  } // Getothers() 
  
};

int main() {
  Scanner scanner = Scanner() ;
  printf( "Welcome to OurScheme!\n" ) ;
  scanner.Gettokenlist() ;
  printf( "Thanks for using OurScheme!\n" ) ;
  
} // main()
