# include <cstdlib>
# include <vector>
# include <iostream>
# include <stdio.h>
# include <string>
# include <map>
# include <sstream>
# include <iomanip>

 
using namespace std;

enum Type { LPAREN, RPAREN, INT, STRING, DOT, FLOAT, NIL, T, QUOTE, SYMBOL, COMMENT, UNKNOWN };

int gLine = 1 ;              // 「下一個要讀進來的字元」所在的line number
int gColumn = 0 ;            // 「下一個要讀進來的字元」所在的column number
bool gReading = false ;
int gTestNum ;

struct Token {
  string str ;
  
  int line ;
  int column ;
  int intnum ;
  float floatnum ;
  Type type ;
};

char Cpeek() {
  // int num = getc( stdin ) ;
  char ch = cin.peek();
  // ungetc(ans, stdin);
  return ch;
} // Cpeek()

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
  
  bool hasdigit = false ;
  
  int dotnum = 0 ;
  if ( str[0] == '.' ) dotnum++ ;
  for ( int i = 1 ; i < str.size() ; i++ ) {
    if ( str[i] == '.' ) dotnum++ ;
    if ( Isdigit( str[i] ) )
      hasdigit = true ;
    if ( !Isdigit( str[i] ) && str[i] != '.' )
      return UNKNOWN ;
  } // for
  
  if ( !hasdigit ) return UNKNOWN ;
  else if ( dotnum == 0 ) return INT ;
  else if ( dotnum == 1 ) return FLOAT ;
  else return UNKNOWN ;

} // Numtype()

bool Isend( vector< Token > tokenlist ) {
  int size = tokenlist.size() ;
  if ( size == 3 && tokenlist.at( 1 ).str == "exit" )
    return true ;
  else return false ;  
  
} // Isend()

int Decodeint( string str ) {
  int positive = 1 ;
  if ( str[0] == '+' )
    str.erase( 0, 1 ) ;
  else if ( str[0] == '-' ) {
    str.erase( 0, 1 ) ;
    positive = -1 ;
  } // else if
  
  int num = atoi( str.c_str() ) * positive ; 
  return num ;
} // Decodeint()

float Decodefloat( string str ) {
  int positive = 1 ;
  if ( str[0] == '+' )
    str.erase( 0, 1 ) ;
  else if ( str[0] == '-' ) {
    str.erase( 0, 1 ) ;
    positive = -1 ;
  } // else if
  
  if ( str[0] == '.' )
    str = "0" + str ;
  
  float num = atof( str.c_str() ) * positive ; 
  return num ;
} // Decodefloat()

bool Justdot() {
  char ch = Cpeek() ;
  
  if ( Iswhitespace( ch ) )
    return true ;
  else if ( Isseparators( ch ) )
    return true ;
  else 
    return false ;  
} // Justdot()

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
  Stringerror( Token temp, int check ) {
    stringstream ss ;
    if ( check == 0 ) {
      ss << "ERROR (unexpected token) : atom or '(' expected when token at Line " ;
      ss << temp.line << " Column " << temp.column << " is >>" << temp.str << "<<" ;
    } // if
    else if ( check == 1 ) {
      ss << "ERROR (unexpected token) : ')' expected when token at Line " ;
      ss << temp.line << " Column " << temp.column << " is >>" << temp.str << "<<" ;
    } // else if 
    else {
      ss << "END-OF-LINE encountered at Line " ;
      ss << temp.line << " Column " << temp.column << "" ;
    } // else
    
    merrorstr = ss.str() ;
  } // Stringerror()
};

class Callend : public Exception {
public:
  Callend() {
    ;
  } // Callend()
};

class EndOfFileError : public Exception {
public:
  EndOfFileError() {
    stringstream ss ;
    if ( gReading ) {
      ss << "ERROR (no closing quote) : END-OF-LINE encountered at Line " << gLine << " Column " << gColumn ;
    } // if
    else {
      ss << "ERROR (no more input) : END-OF-FILE encountered" ;
    } // else 

    merrorstr = ss.str() ;
  } // EndOfFileError()
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
  
  void Skipcomment() {
    while ( mch == ';' ) {
      Getchar() ;
      while ( mch != '\n' ) {
        Getchar() ;
      } // while
      
      Readnwschar() ;
    } // while
  } // Skipcomment()
  
  void ReadSexp( vector<Token> & tokenlist ) {
    Token temp ;
    Readnwschar() ;
    temp = Gettoken() ;
    // cout << "**" << temp.str << endl ;
    gReading = true ;
    if ( temp.type == LPAREN ) {
      // cout << "l(" << temp.str << endl ;
      tokenlist.push_back( temp ) ;
      ReadSexp( tokenlist ) ;
      
      Readnwschar() ;
      // cout << ">>" << mch ;
      
      while ( mch != ')' && ! ( mch == '.' && Justdot() ) ) {
        ReadSexp( tokenlist ) ;
        Readnwschar() ;
        Skipcomment() ;
        // cout << "mch :" << mch << endl ;
      } // while
        
      // cout <<">>" << mch << "<<"<< endl ; 
      
      if ( mch == '.' && Justdot() ) {
        // cout <<"dddot" << endl ; 
        temp = Gettoken() ;
        // cout << temp.str << endl ;
        tokenlist.push_back( temp ) ;
        ReadSexp( tokenlist ) ;
        Readnwschar() ;
      } // if
        
      Readnwschar() ;
      Skipcomment() ;
      // cout <<">>" << mch << "<<"<< endl ; 
      if ( mch == ')' ) {
       
        temp = Gettoken() ;
        // cout << temp.str << endl ;
        tokenlist.push_back( temp ) ;
      } // if
      else {
        // cout << mch << "in" << endl ;
        temp = Gettoken() ;
        throw Stringerror( temp, 1 ) ;// error
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
      // cout << temp.str << endl ;
      throw Stringerror( temp, 0 ) ;// error
    } // else
    
  } // ReadSexp()
  
  void Gettokenlist() {
    
    // Readnwschar() ;
    while ( 0 == 0 ) {
      
      bool err = false ;
      printf( "> " ) ;
      vector<Token> tokenlist ;
      
      try {
        Readnwschar() ;
        Skipcomment() ;
        gLine = 1 ;
        ReadSexp(  tokenlist ) ;
        
      } // try
      catch ( Stringerror e ) {
        printf( "%s\n", e.merrorstr.c_str() ) ;
        while ( mch != '\n' )
          Getchar() ;
        tokenlist.clear() ;
        err = true ;
      } // catch
      
      // cout << "readed" << endl ;
      gReading = false ;
      
      if ( !err ) {
        // cout << tokenlist.size() ;
        if ( Isend( tokenlist ) )
          throw Callend() ; 
          
        /*
        for ( int i = 0 ; i < tokenlist.size() ; i++ ) {
          
          if ( tokenlist.at( i ).type == INT )
            cout << tokenlist.at( i ).intnum << endl ;
          else if ( tokenlist.at( i ).type == FLOAT )
            cout << fixed << setprecision( 3 ) << tokenlist.at( i ).floatnum << endl ;
          else 
            cout << tokenlist.at( i ).str << endl ;
  
        } // for
        */
        Buildtree( tokenlist ) ;  
      } // if
              
      cout << endl ;
      if ( mch == '\n' )
        gLine = 0 ;
      else   
        gLine = 1 ;
      gColumn = 1 ;
    } // while()
    
    // cout << "done" ;
  } // Gettokenlist()
  
  private:
  
  void Getchar() {
    
    int eof = scanf( "%c", &mch ) ;
    if ( eof == EOF )
      throw EndOfFileError() ;
    gColumn++ ;
    if ( mch == '\n' ) {
      
      gColumn = 0 ;
      gLine++ ;
    } // if()
  
  } // Getchar() 
  
  Token Maktoken( string str ) {
    Token retoken ;
    retoken.column = -1 ;
    retoken.line = -1 ;
    retoken.str = str ;
    retoken.type = Gettype( retoken.str ) ;
    retoken.str = Setstr( retoken.str ) ;
    if ( retoken.type == INT )
      retoken.intnum = Decodeint( retoken.str ) ;
    else if ( retoken.type == FLOAT )
      retoken.floatnum = Decodefloat( retoken.str ) ;
      
    return retoken ;
  } // Maktoken()
  
  Token Gettoken() {
    Token retoken ;
    Readnwschar() ; 
    retoken.column = gColumn ;
    retoken.line = gLine ;
    retoken.str = Gettokenstr() ;
    while ( retoken.str == ";" ) {
      while ( mch != '\n' )
        Getchar() ;
      Readnwschar() ; 
      retoken.column = gColumn ;
      retoken.line = gLine ;
      retoken.str = Gettokenstr() ;
    } // while
    
    retoken.type = Gettype( retoken.str ) ;
    retoken.str = Setstr( retoken.str ) ;
    
    if ( retoken.type == INT )
      retoken.intnum = Decodeint( retoken.str ) ;
    else if ( retoken.type == FLOAT )
      retoken.floatnum = Decodefloat( retoken.str ) ;
      
    // cout << retoken.str << " " << retoken.line << " " << retoken.column << endl ;
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
    else if ( str == ";" ) return COMMENT ;
    else return SYMBOL ;  
      
  } // Gettype()
  
  string Setstr( string str ) {
    
    if ( str == "nil" || str == "#f"  || str == "()" ) return "nil" ;
    else if ( str == "t" || str == "#t" ) return "#t" ;
    else return str ; 
    
  } // Setstr()
  
  string Gettokenstr() {
    Readnwschar() ;
    
    if ( mch == '\"' ) {
      
      return Getstring( mch ) ;
    } // if
    else if ( Isseparators( mch ) ) {
      return Getseparators( mch ) ;
    } // else if
    else {
      return Getothers( mch ) ;
    } // else
  } // Gettokenstr() 
  
  string Getstring( char & mch ) {
    string temp = "" ;
    temp += mch ;
    Getchar() ;
       
    while ( mch != '\"' )  {
      if ( mch == '\n' ) {
        Token nonono ;
        throw Stringerror( nonono, 2 ) ;
      } // if
      
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
          temp += '\\' ;
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
  
  void Buildtree( vector<Token> tokenlist ) {
    map< int, Token > tokentree ;
    int point = 1 ;
    int index = 0 ;
    // cout << "size "<< tokenlist.size() ;
    if ( tokenlist.size() == 1 ) {
      // tokentree.insert( pair< int, Token >( 1, tokenlist.at( 0 ) ) ) ;
      // cout << tokenlist.at(0).str << endl ;
      tokentree[1] =  tokenlist.at( 0 ) ;
    } // if
    else {
      Treerecursion( tokentree, tokenlist, point, index ) ;
    } // else
    /*  
     for( int i = 0 ; i < 102400 ; i++ ) {
       map< int, Token >::iterator temp = tokentree.find(i) ;
       if( temp != tokentree.end() ) {
         cout << i << " " << temp->second.str << endl ;  
      }
    }
    */
    // cout << endl ;
    // cout << "printtree" << endl ; 
    
    Printtree( tokentree, point, 0 ) ;
    
  
  
  } // Buildtree()
  
  /*
  void Treerecursion2( map< int, Token > & tokentree, vector<Token> tokenlist, int point, int & index ) {
    // (((1 . 2) (3 4) 5 . 6) 7 . 8)
    // ( (1 (2 3) 4 . 5) 6 . 7 )
    
    // ( ( 1 (2 3) 4 ) 5 )
    // cout << index << " " << tokenlist.at( index ).str << endl;
    cout << "r" << endl ; 
    if( tokenlist.at( index ).type == QUOTE ) {
      // tokentree.insert( pair< int, Token >( point, Maktoken( "." ) ) ) ;
      tokentree[ point ] = Maktoken( "." ) ;
      Treerecursion( tokentree, tokenlist, 2*point, index ) ;
      index++ ;
      Treerecursion( tokentree, tokenlist, 2*point+1, index ) ;
    } // if
    else if( tokenlist.at( index ).type == LPAREN ) {
      cout << "LP "<< index << " " << tokenlist.at( index ).str << endl;
      tokentree.insert( pair< int, Token >( point, Maktoken( "." ) ) ) ;
      index++ ;
      if( tokenlist.at( index ).type != LPAREN ){
        // cout << 2*point << " " << tokenlist.at( index ).str << endl;
        tokentree.insert( pair< int, Token >( 2*point, tokenlist.at( index ) ) ) ;
      } 
      else
        Treerecursion( tokentree, tokenlist, 2*point, index ) ;
        
      index++ ;
      cout << "next "<< index << " " << tokenlist.at( index ).str << endl;
      Treerecursion( tokentree, tokenlist, 2*point+1, index ) ;
      cout << "final "<< index << " " << tokenlist.at( index ).str << endl;
      
      
    } // if
    else if( tokenlist.at( index ).type == DOT ) {
      cout << "dot "<< index << " " << tokenlist.at( index ).str << endl;
      // not finish : if index++ is >>(<< 
      index++ ;
      if( tokenlist.at( index ).type != LPAREN ) {
        // cout << point << " " << tokenlist.at( index ).str << endl;
        tokentree.insert( pair< int, Token >( point, tokenlist.at( index ) ) ) ;  
        index++ ;
        // cout << "nextnext "<< index << " " << tokenlist.at( index ).str << endl;
      } // if
      else {
        Treerecursion( tokentree, tokenlist, point, index ) ;
        index++ ;
      }
        
        
      // tokentree.insert( pair< int, Token >( point, tokenlist.at( index ) ) ) ;
      
    } // if
    else {
      // cout << "else "<< index << " " << tokenlist.at( index ).str << endl;
      tokentree.insert( pair< int, Token >( point, Maktoken( "." ) ) ) ;
      // cout << 2*point << " " << tokenlist.at( index ).str << endl;
      tokentree.insert( pair< int, Token >( 2*point, tokenlist.at( index ) ) ) ;
      index++ ;
    
      if( tokenlist.at( index ).type == RPAREN )
        tokentree.insert( pair< int, Token >( 2*point+1, Maktoken( "nil" ) ) ) ;
      else
        Treerecursion( tokentree, tokenlist, 2*point+1, index ) ;
    }
    
    // cout << index << endl ;
    cout << index << " " << tokenlist.at( index ).str << endl;
  } // Treerecursion()
  */ 
  
  void Treerecursion( map< int, Token > & tokentree, vector<Token> tokenlist, int point, int & index ) {
    // cout << tokenlist.at( index ).str << " " << point << endl ;
    if ( tokenlist.at( index ).type == LPAREN ) {
      
      index++ ;
      Type type = tokenlist.at( index ).type ;
      while ( type != DOT && type != RPAREN ) {
        // cout << ". "<< point << endl;
        // tokentree.insert( pair< int, Token >( point, Maktoken( "." ) ) ) ;
        tokentree[ point ] = Maktoken( "." ) ;
        // cout << "loop" << tokenlist.at( index ).str << " " << 2 * point << endl ;
        Treerecursion( tokentree, tokenlist, 2 * point, index ) ; 
        point = 2 * point + 1 ;
        type = tokenlist.at( index ).type ;
        
      } // while
      // cout << "looped" << tokenlist.at( index ).str << endl ;    
      // point = point / 2 ;
      if ( index < tokenlist.size() - 1 && tokenlist.at( index ).type == RPAREN )
        index++ ;
        
      if ( tokenlist.at( index ).type == DOT ) {
        index++ ;  
        // cout << "dot" << tokenlist.at( index ).str << endl ;
        Treerecursion( tokentree, tokenlist, point, index ) ;
        index++ ;
      } // if
      else {
        // cout << "end" << tokenlist.at( index ).str << endl ;  
        // tokentree.insert( pair< int, Token >( point, Maktoken( "nil" ) ) ) ;
        tokentree[ point ] = Maktoken( "nil" ) ;
        // index++ ; 
      } // else

    } // if
    else {
      // tokentree.insert( pair< int, Token >( point, tokenlist.at( index ) ) ) ;
      tokentree[ point ] = tokenlist.at( index ) ;
      // cout << tokenlist.at( index ).str << " " << point << endl ;
      index++ ;
    } // else 
  } // Treerecursion()
  
  void Printtoken( Token token ) {
    Type type = token.type ;
    // cout << type << endl ;
    if ( type == INT )
      printf( "%d", token.intnum ) ;
    else if ( type == FLOAT )
      printf( "%.3f", token.floatnum ) ;
    else 
      printf( "%s", token.str.c_str() ) ;
         
  } // Printtoken()
  
  void Printtree( map< int, Token > & tokentree, int point, int spacenum ) {
    // string str = tokentree.find( point )->second.str;
    Type type = tokentree.find( point )->second.type ;
    // cout << "print" << tokentree.find( point )->second.str << endl ;
    if ( type == DOT ) {
      for ( int i = 0 ; i < spacenum ; i++ )
        printf( " " ) ;
        
      printf( "( " ) ;
      
      Printtree( tokentree, 2*point, spacenum ) ;
      
      point = 2 * point + 1 ;
      while ( tokentree.find( point ) != tokentree.end() ) {
        type = tokentree.find( point )->second.type ;
        if ( type == DOT ) {
          type = tokentree.find( 2 * point )->second.type ;
          
          if ( type == DOT )
            Printtree( tokentree, 2 * point, spacenum + 2 ) ;
          else {
            for ( int i = 0 ; i < spacenum + 2 ; i++ )
              printf( " " ) ;
            Printtoken( tokentree.find( 2 * point )->second ) ;
            printf( "\n" ) ;
          } // else
        } // if
        else {
          Printtree( tokentree, point, spacenum ) ;
        } // else
          
        
        point = 2 * point + 1 ;
      } // while
      
      for ( int i = 0 ; i < spacenum ; i++ )
        printf( " " ) ;
        
      printf( ")\n" ) ;
    } // if
    else {
      for ( int i = 0 ; i < spacenum ; i++ )
        printf( " " ) ;
      Printtoken( tokentree.find( point )->second ) ;
      printf( "\n" ) ;
    } // else

  } // Printtree()
  
};

int main() {
  Scanner scanner = Scanner() ;
  char t ;
  scanf( "%d",  &gTestNum  ) ;
  scanf( "%c",  &t ) ;
  printf( "Welcome to OurScheme!\n\n" ) ;
  try {
    scanner.Gettokenlist() ;
  } // try
  catch ( Callend e ) {
    ;
  } // catch
  catch ( EndOfFileError e ) {
    printf( "%s\n", e.merrorstr.c_str() ) ;
  } // catch  
  
  // scanner.Print() ;
  printf( "Thanks for using OurScheme!\n" ) ;
  
} // main()
