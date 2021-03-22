# include <cstdlib>
# include <vector>
# include <iostream>
# include <stdio.h>
# include <string>
# include <map>
# include <sstream>

using namespace std;

enum Type { LPAREN, RPAREN, INT, STRING, DOT, FLOAT, NIL, T, QUOTE, SYMBOL, UNKNOWN };

int gLine = 1 ;              // 「下一個要讀進來的字元」所在的line number
int gColumn = -1 ;            // 「下一個要讀進來的字元」所在的column number

int gTestNum ;

struct Token {
  string str ;
  int line ;
  int column ;
  Type type ;
};

bool Isatomtype( Type type ) {
  if ( type == SYMBOL || type == INT || type == FLOAT )
    return true ;
  else if ( type == STRING || type == NIL || type == T )
    return true ;   
  else 
    return false ;
  
} // Isatomtype()

bool Iswhitespace( char ch ) {
  if ( ch == ' ' ) return true ;
  else if ( ch == '\t' ) return true ;
  else if ( ch == '\n' ) return true ;
  else return false ;

} // Iswhitespace()

bool Isdigit( char ch ) {
  if ( ch >= '0' && ch <= '9' ) return true ;
  else return false ;

} // Isdigit()

bool Isseparators( char ch ) {
  if ( ch == '(' || ch == ')' ) return true ;
  else if ( ch == ';' || ch == '\"'  || ch == '\'' ) return true ;
  else if ( Iswhitespace( ch ) ) return true ;
  else return false ;
  
} // Isseparators() 

Type Numtype( string str ) {
  if ( !Isdigit( str[0] ) && str[0] != '+' && str[0] != '-' && str[0] != '.' )
    return UNKNOWN ;
  
  int dotnum = 0 ;
  if ( str[0] == '.' ) dotnum++ ;
  for ( int i = 1 ; i < str.size() ; i++ ) {
    if ( str[i] == '.' ) dotnum++ ;
    
    if ( !Isdigit( str[i] ) && str[i] != '.' )
      return UNKNOWN ;
  } // for
  
  if ( dotnum == 0 ) return INT ;
  else if ( dotnum == 1 ) return FLOAT ;
  else return UNKNOWN ;
  
  
} // Numtype()

bool Isend( vector< Token > tokenlist ) {
  int size = tokenlist.size() ;
  if ( size == 3 && tokenlist.at( 1 ).str == "exit" )
    return true ;
  else return false ;  
  
} // Isend()

class Exception {
public:
  string merrorstr;
  
  Exception() {  }

  Exception( string str ) {
    merrorstr = str;
  } // Exception()

}; // Exception

class Stringerror : public Exception {
public:
  Stringerror( Token temp ) {
    stringstream ss ;
    ss << "stringerror: " << temp.str << "  " << temp.type ;
    
    merrorstr = ss.str() ;
  } // Stringerror()
};

class Callend : public Exception {
public:
  Callend() {
    ;
  } // Callend()
};

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
  
  void ReadSexp( vector<Token> & tokenlist ) {
    Token temp ;
    Readnwschar() ;
    temp = Gettoken() ;
    // cout << "**" << temp.str << endl ;
    if ( temp.type == LPAREN ) {
      // cout << "l(" << temp.str << endl ;
      tokenlist.push_back( temp ) ;
      ReadSexp( tokenlist ) ;
      
      Readnwschar() ;
      // cout << ">>" << mch ;
      
      while ( mch != ')' && mch != '.' ) {
        ReadSexp( tokenlist ) ;
        Readnwschar() ;
       
      } // while
        
       
      if ( mch == '.' ) {
        temp = Gettoken() ;
        // cout << temp.str << endl ;
        tokenlist.push_back( temp ) ;
        ReadSexp( tokenlist ) ;
        Readnwschar() ;
      } // if
        
      Readnwschar() ;
      
      if ( mch == ')' ) {
       
        temp = Gettoken() ;
        // cout << temp.str << endl ;
        tokenlist.push_back( temp ) ;
      } // if
      else {
        cout << mch << "in" << endl ;
        throw Stringerror( temp ) ;// error
      } // else
      
    } // if
    else if ( Isatomtype( temp.type ) ) {
      // cout << temp.str << endl ;
      tokenlist.push_back( temp ) ;
    } // else if
    else if ( temp.type == QUOTE ) {
      tokenlist.push_back( temp ) ;
      ReadSexp( tokenlist ) ;
    } // else if 
    else {
      cout << "out" << endl ;
      throw Stringerror( temp ) ;// error
    } // else
    
  } // ReadSexp()
  
  void Gettokenlist() {
    
    // Readnwschar() ;
    while ( 0 == 0 ) {
      printf( "> " ) ;
      vector<Token> tokenlist ;
      try {
        
        ReadSexp(  tokenlist ) ;
        
      } // try
      catch ( Stringerror e ) {
        cout << e.merrorstr ;
      } // catch
      // cout << "readed" << endl ;
      if ( Isend( tokenlist ) )
        throw Callend() ; 
      for ( int i = 0 ; i < tokenlist.size() ; i++ ) {
        cout << tokenlist.at( i ).str << endl ;
        // cout << tokenlist.at( i ).line ;
        // cout << tokenlist.at( i ).column << endl ; 
      } // for
      
      cout << endl ;
    } // while()
    
    // cout << "done" ;
  } // Gettokenlist()
  
  /*
  void Gettokenlist() {
    printf( "> " ) ;
    Readnwschar() ;
    Token temp ;
    temp = Gettoken() ;
    while ( temp.str != "0" ) {
      tokenlist.push_back( temp ) ;
      
      cout << temp.str << "  " << temp.type << endl ;
      
      printf( "> " ) ;
      temp = Gettoken() ;
      
    } // while()
    
    // cout << "done" ;
  } // Gettokenlist() 
    
  void Print() {
    for ( int i = 0 ; i < tokenlist.size() ; i++ ) {
      cout << tokenlist.at(i).str ;
      cout << tokenlist.at(i).line ;
      cout << tokenlist.at(i).column << endl ; 
    }
    
  }  
  */
  
  private:
  
  // vector<Token> tokenlist ;

  void Getchar() {
    
    scanf( "%c", &mch ) ;
    gColumn++ ;
    if ( mch == '\n' ) {
      
      gColumn = -1 ;
      gLine++ ;
    } // if()
  
  } // Getchar() 
  
  Token Gettoken() {
    Token retoken ;
    retoken.column = gColumn ;
    retoken.line = gLine ;
    retoken.str = Gettokenstr() ;
    if ( retoken.str == ";" ) {
      
      while ( mch != '\n' )
        Getchar() ;
    } // if
    // cout << "type" << endl ;
    // retoken.str = Setstr( Gettokenstr() ) ;
    retoken.type = Gettype( retoken.str ) ;
    
    return retoken ;
  } // Gettoken()
  
  Type Gettype( string str ) {
    Type numtype = Numtype( str ) ;
    if ( str == "(" ) return LPAREN ;
    else if ( str == ")" ) return RPAREN ;
    else if ( str == "." ) return DOT ;
    else if ( str == "nil" || str == "#f"  || str == "()" ) return NIL ;
    else if ( str == "t" || str == "#t" ) return T ;
    else if ( str[0] == '\"' && str[str.size() - 1] == '\"' ) return STRING ;
    else if ( str == "'" ) return QUOTE ;
    else if ( numtype == INT || numtype == FLOAT ) return numtype ;
    else return SYMBOL ;  
      
  } // Gettype()
  
  string Setstr( string str ) {
    
    return "000";
    
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
       
    while ( mch != '\"' )  {
      if ( mch == '\n' ) return "ERROR" ;
      
      bool check = true ;
      if ( mch == '\\' ) {
        Getchar() ; 
        if ( mch == 't' ) {
          temp += '\t' ;
        } // if
        else if ( mch == 'n' ) {
          temp += '\n' ;
        } // else if
        else if ( mch == '\"' ) {
          temp += '\"' ;
        } // else if
        else if ( mch == '\\' ) {
          temp += '\\' ;
        } // else if
        else {
          check = false ;
        } // else
      } // if()
      else {
        temp += mch ;
        
      } // else
      
      if ( check )
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
    if ( temp == "(" ) {
      Readnwschar() ;
      
      if ( mch == ')' ) {
        temp += ")" ; 
        Getchar() ;
      } // if
    } // if 
    
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
  cin >> gTestNum ;
  printf( "Welcome to OurScheme!\n" ) ;
  try {
    scanner.Gettokenlist() ;
  } // try
  catch ( Callend e ) {
    ;
  } // catch
  
  // scanner.Print() ;
  printf( "Thanks for using OurScheme!\n" ) ;
  
} // main()
