# include <cstdlib>
# include <vector>
# include <iostream>
# include <stdio.h>
# include <string>
# include <map>
# include <sstream>
# include <iomanip>
# include <math.h>

 
using namespace std;

enum Type { LPAREN, RPAREN, INT, STRING, DOT, FLOAT, NIL, T, QUOTE, SYMBOL, COMMENT, UNKNOWN };

int gLine = 1 ;              // 「下一個要讀進來的字元」所在的line number
int gColumn = 0 ;            // 「下一個要讀進來的字元」所在的column number
bool gReading = false ;
int gTestNum ;
bool gEnd = false ;

struct Token {
  string str ;
  
  int line ;
  int column ;
  int intnum ;
  float floatnum ;
  bool iscomd ;
  Type type ;
  
  Token * left ;
  Token * right ;
};

struct Symbol {

  string name ;
  Token * args ;
  Token * info ;
  
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
  else if ( Isdigit( str[0] ) ) hasdigit = true ;
  
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

bool Isend( map< int, Token > tokentree ) {
  if ( tokentree.find( 2 ) == tokentree.end() || tokentree.find( 3 ) == tokentree.end() )
    return false ;
  else if ( tokentree.find( 2 )->second.str == "exit" && tokentree.find( 3 )->second.type == NIL )
    return true ;

  else return false ;  
  
} // Isend()

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

string Tofloat( float num ) {
  stringstream ss ;
  ss << num ; 
  if ( ss.str().find( "." ) != -1 )
    return ss.str() ;
  else {
    ss << "." ;
    return ss.str() ;
  } // else
} // Tofloat()

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

bool Isinternalfunc( string str ) {
  if ( str == "cons" || str == "list" || str == "car" ) 
    return true ;
  else if ( str == "cdr" || str == "atom?" || str == "pair?" ) 
    return true ;
  else if ( str == "list?" || str == "null?" || str == "integer?" ) 
    return true ;
  else if ( str == "real?" || str == "number?" || str == "string?" ) 
    return true ;
  else if ( str == "boolean?" || str == "symbol?" || str == "+" ) 
    return true ;
  else if ( str == "-" || str == "*" || str == "/" ) 
    return true ;
  else if ( str == "not" || str == ">" || str == ">=" ) 
    return true ;
  else if ( str == "<" || str == "<=" || str == "=" ) 
    return true ;
  else if ( str == "string-append" || str == "string>?" || str == "string<?" ) 
    return true ;
  else if ( str == "string=?" || str == "eqv?" || str == "equal?" ) 
    return true ;
  else if ( str == "exit" ) 
    return true ;
  else 
    return false ;
} // Isinternalfunc()

bool Isspfunc( string str ) {
  if ( str == "quote" || str == "\'" || str == "define" ) 
    return true ;
  else if ( str == "and" || str == "or" || str == "begin" ) 
    return true ;
  else if ( str == "if" || str == "cond" || str == "clean-environment" ) 
    return true ;
  else if ( str == "exit" ) 
    return true ;
  else 
    return false ;
} // Isspfunc()

bool Isprimitivepredicates( string str ) {
  if ( str == "atom?" ) return true ;
  else if ( str == "pair?" ) return true ;
  else if ( str == "list?" ) return true ;
  else if ( str == "null?" ) return true ;
  else if ( str == "integer?" ) return true ;
  else if ( str == "real?" ) return true ;
  else if ( str == "number?" ) return true ;
  else if ( str == "string?" ) return true ;
  else if ( str == "boolean?" ) return true ;
  else if ( str == "symbol?" ) return true ;
  else return false ;
} // Isprimitivepredicates()

bool IsArith( string str ) {
  if ( str == "atom?" ) return true ;
  else if ( str == "+" ) return true ;
  else if ( str == "-" ) return true ;
  else if ( str == "*" ) return true ;
  else if ( str == "/" ) return true ;
  // else if ( str == "not" ) return true ;
  else if ( str == "and" ) return true ;
  else if ( str == "or" ) return true ;
  else if ( str == ">" ) return true ;
  else if ( str == ">=" ) return true ;
  else if ( str == "<" ) return true ;
  else if ( str == "<=" ) return true ;
  else if ( str == "=" ) return true ;
  else if ( str == "string-append" ) return true ;
  else if ( str == "string>?" ) return true ;
  else if ( str == "string<?" ) return true ;
  else if ( str == "string=?" ) return true ;

  else return false ;
} // IsArith()

float Getnum( Token * temp ) {
  if ( temp->type == INT )
    return ( float ) temp->intnum ;
  else if ( temp->type == FLOAT )
    return temp->floatnum ; 
  else return 0.0 ;

} // Getnum()

class Exception {
public:
  string merrorstr;
  Token * mhead ;
  
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
    
    else { // 2 "  "
      ss << "ERROR (no closing quote) : END-OF-LINE encountered at Line " ;
      ss << temp.line << " Column " << temp.column + 1 << "" ;
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

    ss << "ERROR (no more input) : END-OF-FILE encountered" ;

    merrorstr = ss.str() ;
  } // EndOfFileError()
};

class ArgNumError : public Exception {
public:
  ArgNumError( string str ) {
    stringstream ss ;

    ss << "ERROR (incorrect number of arguments) : " << str ;

    merrorstr = ss.str() ;

  } // ArgNumError()
};

class UnboundError : public Exception {
public:
  UnboundError( string str ) {
    stringstream ss ;

    ss << "ERROR (unbound symbol) : " << str ;

    merrorstr = ss.str() ;
  } // UnboundError()
};

class NonFuncError : public Exception {
public:
  NonFuncError( Token * temp ) {
    stringstream ss ;

    ss << "ERROR (attempt to apply non-function) : " ;

    merrorstr = ss.str() ;
    mhead = temp ;
  } // NonFuncError()
};

class NonListError : public Exception {
public:
  NonListError() {
    stringstream ss ;

    ss << "ERROR (non-list) : " ;

    merrorstr = ss.str() ;
  } // NonListError()
};

class LevelError : public Exception {
public:
  LevelError( string str ) {
    stringstream ss ;

    ss << "ERROR (level of " << str << ")" ;

    merrorstr = ss.str() ;
  } // LevelError()
};

class FormatError : public Exception {
public:
  FormatError( string str ) {
    stringstream ss ;

    ss << "ERROR (" << str << " format) : " ;

    merrorstr = ss.str() ;
  } // FormatError()
};

class ArgTypeError : public Exception {
public:
  ArgTypeError( string str, Token * temp ) {
    stringstream ss ;

    ss << "ERROR (" << str << " with incorrect argument type) : " ;

    merrorstr = ss.str() ;
    mhead = temp ;
  } // ArgTypeError()
};

class DivideZeroError : public Exception {
public:
  DivideZeroError() {
    stringstream ss ;

    ss << "ERROR (division by zero) : /" ;

    merrorstr = ss.str() ;
  } // DivideZeroError()
};

class NoReturnError : public Exception {
public:
  NoReturnError( Token * temp ) {
    stringstream ss ;

    ss << "ERROR (no return value) : " ;
    mhead = temp ;
    merrorstr = ss.str() ;
  } // NoReturnError()
};

class ReE : public Exception {
public:
  ReE() {
    ;
  } // ReE()
};

class Printer {
  private:
  void Printtoken( Token * token ) {
    Type type = token->type ;

    if ( type == INT )
      printf( "%d", token->intnum ) ;
    else if ( type == FLOAT )
      printf( "%.3f", token->floatnum ) ;
    else if ( type == QUOTE )
      printf( "%s", "quote" ) ;
    else if ( token->iscomd )
      printf( "#<procedure %s>", token->str.c_str() ) ;      
    else 
      printf( "%s", token->str.c_str() ) ;
         
  } // Printtoken()
  
  public:
  void Printtree( Token * tokentree ) {
    Token * temp = tokentree ;
    if ( Isatomtype( temp->type ) ) {
      Printtoken( temp );
      printf( "\n" );
    } // if
    else  
      PrintRe( temp, 0 ) ;
  } // Printtree()
  
  void PrintRe( Token * temp, int spacenum ) {

    Type type = temp->type ;

    if ( type == DOT && temp->left->str == "lambda" ) {
      Printtoken( temp->left ) ;
      printf( "\n" ) ;
    } // if
    else if ( type == DOT ) {

      printf( "( " ) ;
      
      type = temp->left->type ;

      if ( type == DOT )
        PrintRe( temp->left, spacenum + 2 ) ;
      else {

        Printtoken( temp->left ) ;  
        printf( "\n" ) ;
      } // else
                  
      temp = temp->right ;
      while ( temp != NULL ) {
        type = temp->type ;
        if ( type == DOT ) {
          type = temp->left->type ;
            
          if ( type == DOT ) {
            for ( int i = 0 ; i < spacenum + 2 ; i++ )
              printf( " " ) ;
            PrintRe( temp->left, spacenum + 2 ) ;
          } // if
          else {
              
            for ( int i = 0 ; i < spacenum + 2 ; i++ )
              printf( " " ) ;
                
            Printtoken( temp->left ) ;
            printf( "\n" ) ;
          } // else
        } // if
        else {
          type = temp->type ;
          if ( type != NIL ) {
            for ( int i = 0 ; i < spacenum + 2 ; i++ )
              printf( " " ) ;
            
            printf( "." ) ;        
            // .Printtoken( Maketoken( "." ) ) ; 
            printf( "\n" ) ;  
          } // if
             
          PrintRe( temp, spacenum + 2 ) ;
        } // else
            
        temp = temp->right ;

      } // while
        
      for ( int i = 0 ; i < spacenum ; i++ )
        printf( " " ) ;
          
      printf( ")\n" ) ;
      
    } // else if
    else {
      if ( type == NIL ) 
        ;
      else {
        for ( int i = 0 ; i < spacenum ; i++ )
          printf( " " ) ;
        Printtoken( temp ) ;
        printf( "\n" ) ;  
      } // else

    } // else

  } // PrintRe()  
  
  
};

class Evaler {
  private:
    
  vector< Symbol > msymbollist ;
    
  Token * NewToken( string str ) {
    Token * retoken = new Token ;
    retoken->str = Setstr( str ) ;
    retoken->type = Gettype( retoken->str ) ;
    retoken->intnum = Decodeint( retoken->str ) ;
    retoken->floatnum = Decodefloat( retoken->str ) ;
    retoken->iscomd = false ;
    retoken->left = NULL ;
    retoken->right = NULL ;
      
    return retoken ;
  } // NewToken()
    
  int Getsize( Token * temp ) {
    if ( temp == NULL || temp->type == NIL )
      return 0 ;
    else
      return 1 + Getsize( temp->right ) ;
    
  } // Getsize()
  
  int Findsymbol( string str ) {
    int i = msymbollist.size() - 1 ;
    
    while ( i >= 0 ) {
      if ( msymbollist.at( i ).name == str ) {
        if ( msymbollist.at( i ).args == NULL )
          return 1 ;
        else 
          return 2 ;

      } // if
        
      i-- ;  
      
    } // while
    
    return 0 ;
  } // Findsymbol()
  
  Token * Interfunc( Token * temp ) {
    stringstream ss ;
    ss << "#<procedure " << temp->str << ">" ;
    return NewToken( ss.str() ) ;
  } // Interfunc()
  
  Token * Exit( Token * temp ) {

    if ( Getsize( temp ) != 0 )
      throw ArgNumError( "exit" ) ;
    else if ( Getsize( temp ) == 0 )
      throw Callend() ;
  
    return NewToken( "exit" ) ;
  } // Exit()

  Token * Copytoken( Token * temp ) {
    if ( temp != NULL ) {
      Token * ntemp = NewToken( temp->str ) ;
      ntemp->left = Copytoken( temp->left ) ;
      ntemp->right = Copytoken( temp->right ) ;
      return ntemp ;
    } // if
    else 
      return NULL ;
  } // Copytoken()
  
  Token * Symbols( Token * temp ) {
    
    int i = msymbollist.size() - 1 ;
    bool find = false ;
    while ( i >= 0 ) {

      if ( msymbollist.at( i ).name == temp->str )  {
        find = true ;
        
        Token * retoken = msymbollist.at( i ).info ;
        return retoken ;
      } // if
      
    
      i-- ;  
      
    } // while
    
    if ( !find ) {
    
      throw UnboundError( temp->str ) ;
    
    } // if 
    
    return NULL ;
  } // Symbols()

  void Change( Token * temp ) {
    if ( temp != NULL ) {
      if ( temp->left != NULL && Findsymbol( temp->left->str ) == 1 ) {
        temp->left = Copytoken( Symbols( temp->left ) ) ;
      } // if

      if ( temp->right != NULL && Findsymbol( temp->right->str ) == 1 ) {
        temp->right = Copytoken( Symbols( temp->right ) ) ;
      } // if    

      Change( temp->left ) ;
      Change( temp->right ) ;
    } // if

  }  // Change()

  Token * Define( Token * temp ) {
    
    if ( Getsize( temp ) != 2 )
      throw FormatError( "DEFINE" ) ;

    if ( temp->left->type == DOT )
      return Definefunc( temp ) ;    
    
    Symbol newsymbol ;
    string name = temp->left->str ;
    newsymbol.name = name ;
    newsymbol.args = NULL ;
    
    
    if ( Isinternalfunc( name ) || Isspfunc( name ) || temp->left->type != SYMBOL ) { 
      throw FormatError( "DEFINE" ) ;
    } // if
    
    if ( Isatomtype( temp->right->left->type ) && temp->right->left->type != SYMBOL ) {
      Token * check = Copytoken( temp->right->left ) ;
      Change( check ) ;
      newsymbol.info = check ;
    } // if
    else if ( temp->right->left->type == DOT && temp->right->left->left->str == "lambda" ) { 
      temp->right->left->left->iscomd = true ;
      newsymbol.info = temp->right->left ;
    } // else if
    else { 
      newsymbol.info = Evalexp( temp->right->left, 1 ) ;
    } // else 
      

    if ( Findsymbol( name ) == 0 )
      msymbollist.push_back( newsymbol ) ;  
    else {
      for ( int i = msymbollist.size() - 1 ; i >= 0 ; i-- ) {

        if ( msymbollist.at( i ).name == name ) {

          msymbollist.at( i ).info = newsymbol.info ;
        } // if
      } // for

    } // else

    return NewToken( name + " defined" ) ;
  } // Define()

  Token * Definefunc( Token * temp ) {
    
    if ( Getsize( temp ) != 2 )
      throw FormatError( "DEFINE" ) ;  
    
    Symbol newsymbol ;
    string name = temp->left->left->str ;
    newsymbol.name = name ;
    newsymbol.args = temp->left->right ;
    
    if ( Isinternalfunc( name ) || Isspfunc( name ) || temp->left->left->type != SYMBOL ) { 
      throw FormatError( "DEFINE" ) ;
    } // if
    
    if ( Isatomtype( temp->right->left->type ) && temp->right->left->type != SYMBOL ) {
      Token * check = Copytoken( temp->right->left ) ;
      Change( check ) ;
      newsymbol.info = check ;
    } // if
    else { 
      newsymbol.info = temp->right->left ;
      
    } // else 
      

    if ( Findsymbol( name ) == 0 )
      msymbollist.push_back( newsymbol ) ;  
    else {
      for ( int i = msymbollist.size() - 1 ; i >= 0 ; i-- ) {

        if ( msymbollist.at( i ).name == name ) {
          msymbollist.at( i ).info = newsymbol.info ;
          msymbollist.at( i ).args = newsymbol.args ;
          
        } // if
      } // for

    } // else

    return NewToken( name + " defined" ) ;
  } // Definefunc()
  
  Token * Customfunc( Token * temp ) {
    int i = msymbollist.size() - 1 ;
    string str = temp->left->str ;
    bool find = false ;
    Token * argname ;
    Token * method ;
    Token * args ;
    while ( i >= 0 && !find ) {
      if ( msymbollist.at( i ).name == str )  {
        argname = msymbollist.at( i ).args ;
        method = msymbollist.at( i ).info ;
        find = true ;
      } // if
      
      i-- ;  
    } // while
    
    args = temp->right ;
    int argsnum = Getsize( argname ) ;
    
    if ( argsnum != Getsize( args ) )
      throw ArgNumError( str ) ;
      
    vector < Symbol > templist ;  
    while ( argname->type != NIL ) {
      Symbol sym ;
      sym.name = argname->left->str ;
      sym.args = NULL ;
      sym.info = Evalexp( args->left, 1 ) ;
      templist.push_back( sym ) ;
      
      argname = argname->right ;
      args = args->right ;
    } // while  
    
    for ( int i = 0 ; i < templist.size() ; i++ ) {
      msymbollist.push_back( templist.at( i ) ) ;
    } // for

    Token * retoken = Evalexp( method, 1 ) ;
    
    for ( int i = 0 ; i < argsnum ; i++ ) {
      msymbollist.pop_back() ;
    } // for
    
    return retoken ;
  } // Customfunc()
  
  Token * Lambda( Token * temp ) {

    Token * ltemp = temp->left->right ;
    Token * argname = ltemp->left ;
    Token * method = ltemp->right ;
    Token * args = temp->right ;

    int argsnum = Getsize( argname ) ;
    
    if ( argsnum != Getsize( args ) )
      throw ArgNumError( "lambda" ) ;
      
    vector < Symbol > templist ;  
    while ( argname->type != NIL ) {
      Symbol sym ;
      sym.name = argname->left->str ;
      sym.args = NULL ;
      sym.info = Evalexp( args->left, 1 ) ;
      templist.push_back( sym ) ;
      
      argname = argname->right ;
      args = args->right ;
    } // while  
    
    for ( int i = 0 ; i < templist.size() ; i++ ) {
      msymbollist.push_back( templist.at( i ) ) ;
    } // for


    Token * result ;

    while ( method != NULL && method->type != NIL ) {

      result = Evalexp( method->left, 1 ) ;
      method = method->right ;
    } // while
    
    for ( int i = 0 ; i < argsnum ; i++ ) {
      msymbollist.pop_back() ;
    } // for
    
    return result ;
  } // Lambda()
  
  Token * Let( Token * temp ) {
    Token * args = temp->left ;
    int argsnum = Getsize( args ) ;
    vector < Symbol > templist ;  
    
    while ( args->type != NIL ) {
      Symbol sym ;
      Token * argtemp = args->left ;
      sym.name = argtemp->left->str ;
      sym.args = NULL ;
      sym.info = Evalexp( argtemp->right->left, 1 ) ;
      templist.push_back( sym ) ;
      
      args = args->right ;
    } // while  
    
    for ( int i = 0 ; i < templist.size() ; i++ ) {
      msymbollist.push_back( templist.at( i ) ) ;
    } // for

    
    Token * t = temp->right ;
    Token * result ;

    while ( t != NULL && t->type != NIL ) {

      result = Evalexp( t->left, 1 ) ;
      t = t->right ;
    } // while

    for ( int i = 0 ; i < argsnum ; i++ ) {
      msymbollist.pop_back() ;
    } // for

    return result ;

  } // Let()

  Token * Quote( Token * temp ) {
    return temp->left ; 
  } // Quote()

  Token * Cons( Token * temp ) {
    if ( Getsize( temp ) != 2 )
      throw ArgNumError( "cons" ) ;
    
    Token * retoken = NewToken( "." ) ;
    retoken->left = Evalexp( temp->left, 1 ) ;
    retoken->right = Evalexp( temp->right->left, 1 );
    return retoken ;  
  } // Cons()

  Token * List( Token * temp ) {
    if ( Getsize( temp ) == 0 )
      return NewToken( "nil" ) ;
    else if ( !Islist( temp ) ) {
      throw NonListError() ;
    } // else if
    // not done
    Token * retoken = NewToken( "." ) ;
    Token * ret = retoken ;
    Token * t = temp ;

    while ( t->type != NIL ) {
      ret->left = Evalexp( t->left, 1 ) ; 
    
      if ( t->right->type != NIL )
        ret->right = NewToken( "." ) ;
      else 
        ret->right = NewToken( "nil" ) ;
    
      ret = ret->right ;
      t = t->right ;

    } // while
    
    return retoken ;  
  } // List()

  Token * Car( Token * temp ) {
    if ( Getsize( temp ) != 1 )
      throw ArgNumError( "car" ) ;
    Token * check = Evalexp( temp->left, 1 ) ;
    
    if ( Isatomtype( check->type ) )
      throw ArgTypeError( "car", check ) ;
      
    return check->left ;
    
  } // Car()

  Token * Cdr( Token * temp ) {
    if ( Getsize( temp ) != 1 )
      throw ArgNumError( "cdr" ) ;
    Token * check = Evalexp( temp->left, 1 ) ;
    
    if ( Isatomtype( check->type ) )
      throw ArgTypeError( "cdr", check ) ;
      
    return check->right ;
    
  } // Cdr()
  
  Token * Not( Token * temp ) {
    if ( Getsize( temp ) != 1 )
      throw ArgNumError( "not" ) ;
    

    if ( Evalexp( temp->left, 1 )->type == NIL )     
      return NewToken( "#t" ) ;
    else 
      return NewToken( "nil" ) ;
    
  } // Not()
  
  Token * Greater( Token * temp ) {
    string check = "#t" ;
    while ( temp->right->type != NIL && temp->right != NULL ) {
      Token * check1 = Evalexp( temp->left, 1 ) ;
      Token * check2 = Evalexp( temp->right->left, 1 ) ;
      if ( check1->type != INT && check1->type != FLOAT )
        throw ArgTypeError( ">", check1 ) ;
      else if ( check2->type != INT && check2->type != FLOAT )
        throw ArgTypeError( ">", check2 ) ;

      if ( Getnum( check1 ) <= Getnum( check2 ) )
        check = "nil" ;
      
      temp = temp->right ;
    } // while

    return NewToken( check ) ;

  } // Greater()

  Token * Less( Token * temp ) {
    string check = "#t" ;
    while ( temp->right->type != NIL && temp->right != NULL ) {
      Token * check1 = Evalexp( temp->left, 1 ) ;
      Token * check2 = Evalexp( temp->right->left, 1 ) ;
      if ( check1->type != INT && check1->type != FLOAT )
        throw ArgTypeError( "<", check1 ) ;
      else if ( check2->type != INT && check2->type != FLOAT )
        throw ArgTypeError( "<", check2 ) ;

      if ( Getnum( check1 ) >= Getnum( check2 ) )
        check = "nil" ;
      
      temp = temp->right ;
    } // while

    return NewToken( check ) ;

  } // Less()  

  Token * Nogreater( Token * temp ) {
    string check = "#t" ;
    while ( temp->right->type != NIL && temp->right != NULL ) {
      Token * check1 = Evalexp( temp->left, 1 ) ;
      Token * check2 = Evalexp( temp->right->left, 1 ) ;
      if ( check1->type != INT && check1->type != FLOAT )
        throw ArgTypeError( "<=", check1 ) ;
      else if ( check2->type != INT && check2->type != FLOAT )
        throw ArgTypeError( "<=", check2 ) ;

      if ( Getnum( check1 ) > Getnum( check2 ) )
        check = "nil" ;
      
      temp = temp->right ;
    } // while

    return NewToken( check ) ;

  } // Nogreater()

  Token * Noless( Token * temp ) {
    string check = "#t" ;
    while ( temp->right->type != NIL && temp->right != NULL ) {
      Token * check1 = Evalexp( temp->left, 1 ) ;
      Token * check2 = Evalexp( temp->right->left, 1 ) ;
      if ( check1->type != INT && check1->type != FLOAT )
        throw ArgTypeError( ">=", check1 ) ;
      else if ( check2->type != INT && check2->type != FLOAT )
        throw ArgTypeError( ">=", check2 ) ;

      if ( Getnum( check1 ) < Getnum( check2 ) )
        check = "nil" ;
      
      temp = temp->right ;
    } // while

    return NewToken( check ) ;

  } // Noless()
  
  Token * Equalnum( Token * temp ) {
    string check = "#t" ;
    while ( temp->right->type != NIL && temp->right != NULL ) {
      Token * check1 = Evalexp( temp->left, 1 ) ;
      Token * check2 = Evalexp( temp->right->left, 1 ) ;
  
      if ( check1->type != INT && check1->type != FLOAT )
        throw ArgTypeError( "=", check1 ) ;
      else if ( check2->type != INT && check2->type != FLOAT )
        throw ArgTypeError( "=", check2 ) ;
      if ( Getnum( check1 ) != Getnum( check2 ) )
        check = "nil" ;
      
      temp = temp->right ;
    } // while

    return NewToken( check ) ;

  } // Equalnum()

  Token * Strappend( Token * temp ) {
    string check = "" ;
    while ( temp->type != NIL && temp != NULL ) {
      Token * nstr = Evalexp( temp->left, 1 ) ;
      if ( nstr->type != STRING )
        throw ArgTypeError( "string-append", nstr ) ;

      check = Mergestr( check, nstr->str ) ;
      temp = temp->right ;
    } // while

    return NewToken( check ) ;

  } // Strappend()

  Token * Strgreat( Token * temp ) {
    string check = "#t" ;
    while ( temp->right->type != NIL && temp->right != NULL ) {
      Token * check1 = Evalexp( temp->left, 1 ) ;
      Token * check2 = Evalexp( temp->right->left, 1 ) ;
  
      if ( check1->type != STRING )
        throw ArgTypeError( "string>?", check1 ) ;
      else if ( check2->type != STRING )
        throw ArgTypeError( "string>?", check2 ) ;
      if ( check1->str.compare( check2->str ) <= 0 )
        check = "nil" ;
      
      temp = temp->right ;
    } // while

    return NewToken( check ) ;

  } // Strgreat()

  Token * Strless( Token * temp ) {
    string check = "#t" ;
    while ( temp->right->type != NIL && temp->right != NULL ) {
      Token * check1 = Evalexp( temp->left, 1 ) ;
      Token * check2 = Evalexp( temp->right->left, 1 ) ;
  
      if ( check1->type != STRING )
        throw ArgTypeError( "string<?", check1 ) ;
      else if ( check2->type != STRING )
        throw ArgTypeError( "string<?", check2 ) ;
      if ( check1->str.compare( check2->str ) >= 0 )
        check = "nil" ;
      
      temp = temp->right ;
    } // while

    return NewToken( check ) ;

  } // Strless()

  Token * Strequal( Token * temp ) {
    string check = "#t" ;
    while ( temp->right->type != NIL && temp->right != NULL ) {
      Token * check1 = Evalexp( temp->left, 1 ) ;
      Token * check2 = Evalexp( temp->right->left, 1 ) ;
  
      if ( check1->type != STRING )
        throw ArgTypeError( "string=?", check1 ) ;
      else if ( check2->type != STRING )
        throw ArgTypeError( "string=?", check2 ) ;
      if ( check1->str.compare( check2->str ) != 0 )
        check = "nil" ;
      
      temp = temp->right ;
    } // while

    return NewToken( check ) ;

  } // Strequal()

  Token * Eqv( Token * temp ) {
    if ( Getsize( temp ) != 2 )
      throw ArgNumError( "eqv?" ) ;
    Token * check1 = Evalexp( temp->left, 1 ) ;
    Token * check2 = Evalexp( temp->right->left, 1 ) ;
    if ( Isatomtype( check1->type ) && Isatomtype( check2->type ) && 
         check1->type != STRING && check2->type != STRING ) {
      if ( check1->str.compare( check2->str ) == 0 )
        return NewToken( "#t" ) ;
      else 
        return NewToken( "nil" ) ;
    } // if
    else {
      if ( check1 == check2 )
        return NewToken( "#t" ) ;
      else 
        return NewToken( "nil" ) ;
    } // else

    

  } // Eqv()

  bool Issame( Token * t1, Token * t2 ) {
    if ( t1 == NULL && t2 == NULL )
      return true ;
    else if ( t1->str.compare( t2->str ) != 0 )
      return false ;
    else 
      return Issame( t1->left, t2->left ) && Issame( t1->right, t2->right ) ;

  } // Issame()

  Token * Equal( Token * temp ) {
    if ( Getsize( temp ) != 2 )
      throw ArgNumError( "equ?" ) ;
    
    Token * check1 = Evalexp( temp->left, 1 ) ;
    Token * check2 = Evalexp( temp->right->left, 1 ) ;
    if ( Issame( check1, check2 ) ) 
      return NewToken( "#t" ) ;
    else 
      return NewToken( "nil" ) ;

  } // Equal()
  
  Token * If( Token * temp ) {
    int size = Getsize( temp ) ;
    if ( size != 2 && size != 3 )
      throw ArgNumError( "if" ) ;
    
    Token * check = Evalexp( temp->left, 1 ) ;
    Token * result ;

    if ( check->type != NIL ) {
      result = Evalexp( temp->right->left, 1 ) ;
    } // if
    else {
      if ( size == 2 )
        throw ReE() ;
      else {
        result = Evalexp( temp->right->right->left, 1 ) ;
      } // else

    } // else

    return result ;

  } // If()

  Token * Decide( Token * temp, bool last, bool & done ) {

    if ( Getsize( temp ) < 1 )
      throw FormatError( "COND" ) ;
    else if ( Isatomtype( temp->type ) ) {
      throw FormatError( "COND" ) ;
    } // else if
    else if ( ( last && temp->left->str == "else" ) ) {
      done = true ;
      return Begincond( temp->right ) ;
    } // if

    Token * check = Evalexp( temp->left, 1 );
    Token * result ;

    if ( check->type != NIL ) {
      done = true ;
      return Begincond( temp->right ) ;
    } // if
    else {
      done = false ;
      return NewToken( "nil" ) ;
    } // else

  } // Decide()

  Token * Cond( Token * temp ) {
    int size = Getsize( temp ) ;
    if ( Getsize( temp ) < 1 )
      throw FormatError( "COND" ) ;
    
    Token * t = temp ;
    bool check, done ;
    Token * result ;

    while ( t->type != NIL ) {
      if ( Isatomtype( t->left->type ) ) {
        throw FormatError( "COND" ) ;
      } // if
      else if ( Getsize( t->left ) < 2 ) {
        throw FormatError( "COND" ) ;
      } // else if

      t = t->right ;
    } // while

    t = temp ;

    while ( t->type != NIL ) {
      if ( t->right->type == NIL ) {
        result = Decide( t->left, true, done ) ;
      } // if
      else {
        result = Decide( t->left, false, done ) ; 
      } // else
        

      
      if ( done ) {
        return result ;
      } // if

      t = t->right ;
    } // while

    throw ReE() ;

  } // Cond()
  
  Token * Begin( Token * temp ) {
    int size = Getsize( temp ) ;
    if ( Getsize( temp ) < 1 )
      throw ArgNumError( "begin" ) ;
    
    Token * t = temp ;
    bool check, done ;
    Token * result ;

    while ( t != NULL && t->type != NIL ) {

      result = Evalexp( t->left, 1 ) ;
      t = t->right ;
    } // while

    return result ;

  } // Begin()

  Token * Begincond( Token * temp ) {
    int size = Getsize( temp ) ;
    if ( Getsize( temp ) < 1 )
      throw FormatError( "COND" ) ;
    
    Token * t = temp ;
    bool check, done ;
    Token * result ;

    while ( t != NULL && t->type != NIL ) {

      result = Evalexp( t->left, 1 ) ;
      t = t->right ;
    } // while

    return result ;

  } // Begincond()

  Token * Or( Token * temp ) {
    int size = Getsize( temp ) ;
    if ( Getsize( temp ) < 2 )
      throw ArgNumError( "Or" ) ;
    
    Token * t = temp ;
    bool check, done ;
    Token * result ;
    while ( t != NULL && t->type != NIL ) {

      result = Evalexp( t->left, 1 ) ;
      if ( result->type != NIL )
        return result ;

      t = t->right ;
    } // while

    return result ;

  } // Or()

  Token * And( Token * temp ) {
    int size = Getsize( temp ) ;
    if ( Getsize( temp ) < 2 )
      throw ArgNumError( "And" ) ;
    
    Token * t = temp ;
    bool check, done ;
    Token * result ;

    while ( t != NULL && t->type != NIL ) {

      result = Evalexp( t->left, 1 ) ;
      if ( result->type == NIL )
        return result ;
      t = t->right ;
    } // while

    return result ;

  } // And()

  Token * Primitivepredicates( Token * temp, string str ) {
    if ( Getsize( temp ) != 1 )
      throw ArgNumError( str ) ;
    
    if ( str == "atom?" ) {
      if ( Evalexp( temp->left, 1 )->type != DOT )
        return NewToken( "#t" ) ;
      else return NewToken( "nil" ) ;
    } // if
    else if ( str == "pair?" ) {

      Token * check = Evalexp( temp->left, 1 ) ;

      if ( check->type == DOT )
        return NewToken( "#t" ) ;
      else return NewToken( "nil" ) ;
    } // else if
    else if ( str == "list?" ) {
      Token * check = Evalexp( temp->left, 1 ) ;
      while ( check->right != NULL ) 
        check = check->right ;    
      if ( check->type == NIL )
        return NewToken( "#t" ) ;
      else return NewToken( "nil" ) ;
    } // else if
    else if ( str == "null?" ) {
      Token * check = Evalexp( temp->left, 1 ) ;
      if ( check->type == NIL )
        return NewToken( "#t" ) ;
      else return NewToken( "nil" ) ;
    } // else if
    else if ( str == "integer?" ) {
      Token * check = Evalexp( temp->left, 1 ) ;
      if ( check->type == INT )
        return NewToken( "#t" ) ;
      else return NewToken( "nil" ) ;
    } // else if
    else if ( str == "real?" ) {
      Token * check = Evalexp( temp->left, 1 ) ;
      if ( check->type == INT || check->type == FLOAT )
        return NewToken( "#t" ) ;
      else return NewToken( "nil" ) ;
    } // else if
    else if ( str == "number?" ) {
      Token * check = Evalexp( temp->left, 1 ) ;
      if ( check->type == INT || check->type == FLOAT )
        return NewToken( "#t" ) ;
      else return NewToken( "nil" ) ;
    } // else if
    else if ( str == "string?" ) {
      Token * check = Evalexp( temp->left, 1 ) ;
      if ( check->type == STRING )
        return NewToken( "#t" ) ;
      else return NewToken( "nil" ) ;
    } // else if
    else if ( str == "boolean?" ) {
      Token * check = Evalexp( temp->left, 1 ) ;
      if ( check->type == NIL || check->type == T )
        return NewToken( "#t" ) ;
      else return NewToken( "nil" ) ;
    } // else if
    else if ( str == "symbol?" ) {
      Token * check = Evalexp( temp->left, 1 ) ;
      if ( check->type == SYMBOL )
        return NewToken( "#t" ) ;
      else return NewToken( "nil" ) ;
    } // else if
    else return NewToken( "nil" ) ;

  } // Primitivepredicates()

  Token * Arith( Token * temp, string str ) {
    if ( Getsize( temp ) < 2 )
      throw ArgNumError( str ) ;
    stringstream ss;
    bool isfloat = false ;
    if ( str == "+" ) {
      float num = 0 ;
      num = Add( temp, isfloat ) ;
      if ( !isfloat )
        ss << ( int ) num ;
      else
        ss << Tofloat( num ) ;
      return NewToken( ss.str() ) ;
    } // if
    else if ( str == "-" ) {
      float num = 0 ;
      num = Sub( temp, isfloat ) ;
      if ( !isfloat )
        ss << ( int ) num ;
      else
        ss << Tofloat( num ) ;
      return NewToken( ss.str() ) ;
    } // else if
    else if ( str == "*" ) {
      float num = 0 ;
      num = Mul( temp, isfloat ) ;
      if ( !isfloat )
        ss << ( int ) num ;
      else
        ss << Tofloat( num ) ;
      return NewToken( ss.str() ) ;
    } // else if
    else if ( str == "/" ) {
      float num = 0 ;
      num = Div( temp, isfloat ) ;
      if ( !isfloat )
        ss << ( int ) num ;
      else
        ss << Tofloat( num ) ;
      return NewToken( ss.str() ) ;
    } // else if
    else if ( str == ">" ) {
      return Greater( temp ) ;
    } // else if
    else if ( str == "<" ) {
      return Less( temp ) ;
    } // else if
    else if ( str == ">=" ) {
      return Noless( temp ) ;
    } // else if
    else if ( str == "<=" ) {
      return Nogreater( temp ) ;
    } // else if
    else if ( str == "=" ) {
      return Equalnum( temp ) ;
    } // else if
    else if ( str == "string-append" ) {
      return Strappend( temp ) ;
    } // else if
    else if ( str == "string>?" ) {
      return Strgreat( temp ) ;
    } // else if
    else if ( str == "string<?" ) {
      return Strless( temp ) ;
    } // else if
    else if ( str == "string=?" ) {
      return Strequal( temp ) ;
    } // else if
    else if ( str == "or" ) {
      return Or( temp ) ;
    } // else if
    else if ( str == "and" ) {
      return And( temp ) ;
    } // else if
    else return NewToken( "nil" ) ;

  } // Arith()
  
  Token * Func( Token * temp ) {
    // string str = Symbols( temp->left )->str ;
    throw NonFuncError( Symbols( temp->left ) ) ;

  } // Func()
 
  float Add( Token * temp, bool & isfloat ) {
    if ( temp->left != NULL ) {
      Token * check = Evalexp( temp->left, 1 ) ;
      if ( check->type == FLOAT )
        isfloat = true ;
      else if ( check->type != INT && check->type != FLOAT )
        throw ArgTypeError( "+", check ) ;
      return Getnum( check ) + Add( temp->right, isfloat ) ;
    } // if
    else {
      float z = 0 ;
      return z ;
    } // else    
  } // Add()

  float Sub( Token * temp, bool & isfloat ) {
    if ( temp->left != NULL ) {
      Token * check = Evalexp( temp->left, 1 ) ;
      float num = Getnum( check ) ;
      if ( check->type == FLOAT )
        isfloat = true ;
      check = temp->right ;
      while ( check != NULL && check->type != NIL ) {
        Token * check2 = Evalexp( check->left, 1 ) ;
        if ( check2->type == FLOAT )
          isfloat = true ;
        else if ( check2->type != INT && check2->type != FLOAT )
          throw ArgTypeError( "-", check2 ) ;
        num -= Getnum( check2 ) ;

        check = check->right ;
        
      } // while

      return num ;

    } // if
    else {
      float z = 0 ;
      return z ;
    } // else    
  } // Sub()

  float Mul( Token * temp, bool & isfloat ) {
    if ( temp->left != NULL ) {
      Token * check = Evalexp( temp->left, 1 ) ;
      if ( check->type == FLOAT )
        isfloat = true ;
      else if ( check->type != INT && check->type != FLOAT )
        throw ArgTypeError( "*", check ) ;

      return Getnum( check ) * Mul( temp->right, isfloat ) ;
    } // if
    else {
      float z = 1 ;
      return z ;
    } // else   
  } // Mul()

  float Div( Token * temp, bool & isfloat ) {
    if ( temp->left != NULL ) {
      Token * check = Evalexp( temp->left, 1 ) ;
      float num = Getnum( check ) ;
      if ( check->type == FLOAT )
        isfloat = true ;
      check = temp->right ;
      while ( check != NULL && check->type != NIL ) {
        Token * check2 = Evalexp( check->left, 1 ) ;
        if ( check2->type == FLOAT )
          isfloat = true ;
        else if ( check2->type != INT && check2->type != FLOAT )
          throw ArgTypeError( "/", check2 ) ;

        float number = Getnum( check2 ) ;
        if ( number == 0 )
          throw DivideZeroError() ;        

        num /= number ;

        check = check->right ;
        
      } // while

      return num ;

    } // if
    else {
      float z = 0 ;
      return z ;
    } // else      
  } // Div()

  string Mergestr( string str1, string str2 ) {
    if ( str1.size() == 0 )
      return str2 ;      
    else if ( str2.size() == 0 )
      return str1 ;      
    else {
      str1.erase( str1.size() - 1, 1 ) ;
      str2.erase( 0, 1 ) ;
      return str1 + str2 ;
    } // else

    
  } // Mergestr()

  bool Islist( Token * temp ) {
    Token * check = temp ;
    while ( check->right != NULL ) 
      check = check->right ;    
    if ( check->type == NIL ) {
      // cout << "islist" ;
      return true ;
    } // if
    else {
      return false ;
    } // else
  } // Islist()

  Printer mprinter ;

  public:
  
  Token * Evalexp( Token * temp, int head ) {

    if ( Isatomtype( temp->type ) ) {
      if ( temp->type == SYMBOL ) {
        if ( Findsymbol( temp->str ) == 1 ) {
          Token * retoken = Symbols( temp ) ;
          return retoken ; 
          
          if (  retoken->left != NULL && retoken->left->str == "lambda" )
            return Evalexp( retoken, 1 ) ;
          else
            return retoken ;
          
        } // if
        else if ( Isinternalfunc( temp->str ) || Findsymbol( temp->str ) == 2 ) {
          temp->iscomd = true ;
          return temp ;
        }  // else if
        else 
          throw UnboundError( temp->str ) ;
      } // if
      else
        return temp ;
    } // if
    else if ( temp->left != NULL ) {
      if ( !Islist( temp ) ) {
        throw NonListError() ;
      } // if

      if ( temp->left->str == "clean-environment" ) {
        if ( head != 0 )
          throw LevelError( "CLEAN-ENVIRONMENT" ) ;
        // else if ( Getsize( temp->right ) != 0 )
        msymbollist.clear() ;
        return NewToken( "environment cleaned" ) ;  
      } // if
      else if ( temp->left->str == "define" ) {
        if ( head != 0 )
          throw LevelError( "DEFINE" ) ;
        return Define( temp->right ) ;    
      } // else if
      else if ( temp->left->str == "exit" ) {
        if ( head != 0 )
          throw LevelError( "EXIT" ) ;
        return Exit( temp->right ) ;    
      } // else if
      else if ( temp->left->type == QUOTE || temp->left->str == "quote" )
        return Quote( temp->right ) ;   
      else if ( temp->left->str == "cons" )
        return Cons( temp->right ) ;
      else if ( temp->left->str == "list" )
        return List( temp->right ) ;
      else if ( temp->left->str == "car" )
        return Car( temp->right ) ;  
      else if ( temp->left->str == "cdr" )
        return Cdr( temp->right ) ; 
      else if ( temp->left->str == "not" )
        return Not( temp->right ) ; 
      else if ( Isprimitivepredicates( temp->left->str ) ) 
        return Primitivepredicates( temp->right, temp->left->str ) ; 
      else if ( IsArith( temp->left->str ) ) 
        return Arith( temp->right, temp->left->str ) ; 
      else if ( temp->left->str == "eqv?" ) {
        return Eqv( temp->right ) ;
      } // else if
      else if ( temp->left->str == "equal?" ) {
        return Equal( temp->right ) ;
      } // else if
      else if ( temp->left->str == "if" ) {
        try {
          return If( temp->right ) ;
        } // try
        catch ( ReE e ) {
          throw NoReturnError( temp ) ;
        } // catch
        
      } // else if
      else if ( temp->left->str == "cond" ) {
        try {
          return Cond( temp->right ) ;
        } // try
        catch ( ReE e ) {
          throw NoReturnError( temp ) ;
        } // catch
      } // else if
      else if ( temp->left->str == "begin" ) {
        return Begin( temp->right ) ;
      } // else if
      else if ( temp->left->str == "lambda" ) {
        Token * re = NewToken( "lambda" ) ;
        temp->left->iscomd = true ;
        return temp ;
      } // else if
      else if ( temp->left->str == "let" ) {
        return Let( temp->right ) ;
      } // else if
      else if ( temp->left->type == SYMBOL ) {
        string str = temp->left->str ;
        int mode = Findsymbol( str ) ;
        Token * sym = Symbols( temp->left ) ;
        if ( mode != 0 ) {
          if ( mode == 2 ) {
            return Customfunc( temp ) ;
          } // if
          else if ( Findsymbol( sym->str ) == 2 ) {
            Token * ntemp = NewToken( "." ) ;
            ntemp->left = Symbols( temp->left ) ;
            ntemp->right = temp->right ;
            return Evalexp( ntemp, 1 ) ;
          } // else if
          else if ( Isinternalfunc( sym->str ) ) {
            Token * ntemp = NewToken( "." ) ;
            ntemp->left = Symbols( temp->left ) ;
            ntemp->right = temp->right ;
            
            return Evalexp( ntemp, 1 ) ;
          } // else if
          else if ( sym->left != NULL && sym->left->str == "lambda" ) {
            Token * ntemp = NewToken( "." ) ;
            ntemp->left = Symbols( temp->left ) ;
            ntemp->right = temp->right ;
            
            return Evalexp( ntemp, 1 ) ;
          } // else if
          else {
            throw NonFuncError( Symbols( temp->left ) ) ;
          } // else
        } // if
        else {
          // cout << Evalexp( temp->left )->str ;
          string str = temp->left->str ;
          throw UnboundError( str ) ;
          // return temp ;
        } // else
        
        
        return temp ;
      } // else if
      else if ( temp->left->type == DOT ) {
        if ( temp->left->left->str == "lambda" ) {
          return Lambda( temp ) ;
          
        } // if
        else {
          Token * check = Evalexp( temp->left, 1 ) ;
          temp->left = check ;
          if ( check->type == SYMBOL && !Isinternalfunc( check->str ) )
            throw NonFuncError( temp->left ) ;
          else if ( check->left != NULL && check->left->str != "lambda" )
            throw NonFuncError( temp->left ) ;

          return Evalexp( temp, 1 ) ;
        } // else

      } // else if
      else {
        throw NonFuncError( temp->left ) ;
      } // else
    } // else if         
    else 
      return temp ;
      
      
  } // Evalexp()
  
};

class Scanner {
  public:
  char Getch() {
    return mch ;
  } // Getch()
  
  void Getchar() {
    
    int eof = scanf( "%c", &mch ) ;
    if ( eof == EOF ) {

      gEnd = true ;
      mch = '\0' ;
    } // if
      
    gColumn++ ;
    if ( mch == '\n' ) {
      
      gColumn = 0 ;
      gLine++ ;
    } // if()
  
  } // Getchar() 
  
  void Readnwschar() {
    while ( ( mch == ' ' || mch == '\n' || mch == '\t' || mch == '\0' ) && !gEnd ) {
      Getchar() ;
    } // while()
    
  } // Readnwschar() 
  
  void Skipcomment() {
    while ( mch == ';' ) {
      Getchar() ;
      while ( mch != '\n' && !gEnd ) {
        Getchar() ;
      } // while
      
      Readnwschar() ;
    } // while
  } // Skipcomment() 
  
  void ReadSexp( vector<Token> & tokenlist ) {
    Token temp ;
    Readnwschar() ;
    temp = Gettoken() ;

    gReading = true ;
    if ( temp.type == LPAREN ) {

      tokenlist.push_back( temp ) ;
      ReadSexp( tokenlist ) ;
      
      Readnwschar() ;
      Skipcomment() ; 
      
      while ( mch != ')' && ! ( mch == '.' && Justdot() ) ) {
        ReadSexp( tokenlist ) ;
        Readnwschar() ;
        Skipcomment() ;
      } // while

      if ( mch == '.' && Justdot() ) {

        temp = Gettoken() ;

        tokenlist.push_back( temp ) ;
        ReadSexp( tokenlist ) ;
        Readnwschar() ;
      } // if
        
      Readnwschar() ;
      Skipcomment() ;

      if ( mch == ')' ) {
       
        temp = Gettoken() ;
        tokenlist.push_back( temp ) ;
      } // if
      else {

        temp = Gettoken() ;
        throw Stringerror( temp, 1 ) ;// error
      } // else
      
    } // if
    else if ( Isatomtype( temp.type ) ) {

      tokenlist.push_back( temp ) ;
    } // else if
    else if ( temp.type == QUOTE ) {
      tokenlist.push_back( temp ) ;
      ReadSexp( tokenlist ) ;
    } // else if 
    else {
      throw Stringerror( temp, 0 ) ;// error
    } // else
    
  } // ReadSexp()  
    
  private:  
  char mch ; 
  
  Token Gettoken() {
    Token retoken ;

    Readnwschar() ; 
    retoken.column = gColumn ;
    retoken.line = gLine ;
    retoken.str = Gettokenstr() ;
    while ( retoken.str == ";" ) {
      while ( mch != '\n' && !gEnd )
        Getchar() ;
      
      if ( gEnd )  
        throw EndOfFileError() ;
      else
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

    retoken.iscomd = false ;

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
    
    Token tk ;
    tk.line = gLine ;  
    tk.column = gColumn ;
    Getchar() ;

      
    while ( mch != '\"' && !gEnd )  {
      
      if ( mch == '\n' ) {

        throw Stringerror( tk, 2 ) ;
      } // if
      
      bool check = true ;
      if ( mch == '\\' ) {

        if ( gColumn > tk.column )
          tk.column = gColumn ;
          
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
        else if ( mch == '\n' ) {
          throw Stringerror( tk, 2 ) ;
        } // else if
        else {
          check = false ;
          temp += '\\' ;
        } // else
      } // if()
      else {
        temp += mch ;
        
      } // else
      
      if ( gColumn > tk.column )
        tk.column = gColumn ;
        
      if ( check )
        Getchar() ; 
        
      if ( gColumn > tk.column )
        tk.column = gColumn ;
          
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
      Skipcomment() ;
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
    
    while ( !Isseparators( mch ) && !gEnd ) {
      temp += mch ;
      Getchar() ; 
      
    } // while()

    if ( gEnd )
      throw EndOfFileError() ; 
      
    return temp ;
  } // Getothers() 
  
    
  
};

class Treemaker {
  public:
  void Buildtree( vector<Token> & tokenlist, map< int, Token > & tokentree ) {
    
    int point = 1 ;
    int index = 0 ;

    if ( tokenlist.size() == 1 ) {
      tokentree[1] =  tokenlist.at( 0 ) ;
    } // if
    else {
      Treerecursion( tokentree, tokenlist, point, index ) ;
    } // else
    
  
  } // Buildtree()
      
  private:    
  

  void Treerecursion( map< int, Token > & tokentree, vector<Token> tokenlist, int point, int & index ) {

    if ( tokenlist.at( index ).type == LPAREN ) {
      
      index++ ;
      Type type = tokenlist.at( index ).type ;
      while ( type != DOT && type != RPAREN ) {
        tokentree[ point ] = Maktoken( "." ) ;
        Treerecursion( tokentree, tokenlist, 2 * point, index ) ; 
        point = 2 * point + 1 ;
        type = tokenlist.at( index ).type ;
        
      } // while
        
      if ( tokenlist.at( index ).type == DOT ) {
        index++ ;  

        Treerecursion( tokentree, tokenlist, point, index ) ;
        index++ ; // problem
      } // if
      else {
        tokentree[ point ] = Maktoken( "nil" ) ;
        if ( index < tokenlist.size() - 1 ) {
          index++ ;
        } // if
      } // else

    } // if
    else if ( tokenlist.at( index ).type == QUOTE ) {
      
      tokentree[ point ] = Maktoken( "." ) ;
      
      tokentree[ 2 * point ] = tokenlist.at( index ) ;
      
      index++ ;
      
      point = 2 * point + 1 ;
      tokentree[ point ] = Maktoken( "." ) ;
      Treerecursion( tokentree, tokenlist, 2 * point, index ) ;
      tokentree[ 2 * point + 1 ] = Maktoken( "nil" ) ;
      
    } // else if
    else {
      tokentree[ point ] = tokenlist.at( index ) ;
      index++ ;
    } // else 
    
  } // Treerecursion()

  Token Maktoken( string str ) {
    Token retoken ;
    retoken.column = -1 ;
    retoken.line = -1 ;
    retoken.str = str ;
    retoken.iscomd = false ;
    retoken.type = Gettype( retoken.str ) ;
    retoken.str = Setstr( retoken.str ) ;
    if ( retoken.type == INT )
      retoken.intnum = Decodeint( retoken.str ) ;
    else if ( retoken.type == FLOAT )
      retoken.floatnum = Decodefloat( retoken.str ) ;
      
    return retoken ;
  } // Maktoken()
    
};

class Interpreter{
  public:
   
  
  Interpreter() {
    // mch = '\0' ;
    mprinter = Printer() ;
    mscanner = Scanner() ;
    mtreemaker = Treemaker() ;
  } // Interpreter() 
  
  void Gettokenlist() {
    
    while ( 0 == 0 ) {
      
      mtokenlist.clear() ;
      morigintree.clear() ;
      mtokentree = NULL ;
      
      bool err = false ;
      
      printf( "> " ) ;
      
      if ( gEnd )
        throw EndOfFileError() ;
      
      try {
        gLine = 1 ;  
        while ( Iswhitespace( mscanner.Getch() ) && mscanner.Getch() != '\n' && !gEnd )
          mscanner.Getchar() ; 
        
        mscanner.ReadSexp(  mtokenlist ) ;
        
      } // try
      catch ( Stringerror e ) {
        printf( "%s\n", e.merrorstr.c_str() ) ;
        gReading = false ;
        while ( !gEnd && mscanner.Getch() != '\n' )
          mscanner.Getchar() ;
          
        mtokenlist.clear() ;
        err = true ;
      } // catch
      
      gReading = false ;
      
      if ( !err ) {
        bool evalerr = false ;
        mtreemaker.Buildtree( mtokenlist, morigintree ) ; 
        mtokentree = SetTree( 1 ) ;
        Token * outtree ;
        try {
          
          outtree = mevaler.Evalexp( mtokentree, 0 ) ;
           
          
        }
        catch ( LevelError e ) {
          printf( "%s\n", e.merrorstr.c_str() ) ;
          evalerr = true ;
        } // catch
        catch ( ArgNumError e ) {
          printf( "%s\n", e.merrorstr.c_str() ) ;
          evalerr = true ;
        } // catch
        catch ( UnboundError e ) {
          printf( "%s\n", e.merrorstr.c_str() ) ;
          evalerr = true ;
        } // catch
        catch ( DivideZeroError e ) {
          printf( "%s\n", e.merrorstr.c_str() ) ;
          evalerr = true ;
        } // catch
        catch ( NonFuncError e ) {
          printf( "%s", e.merrorstr.c_str() ) ;
          evalerr = true ;
          mprinter.Printtree( e.mhead ) ; 
        } // catch
        catch ( FormatError e ) {
          printf( "%s", e.merrorstr.c_str() ) ;
          evalerr = true ;
          mprinter.Printtree( mtokentree ) ; 
        } // catch
        catch ( NonListError e ) {
          printf( "%s", e.merrorstr.c_str() ) ;
          evalerr = true ;
          mprinter.Printtree( mtokentree ) ; 
        } // catch
        catch ( ArgTypeError e ) {
          printf( "%s", e.merrorstr.c_str() ) ;
          evalerr = true ;
          mprinter.Printtree( e.mhead ) ; 
        } // catch
        catch ( NoReturnError e ) {
          printf( "%s", e.merrorstr.c_str() ) ;
          mprinter.Printtree( e.mhead ) ; 
          evalerr = true ;
        } // catch
        
        if ( !evalerr ) {
          mprinter.Printtree( outtree ) ; 
        } // if
          
        
      } // if

      printf( "\n" ) ;
      
      int c = gColumn ;
      char ch = mscanner.Getch() ;
      while ( Iswhitespace( ch ) && ch != '\n' ) {
        mscanner.Getchar() ;
        ch = mscanner.Getch() ;
      } // while
      
      ch = mscanner.Getch() ;
      if ( ch == ';' ) {
        while ( ch != '\n' && !gEnd ) {
          mscanner.Getchar() ;
          ch = mscanner.Getch() ;
        } // while
      } // if

      if ( mscanner.Getch() == '\n' )
        gColumn = 0 ; 
      else
        gColumn = gColumn - c + 1 ;
         
         
    } // while()
    

  } // Gettokenlist()
  
  private:
  
  Printer mprinter ;
  Scanner mscanner ;
  Evaler mevaler ;
  Treemaker mtreemaker ;
  vector<Token> mtokenlist ;
  map< int, Token > morigintree ;
  Token * mtokentree ; 
  
  Token * NewToken( Token token ) {
    Token * retoken = new Token ;
    retoken->str = token.str ;
    retoken->line = token.line ;
    retoken->column = token.column ;
    retoken->intnum = token.intnum ;
    retoken->floatnum = token.floatnum ;
    retoken->type = token.type ;
    retoken->iscomd = false ;
    retoken->left = NULL ;
    retoken->right = NULL ;
    
    return retoken ;
  } // NewToken()
  
  Token * SetTree( int index ) {
    if ( morigintree.find( index ) == morigintree.end() )
      return NULL ;
    else {
      Token * temp = NewToken( morigintree.find( index )->second ) ;
      temp->left = SetTree( 2 * index ) ;
      temp->right = SetTree( 2 * index + 1 ) ;  
      return temp ;
    } // else
  } // SetTree()  
    
};

int main() {
  Interpreter interpreter = Interpreter() ;
  char t ;
  scanf( "%d",  &gTestNum  ) ;
  scanf( "%c",  &t ) ;
  printf( "Welcome to OurScheme!\n\n" ) ;
  try {
    interpreter.Gettokenlist() ;
  } // try
  catch ( Callend e ) {
    ;
  } // catch
  catch ( EndOfFileError e ) {
    printf( "%s", e.merrorstr.c_str() ) ;
  } // catch  
  
  // scanner.Print() ;
  printf( "\nThanks for using OurScheme!" ) ;
  
} // main()
