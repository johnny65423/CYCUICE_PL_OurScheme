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

bool iswhitespace( char ch ) {
  if ( ch == ' ' ) return true ;
  else if ( ch == '\t' ) return true ;
  else if ( ch == '\n' ) return true ;
  else return false ;
  
}

bool isdight( char ch ) {
  if ( ch >= '0' && ch <= '9' ) return true ;
  else return false ;
		  
}

bool isseparators( char ch ) {
  if ( ch == '(' || ch == ')' ) return true ;
  else if ( ch == ';' || ch == '\"'  || ch == '\'' ) return true ;
  else if ( iswhitespace( ch ) ) return true ;
  else return false ;
		  
}


class Scanner{
  public:
  char ch ;  
  
  Scanner() {
    ch = '\0' ;
    
  }
  void Readnwschar(){
    while( ch == ' ' || ch == '\n' || ch == '\t' || ch == '\0' ) {
      Getchar() ;
    }
  }
  
  void Gettokenlist() {
    
    Readnwschar() ;
    string temp = "" ;
    temp = Gettoken() ;
    while( temp != "\"exit\"" ) {
      
      cout << "Token  "<< temp << endl ;
      
      temp = Gettoken() ;
      
    }
    cout << "done" ;
  }
    
  private:

  void Getchar(){
    
    scanf( "%c", &ch ) ;
    gColumn++ ;
    if ( ch == '\n' ) {
      gColumn = 1 ;
      gLine++ ;
    }
  
  }
  
  string Gettoken(){
    Readnwschar() ;
    
    if( ch == '\"' ) {
      
      return Getstring( ch ) ;
    }
    else if ( isseparators(ch) ) {
      return Getseparators( ch ) ;
    }
  }
  
  string Getstring( char & ch ) {
    string temp = "" ;
    temp += ch ;
    Getchar() ;
       
    while ( ch != '\"' ){
      temp += ch ;
      
      if( ch == '\\' ) {
        Getchar() ; 
        temp += ch ;
      }
      
      Getchar() ; 
    }
    
    temp += ch ;
    Getchar() ;
    
    return temp ;
  } 

  string Getseparators( char & ch ) {
    string temp = "" ; 
    temp += ch ;
    Getchar() ;
    
    return temp ;
  } 
  
};

int main() {
  Scanner scanner = Scanner() ;
  scanner.Gettokenlist() ;
} // main()
