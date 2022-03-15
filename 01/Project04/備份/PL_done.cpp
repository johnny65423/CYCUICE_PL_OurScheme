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

enum Type { LPAREN, RPAREN, INT, STRING, DOT, FLOAT, NIL, T, QUOTE, SYMBOL, COMMENT, ERROBJ, UNKNOWN };

int gLine = 1 ;              // 「下一個要讀進來的字元」所在的line number
int gColumn = 0 ;            // 「下一個要讀進來的字元」所在的column number
bool gReading = false ;
int gTestNum ;
bool gEnd = false ;
int gCount = 0 ;
bool gVerbose = true ;
struct Token {
  string str ;
  
  int line ;
  int column ;
  int intnum ;
  double doublenum ;
  bool iscomd ;
  Type type ;
  
  Token * left ;
  Token * right ;
};

typedef Token * TokenPtr ;

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

string Todouble( double num ) {
  stringstream ss ;
  ss << num ; 
  if ( ss.str().find( "." ) != -1 )
    return ss.str() ;
  else {
    ss << "." ;
    return ss.str() ;
  } // else
} // Todouble()

Token * CreatToken( Token token ) {
  Token * retoken = new Token ;
  retoken->str = token.str ;
  retoken->line = token.line ;
  retoken->column = token.column ;
  retoken->intnum = token.intnum ;
  retoken->doublenum = token.doublenum ;
  retoken->type = token.type ;
  retoken->iscomd = false ;
  retoken->left = NULL ;
  retoken->right = NULL ;
  
  return retoken ;
} // CreatToken()

Token * SetTree( int index, map< int, Token > morigintree ) {
  if ( morigintree.find( index ) == morigintree.end() )
    return NULL ;
  else {
    Token * temp = CreatToken( morigintree.find( index )->second ) ;
    temp->left = SetTree( 2 * index, morigintree ) ;
    temp->right = SetTree( 2 * index + 1, morigintree ) ;  
    return temp ;
  } // else
} // SetTree() 

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

double Decodedouble( string str ) {
  int positive = 1 ;
  if ( str[0] == '+' )
    str.erase( 0, 1 ) ;
  else if ( str[0] == '-' ) {
    // str.erase( 0, 1 ) ;
    positive = -1 ;
  } // else if
  
  if ( str[0] == '.' )
    str = "0" + str ;
  
  string ch = "" ;


  return atof( str.c_str() ) ;
} // Decodedouble()

string Setdoublestr( string str ) {
  
  if ( str[0] == '+' )
    str.erase( 0, 1 ) ;
    
  if ( str[0] == '.' )
    str = "0" + str ;
  else if ( str[0] == '-' && str[1] == '.' ) {
    str.erase( 0, 1 ) ;
    str = "-0" + str ;
  } // else if
      
   
      
  int index = str.find( "." ) ;
  int size = str.size() - 5 ;

  if ( index > size ) {

    while ( index != str.size() - 5 ) {
      str = str + "0" ;  
    } // while
      
  } // if
  else if ( index < size ) {

    while ( index != str.size() - 5 ) {
      str.erase( str.end() - 1 ) ;  
    } // while
      
  } // if
  
  char ch = str[str.size() - 1] ;
  
  if ( ch > '4' ) {
    str.erase( str.end() - 1 ) ;
    double num = atof( str.c_str() ) ;
    num = num + 0.001 ;
    stringstream ss ;
    ss << num ;
    str = Setdoublestr( ss.str() ) ;  
  } // if
  else {
    str.erase( str.end() - 1 ) ;
  } // else
  
  return str ;
} // Setdoublestr()

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
  else if ( str == "quote" || str == "exit" || str == "error-object?" ) 
    return true ;
  else if ( str == "create-error-object" ) 
    return true ;
  else 
    return false ;
} // Isinternalfunc()

bool Isspfunc( string str ) {
  if ( str == "define" ) 
    return true ;
  else if ( str == "and" || str == "or" || str == "begin" ) 
    return true ;
  else if ( str == "if" || str == "cond" || str == "clean-environment" ) 
    return true ;
  else if ( str == "let" || str == "exit" || str == "lambda" ) 
    return true ;
  else if ( str == "read" || str == "set!" || str == "eval" ) 
    return true ; 
  else if ( str == "write" || str == "display-string" || str == "newline" ) 
    return true ; 
  else if ( str == "symbol->string" || str == "number->string" || str == "newline" ) 
    return true ; 
  else if ( str == "verbose" || str == "verbose?" ) 
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
  else if ( str == "error-object?" ) return true ;
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

double Getnum( Token * temp ) {
  if ( temp->type == INT )
    return ( double ) temp->intnum ;
  else if ( temp->type == FLOAT )
    return temp->doublenum ; 
  else return 0.0 ;

} // Getnum()

class Exception {
public:
  string mname ;
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
    mname = "Stringerror" ;
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
    mname = "Callend" ;
  } // Callend()
};

class EndOfFileError : public Exception {
public:
  EndOfFileError() {
    mname = "EndOfFileError" ;
    stringstream ss ;

    ss << "ERROR (no more input) : END-OF-FILE encountered" ;

    merrorstr = ss.str() ;
  } // EndOfFileError()
};

class ArgNumError : public Exception {
public:
  ArgNumError( string str ) {
    mname = "ArgNumError" ;
    stringstream ss ;

    ss << "ERROR (incorrect number of arguments) : " << str ;

    merrorstr = ss.str() ;

  } // ArgNumError()
};

class UnboundError : public Exception {
public:
  UnboundError( string str ) {
    mname = "UnboundError" ;
    stringstream ss ;

    ss << "ERROR (unbound symbol) : " << str ;

    merrorstr = ss.str() ;
  } // UnboundError()
};

class NonFuncError : public Exception {
public:
  NonFuncError( Token * temp ) {
    mname = "NonFuncError" ;
    stringstream ss ;

    ss << "ERROR (attempt to apply non-function) : " ;

    merrorstr = ss.str() ;
    mhead = temp ;
  } // NonFuncError()
};

class NonListError : public Exception {
public:
  NonListError() {
    mname = "NonListError" ;
    stringstream ss ;

    ss << "ERROR (non-list) : " ;

    merrorstr = ss.str() ;
  } // NonListError()
};

class LevelError : public Exception {
public:
  LevelError( string str ) {
    mname = "LevelError" ;
    stringstream ss ;

    ss << "ERROR (level of " << str << ")" ;

    merrorstr = ss.str() ;
  } // LevelError()
};

class FormatError : public Exception {
public:
  FormatError( string str, Token * temp ) {
    mname = "FormatError" ;
    stringstream ss ;

    ss << "ERROR (" << str << " format) : " ;
    mhead = temp ;
    merrorstr = ss.str() ;
  } // FormatError()
};

class ArgTypeError : public Exception {
public:
  ArgTypeError( string str, Token * temp ) {
    stringstream ss ;
    mname = "ArgTypeError" ;
    ss << "ERROR (" << str << " with incorrect argument type) : " ;

    merrorstr = ss.str() ;
    mhead = temp ;
  } // ArgTypeError()
};

class DivideZeroError : public Exception {
public:
  DivideZeroError() {
    stringstream ss ;
    mname = "DivideZeroError" ;
    ss << "ERROR (division by zero) : /" ;

    merrorstr = ss.str() ;
  } // DivideZeroError()
};

class NoReturnError : public Exception {
public:
  NoReturnError( Token * temp ) {
    stringstream ss ;
    mname = "NoReturnError" ;
    ss << "ERROR (no return value) : " ;
    mhead = temp ;
    merrorstr = ss.str() ;
  } // NoReturnError()
};

class UnboundParaError : public Exception {
public:
  UnboundParaError( Token * temp ) {
    stringstream ss ;
    mname = "UnboundParaError" ;
    ss << "ERROR (unbound parameter) : " ;
    mhead = temp ;
    merrorstr = ss.str() ;
  } // UnboundParaError()
};

class UnboundCondiError : public Exception {
public:
  UnboundCondiError( Token * temp ) {
    stringstream ss ;
    mname = "UnboundCondiError" ;
    ss << "ERROR (unbound condition) : " ;
    mhead = temp ;
    merrorstr = ss.str() ;
  } // UnboundCondiError()
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
    // else if ( type == FLOAT )
    //   printf( "%.3f", token->doublenum ) ;
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
      // printf( "\n" );
    } // if
    else  
      PrintRe( temp, 0 ) ;
  } // Printtree()
  
  void PrintRe( Token * temp, int spacenum ) {

    Type type = temp->type ;

    if ( type == DOT && temp->left->str == "lambda" && temp->left->iscomd ) {
      Printtoken( temp->left ) ;
      // printf( "\n" ) ;
    } // if
    else if ( type == DOT ) {
      
      printf( "( " ) ;
      
      type = temp->left->type ;

      if ( type == DOT ) {
        PrintRe( temp->left, spacenum + 2 ) ;
      } // if
      else {
        
        Printtoken( temp->left ) ;  

      } // else
                  
      temp = temp->right ;
      while ( temp != NULL ) {
        type = temp->type ;
        if ( type == DOT ) {
          type = temp->left->type ;
            
          if ( type == DOT ) {
            printf( "\n" ) ;
            for ( int i = 0 ; i < spacenum + 2 ; i++ )
              printf( " " ) ;
            PrintRe( temp->left, spacenum + 2 ) ;
          } // if
          else {
            printf( "\n" ) ;
            for ( int i = 0 ; i < spacenum + 2 ; i++ )
              printf( " " ) ;
                
            Printtoken( temp->left ) ;

          } // else
        } // if
        else {
          type = temp->type ;
          if ( type != NIL ) {
            printf( "\n" ) ; 
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
      
      printf( "\n" ) ; 
      
      for ( int i = 0 ; i < spacenum ; i++ )
        printf( " " ) ;
          
      printf( ")" ) ;
      
    } // else if
    else {
      if ( type == NIL ) 
        ;
      else {
        for ( int i = 0 ; i < spacenum ; i++ )
          printf( " " ) ;
        Printtoken( temp ) ;
        // printf( "\n" ) ;  
      } // else

    } // else
    
    // printf( "\n" ) ;
  } // PrintRe()  
  
  
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
      
      while ( !gEnd && mch != ')' && ! ( mch == '.' && Justdot() ) ) {
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

      if ( gEnd ) {
        // cout << " f" ;
        throw EndOfFileError() ;// error
      } // if
      else if ( mch == ')' ) {
       
        temp = Gettoken() ;
        tokenlist.push_back( temp ) ;
      } // else if
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
    else if ( retoken.type == FLOAT ) {
      retoken.doublenum = Decodedouble( retoken.str ) ;
      retoken.str = Setdoublestr( retoken.str ) ;
    } // else if
      

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
       
      if ( gEnd )
        throw Stringerror( tk, 2 ) ;
       
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

    // if ( gEnd )
    //   throw EndOfFileError() ; 
      
    return temp ;
  } // Getothers() 
  
    
  
};

class Treemaker {
  public:
  void Buildtree( vector<Token> & tokenlist, map< int, Token > & tokentree ) {
    
    int point = 1 ;
    int index = 0 ;

    if ( tokenlist.size() == 1 ) {
      tokentree[1] = tokenlist.at( 0 ) ;
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
    else if ( retoken.type == FLOAT ) {
      retoken.doublenum = Decodedouble( retoken.str ) ;
      retoken.str = Setdoublestr( retoken.str ) ;
    } // else if
      
      
    return retoken ;
  } // Maktoken()
    
};

Scanner gScanner ;
Printer gPrinter ;
Treemaker gTreemaker ;

class Evaler {
  private:
    
  // vector< Symbol > msymbollist ;
  map < string, TokenPtr > msymbollist ;
    
  Token * NewToken( string str ) {
    Token * retoken = new Token ;
    retoken->str = Setstr( str ) ;
    retoken->type = Gettype( retoken->str ) ;
    retoken->intnum = Decodeint( retoken->str ) ;
    retoken->doublenum = Decodedouble( retoken->str ) ;
    if ( retoken->type == FLOAT ) 
      retoken->str = Setdoublestr( retoken->str ) ;
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
  
  int Findsymbol( string str, map < string, TokenPtr > & localsymlist ) {

    if ( localsymlist.find( str ) != localsymlist.end() ) {
        
      if ( localsymlist[str]->left == NULL )
        return 1 ;
      else 
        return 2 ;

    } // if

    

    if ( msymbollist.find( str ) != msymbollist.end() ) {
      
      if ( msymbollist[str]->left == NULL )
        return 1 ;
      else 
        return 2 ;

    } // if

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
  
  Token * Symbols( Token * temp, map < string, TokenPtr > & localsymlist ) {
    bool find = false ;

    if ( localsymlist.find( temp->str ) != localsymlist.end() )  {
      find = true ; 
        
      Token * retoken = localsymlist[ temp->str ]->right ;
      return retoken ;
    } // if


    if ( msymbollist.find( temp->str ) != msymbollist.end() )  {
      find = true ;
        
      Token * retoken = msymbollist[temp->str]->right ;
      return retoken ;
    } // if

    
    if ( !find ) {
    
      throw UnboundError( temp->str ) ;
    
    } // if 
    
    return NULL ;
  } // Symbols()

  void Change( Token * temp, map < string, TokenPtr > & localsymlist ) {
    if ( temp != NULL ) {
      if ( temp->left != NULL && Findsymbol( temp->left->str, localsymlist ) == 1 ) {
        temp->left = Copytoken( Symbols( temp->left, localsymlist ) ) ;
      } // if

      if ( temp->right != NULL && Findsymbol( temp->right->str, localsymlist ) == 1 ) {
        temp->right = Copytoken( Symbols( temp->right, localsymlist ) ) ;
      } // if    

      Change( temp->left, localsymlist ) ;
      Change( temp->right, localsymlist ) ;
    } // if

  }  // Change()

  Token * Define( Token * temp1, map < string, TokenPtr > & localsymlist ) {
    Token * temp = temp1->right ;
    
    if ( temp->left != NULL && temp->left->type == DOT )
      return Definefunc( temp1, localsymlist ) ;   
 
    if ( Getsize( temp ) != 2 )
      throw FormatError( "DEFINE", temp1 ) ;

  
    
    // Symbol newsymbol ;
    TokenPtr newsymbol = NewToken( "." ) ;
    string name = temp->left->str ;
    // newsymbol.name = name ;
    newsymbol->left = NULL ;
    
    
    if ( Isinternalfunc( name ) || Isspfunc( name ) || temp->left->type != SYMBOL ) { 
      throw FormatError( "DEFINE", temp1 ) ;
    } // if
    
    if ( Isatomtype( temp->right->left->type ) && temp->right->left->type != SYMBOL ) {
      Token * check = Copytoken( temp->right->left ) ;
      Change( check, localsymlist ) ;
      newsymbol->right = check ;
    } // if
    else if ( temp->right->left->type == DOT && temp->right->left->left->str == "lambda" ) { 
      temp->right->left->left->iscomd = true ;
      newsymbol->right = temp->right->left ;
    } // else if
    else { 
      try {
        newsymbol->right = Evalexp( temp->right->left, 1, localsymlist ) ;
      } // try
      catch ( Exception e ) {
        if ( e.mname == "NoReturnError" )
          e.mname = "NoReturnErrorgogo" ;

        throw e ;
      } // catch
    } // else 
      

    msymbollist[name] = newsymbol ;  

    if ( gVerbose )
      printf( "%s defined\n", name.c_str() ) ;
    return NULL ;
  } // Define()

  Token * Set( Token * temp1, map < string, TokenPtr > & localsymlist ) {
    Token * temp = temp1->right ;
    
    if ( temp->left != NULL && temp->left->type == DOT )
      throw FormatError( "SET", temp1 ) ;  
 
    if ( Getsize( temp ) != 2 )
      throw FormatError( "SET", temp1 ) ;
    
    // Symbol newsymbol ;
    TokenPtr newsymbol = NewToken( "." ) ;
    string name = temp->left->str ;
    // newsymbol.name = name ;
    newsymbol->left = NULL ;
    
    
    if ( Isinternalfunc( name ) || Isspfunc( name ) || temp->left->type != SYMBOL ) { 
      throw FormatError( "SET", temp1 ) ;
    } // if
    
    if ( Isatomtype( temp->right->left->type ) && temp->right->left->type != SYMBOL ) {
      Token * check = Copytoken( temp->right->left ) ;
      Change( check, localsymlist ) ;
      newsymbol->right = check ;
    } // if
    else if ( temp->right->left->type == DOT && temp->right->left->left->str == "lambda" ) { 
      temp->right->left->left->iscomd = true ;
      newsymbol->right = temp->right->left ;
    } // else if
    else { 
      try {
        newsymbol->right = Evalexp( temp->right->left, 1, localsymlist ) ;
      } // try
      catch ( Exception e ) {
        if ( e.mname == "NoReturnError" )
          e.mname = "NoReturnErrorgogo" ;

        throw e ;
      } // catch
    } // else 
      
    bool seted = false ;

    if ( localsymlist.find( name ) != localsymlist.end() ) {
      localsymlist[name] = newsymbol ;
      seted = true ;
    } // if

    
    if ( !seted ) {
      msymbollist[name] = newsymbol ;  
    } // if


    return newsymbol->right ;
  } // Set()

  Token * Definefunc( Token * temp1, map < string, TokenPtr > & localsymlist ) {
    Token * temp = temp1->right ;
    
    if ( Getsize( temp ) < 2 )
      throw FormatError( "DEFINE", temp1 ) ;  
    
    // Symbol newsymbol ;
    TokenPtr newsymbol = NewToken( "." ) ;
    string name = temp->left->left->str ;
    // newsymbol.name = name ;
    newsymbol->left = temp->left->right ;
    
    if ( Isinternalfunc( name ) || Isspfunc( name ) || temp->left->left->type != SYMBOL ) { 
      throw FormatError( "DEFINE", temp1 ) ;
    } // if
    
    newsymbol->right = temp->right ;

    msymbollist[name] = newsymbol ;  

    if ( gVerbose ) {
      printf( "%s defined\n", name.c_str() ) ;
    } // if
    
    return NULL ;
  } // Definefunc()
  
  Token * Customfunc( Token * temp, int head, map < string, TokenPtr > & localsymlist ) {
    int i = msymbollist.size() - 1 ;
    string str = temp->left->str ;
    bool find = false ;
    Token * argname ;
    Token * method ;
    Token * args ;
    Token * retoken ;

    
    argname = msymbollist[str]->left ;
    method = msymbollist[str]->right ;
    find = true ;


    
    args = temp->right ;
    int argsnum = Getsize( argname ) ;
    
    if ( argsnum != Getsize( args ) )
      throw ArgNumError( str ) ;

    map <string, TokenPtr> templist ;
    while ( argname->type != NIL ) {
      TokenPtr sym = NewToken( "." ) ;
      string name = argname->left->str ;
      sym->left = NULL ;
      try {
        sym->right = Evalexp( args->left, 1, localsymlist ) ;
      } // try
      catch ( Exception e ) {

        if ( e.mname == "NoReturnError" )
          throw UnboundParaError( args->left ) ;
        
        throw e ;
      } // catch
      
      templist[name] = sym ;
      
      argname = argname->right ;
      args = args->right ;
    } // while 
    
    Token * ans = NULL ;
    retoken = NULL ;

    while ( method->type != NIL ) {
      try {
        ans = Evalexp( method->left, head, templist ) ;
        retoken = ans ;
      } // try
      catch ( Exception e ) {
        
        if ( e.mname == "NoReturnError" ) {
          e.mhead = temp ;
          if ( method->right->type == NIL ) {

            throw e ;
          } // if
        } // if
        else {

          throw e ;
        } // else
      } // catch

      method = method->right ;
    } // while

    if ( retoken == NULL )
      throw NoReturnError( temp ) ;
    return retoken ;
  } // Customfunc()
  
  Token * Setlambda( Token * temp, int head, map < string, TokenPtr > & localsymlist ) {
    if ( Getsize( temp->right ) < 2 )
      throw FormatError( "LAMBDA", temp ) ;


    Token * argname = temp->right->left ;
    Token * method = temp->right->right ;

    if ( argname->type != DOT && argname->type != NIL )
      throw FormatError( "LAMBDA", temp ) ;
    
    Token * t = argname ;
    
    while ( t != NULL && t->type != NIL ) {
      if ( t->left->type != SYMBOL )
        throw FormatError( "LAMBDA", temp ) ;
      
      t = t->right ;
    } // while

    Token * retoken = NewToken( "lambda" ) ;
    retoken->left = argname ;
    retoken->right = method ;
    
    retoken->iscomd = true ;
    return retoken ;
    
    
  } // Setlambda()
  
  Token * Lambda( Token * temp, int head, map < string, TokenPtr > & localsymlist ) {

    Token * argname = temp->left->left ;
    Token * method = temp->left->right ;
    Token * args = temp->right ;

    int argsnum = Getsize( argname ) ;
    
    if ( argsnum != Getsize( args ) )
      throw ArgNumError( "lambda" ) ;
      
    map < string, TokenPtr > templist ;  
    while ( argname->type != NIL ) {
      // Symbol sym ;
      TokenPtr sym = NewToken( "." ) ;
      string name = argname->left->str ;
      sym->left = NULL ;
      try {
        sym->right = Evalexp( args->left, 1, localsymlist ) ;
      } // try
      catch ( Exception e ) {

        if ( e.mname == "NoReturnError" )
          throw UnboundParaError( args->left ) ;
        throw e ;
      } // catch
      templist[name] = sym ;
      
      argname = argname->right ;
      args = args->right ;
    } // while  

    Token * ans = NULL ;
    Token * retoken ;

    while ( method->type != NIL ) {
      try {
        ans = Evalexp( method->left, head, templist ) ;
        retoken = ans ;
      } // try
      catch ( Exception e ) {

        if ( e.mname == "NoReturnError" ) {
          e.mhead = temp ;
          if ( method->right->type == NIL ) {

            throw e ;
          } // if
        } // if
        else {

          throw e ;
        } // else
      } // catch

      method = method->right ;
    } // while

    return retoken ;
  } // Lambda()
  
  Token * Let( Token * temp1, map < string, TokenPtr > & localsymlist ) {
    Token * temp = temp1->right ;
    if ( Getsize( temp ) < 2 )
      throw FormatError( "LET", temp1 ) ;
    
    Token * args = temp->left ;
    Token * argtemp ;
    int argsnum = Getsize( args ) ;
    // vector < Symbol > templist ;  
    map < string, TokenPtr > templist ;
    
    while ( args->type != NIL ) {
      if ( Getsize( args->left ) != 2 )
        throw FormatError( "LET", temp1 ) ;
      else {
        argtemp = args->left ;
        string name = argtemp->left->str ;
        
        if ( argtemp->left->type != SYMBOL || Isinternalfunc( name ) || Isspfunc( name ) ) {
          throw FormatError( "LET", temp1 ) ;
        } // if
      } // else
        
      args = args->right ;
    } // while  


    if ( !localsymlist.empty() ) {
      for ( map<string,TokenPtr>::iterator it = localsymlist.begin() ; it != localsymlist.end() ; it++ ) {
        templist[ it->first ] = it->second ;
      } // for
    } // if
    
    args = temp->left ;
    while ( args->type != NIL ) {

      // Symbol sym ;
      TokenPtr sym = NewToken( "." ) ;
      argtemp = args->left ;
      if ( argtemp->type != DOT )
        throw FormatError( "LET", temp1 ) ;
      string name = argtemp->left->str ; 
      if ( Isinternalfunc( name ) || Isspfunc( name ) || argtemp->left->type != SYMBOL ) { 
        throw FormatError( "LET", temp1 ) ;
      } // if
      
      // sym.name = name ;
      sym->left = NULL ;
      
      try {
        sym->right = Evalexp( argtemp->right->left, 0, localsymlist ) ;
      } // try 
      catch ( Exception e ) {
        if ( e.mname == "NoReturnError" )
          e.mname = "NoReturnErrorgogo" ;
        throw e ;        
      } // catch
      templist[name] = sym ;
      
      args = args->right ;
    } // while  
    
    
    Token * t = temp->right ;
    Token * result = NULL;
    Token * ans = NULL;
    while ( t != NULL && t->type != NIL ) {
      try {
        ans = Evalexp( t->left, 1, templist ) ;
        result = ans ;
      } // try
      catch ( Exception e ) {
        if ( e.mname == "NoReturnError" ) {
          if ( t->right->type == NIL ) {
            e.mhead = temp1 ;

            throw e ;
          } // if
        } // if
        else {

          throw e ;
        } // else
      } // catch
      
      t = t->right ;
    } // while

    return result ;

  } // Let()

  Token * If( Token * temp1, map < string, TokenPtr > & localsymlist ) {
    Token * temp = temp1->right ;
    int size = Getsize( temp ) ;
    if ( size != 2 && size != 3 )
      throw ArgNumError( "if" ) ;
    
    Token * check ;
    Token * result ;
    
    try {
      check = Evalexp( temp->left, 1, localsymlist ) ;
    } // try
    catch ( Exception e ) {
      if ( e.mname == "NoReturnError" )
        throw UnboundParaError( e.mhead ) ;
        // unboundtest
      throw e ;
    } // catch



    if ( check->type != NIL ) {
      result = Evalexp( temp->right->left, 1, localsymlist ) ;
    } // if
    else {
      if ( size == 2 )
        throw NoReturnError( temp1 ) ;
      else {
        result = Evalexp( temp->right->right->left, 1, localsymlist ) ;
      } // else

    } // else

    return result ;

  } // If()

  Token * Decide( Token * temp, bool last, bool & done, Token * temp1, 
                  map < string, TokenPtr > & localsymlist ) {

    if ( Getsize( temp ) < 1 )
      throw FormatError( "COND", temp1 ) ;
    else if ( Isatomtype( temp->type ) ) {
      throw FormatError( "COND", temp1 ) ;
    } // else if
    else if ( ( last && temp->left->str == "else" ) ) {
      done = true ;
      return Begincond( temp->right, temp1, localsymlist ) ;
    } // if

    Token * check ;
    Token * result ;

    try {
      check = Evalexp( temp->left, 1, localsymlist );
    } // try 
    catch ( Exception e ) {
      if ( e.mname == "NoReturnError" )
        throw UnboundParaError( e.mhead ) ;
        // unboundtest
      throw e ;
    } // catch

    if ( check->type != NIL ) {
      done = true ;
      return Begincond( temp->right, temp1, localsymlist ) ;
    } // if
    else {
      done = false ;
      return NewToken( "nil" ) ;
    } // else

  } // Decide()

  Token * Cond( Token * temp1, map < string, TokenPtr > & localsymlist ) {
    Token * temp = temp1->right ;
    int size = Getsize( temp ) ;
    if ( Getsize( temp ) < 1 )
      throw FormatError( "COND", temp1 ) ;
    
    Token * t = temp ;
    bool check, done ;
    Token * result ;

    while ( t->type != NIL ) {
      if ( Isatomtype( t->left->type ) ) {
        throw FormatError( "COND", temp1 ) ;
      } // if
      else if ( Getsize( t->left ) < 2 ) {
        throw FormatError( "COND", temp1 ) ;
      } // else if

      t = t->right ;
    } // while

    t = temp ;

    while ( t->type != NIL ) {
      if ( t->right->type == NIL ) {

        result = Decide( t->left, true, done, temp1, localsymlist ) ;

      } // if
      else {
        result = Decide( t->left, false, done, temp1, localsymlist ) ;
      } // else

      if ( done ) {
        return result ;
      } // if

      t = t->right ;
    } // while

    throw NoReturnError( temp1 ) ;

  } // Cond()
  
  Token * Begin( Token * temp, map < string, TokenPtr > & localsymlist ) {
    int size = Getsize( temp ) ;
    if ( Getsize( temp ) < 1 )
      throw ArgNumError( "begin" ) ;
    
    Token * t = temp ;
    bool done = true ;
    Token * result = NULL ;
    Token * ans ;
    while ( t != NULL && t->type != NIL ) {
      done = true ;
      // error ;
      try {
        ans = Evalexp( t->left, 1, localsymlist ) ;
        result = ans ;
      } // try
      catch ( Exception e ) {
        if ( e.mname == "NoReturnError" ) {
          if ( t->right->type == NIL ) {
            // e.mhead = temp1 ;
            throw e ;
          } // if
        } // if
        else 
          throw e ;

      } // catch


      t = t->right ;
    } // while
    
    if ( result == NULL ) {
      throw NoReturnError( temp ) ;
    } // if
    
    return result ;

  } // Begin()

  Token * Begincond( Token * temp, Token * temp1, map < string, TokenPtr > & localsymlist ) {
    int size = Getsize( temp ) ;
    if ( Getsize( temp ) < 1 )
      throw FormatError( "COND", temp1 ) ;
    
    Token * t = temp ;
    bool check, done ;
    Token * result = NULL ;
    Token * ans ;
    while ( t != NULL && t->type != NIL ) {
      // result = Evalexp( t->left, 1, localsymlist ) ;
      // error
      try {
        ans = Evalexp( t->left, 1, localsymlist ) ;
        result = ans ;
      } // try
      catch ( Exception e ) {
        if ( e.mname == "NoReturnError" ) {
          if ( t->right->type == NIL )
            throw e ;
        } // if
        else
          throw e ;
      } // catch

      
      t = t->right ;
    } // while

    if ( result == NULL ) {
      throw NoReturnError( temp ) ;
    } // if
    
    return result ;

  } // Begincond()

  Token * Quote( Token * temp ) {
    temp->left->iscomd = false ;
    return temp->left ; 
  } // Quote()

  Token * Cons( Token * temp, map < string, TokenPtr > & localsymlist ) {
    if ( Getsize( temp ) != 2 )
      throw ArgNumError( "cons" ) ;
    
    Token * retoken = NewToken( "." ) ;
    TokenPtr ans[2] ;
    Token * t = temp ;
    for ( int i = 0 ; i < 2 ; i++ ) {
      try {
        ans[i] = Evalexp( t->left, 1, localsymlist ) ;
      } // try
      catch ( Exception e ) {
        if ( e.mname == "NoReturnError" )
          throw UnboundParaError( e.mhead ) ;
      
        throw e ;
      } // catch
      
      t = t->right ;
    } // for

    retoken->left = ans[0] ;
    retoken->right = ans[1] ;

    return retoken ;  
    
    
  } // Cons()

  Token * List( Token * temp, map < string, TokenPtr > & localsymlist ) {
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
      try {
        ret->left = Evalexp( t->left, 1, localsymlist ) ; 
      } // try
      catch ( Exception e ) {
        if ( e.mname == "NoReturnError" )
          throw UnboundParaError( e.mhead ) ;
      
        throw e ;
      } // catch
      
      if ( t->right->type != NIL )
        ret->right = NewToken( "." ) ;
      else 
        ret->right = NewToken( "nil" ) ;
    
      ret = ret->right ;
      t = t->right ;

    } // while
    
    return retoken ;  
  } // List()

  Token * Car( Token * temp, map < string, TokenPtr > & localsymlist ) {
    if ( Getsize( temp ) != 1 )
      throw ArgNumError( "car" ) ;
      
    Token * check ;  
    try {
      check = Evalexp( temp->left, 1, localsymlist ) ;
    } // try
    catch ( Exception e ) {
      if ( e.mname == "NoReturnError" )
        throw UnboundParaError( e.mhead ) ;
    
      throw e ;
    } // catch
    
    if ( Isatomtype( check->type ) )
      throw ArgTypeError( "car", check ) ;
      
    return check->left ;
    
  } // Car()

  Token * Cdr( Token * temp, map < string, TokenPtr > & localsymlist ) {
    if ( Getsize( temp ) != 1 )
      throw ArgNumError( "cdr" ) ;
    
    Token * check ;  
    try {
      check = Evalexp( temp->left, 1, localsymlist ) ;
    } // try
    catch ( Exception e ) {
      if ( e.mname == "NoReturnError" )
        throw UnboundParaError( e.mhead ) ;
    
      throw e ;
    } // catch

    
    if ( Isatomtype( check->type ) )
      throw ArgTypeError( "cdr", check ) ;
      
    return check->right ;
    
  } // Cdr()
  
  Token * Not( Token * temp, map < string, TokenPtr > & localsymlist ) {
    if ( Getsize( temp ) != 1 )
      throw ArgNumError( "not" ) ;
    Token * check ;
    try {
      check = Evalexp( temp->left, 1, localsymlist ) ;
    } // try
    catch ( Exception e ) {
      if ( e.mname == "NoReturnError" )
        throw UnboundParaError( e.mhead ) ;
    
      throw e ;
    } // catch
    
    if ( check->type == NIL )     
      return NewToken( "#t" ) ;
    else 
      return NewToken( "nil" ) ;
    
  } // Not()
  
  Token * Greater( Token * temp, map < string, TokenPtr > & localsymlist ) {
    string check = "#t" ;
    while ( temp->right->type != NIL && temp->right != NULL ) {
      Token * check1 ;
      Token * check2 ;
      try {
        check1 = Evalexp( temp->left, 1, localsymlist ) ;
        check2 = Evalexp( temp->right->left, 1, localsymlist ) ;
      } // try
      catch ( Exception e ) {
        if ( e.mname == "NoReturnError" )
          throw UnboundParaError( e.mhead ) ;
      
        throw e ;
      } // catch
      
      
      
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

  Token * Less( Token * temp, map < string, TokenPtr > & localsymlist ) {
    string check = "#t" ;
    while ( temp->right->type != NIL && temp->right != NULL ) {
      Token * check1 ;
      Token * check2 ;
      try {
        check1 = Evalexp( temp->left, 1, localsymlist ) ;
        check2 = Evalexp( temp->right->left, 1, localsymlist ) ;
      } // try
      catch ( Exception e ) {
        if ( e.mname == "NoReturnError" )
          throw UnboundParaError( e.mhead ) ;
      
        throw e ;
      } // catch
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

  Token * Nogreater( Token * temp, map < string, TokenPtr > & localsymlist ) {
    string check = "#t" ;
    while ( temp->right->type != NIL && temp->right != NULL ) {
      Token * check1 ;
      Token * check2 ;
      try {
        check1 = Evalexp( temp->left, 1, localsymlist ) ;
        check2 = Evalexp( temp->right->left, 1, localsymlist ) ;
      } // try
      catch ( Exception e ) {
        if ( e.mname == "NoReturnError" )
          throw UnboundParaError( e.mhead ) ;
      
        throw e ;
      } // catch
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

  Token * Noless( Token * temp, map < string, TokenPtr > & localsymlist ) {
    string check = "#t" ;
    while ( temp->right->type != NIL && temp->right != NULL ) {
      Token * check1 ;
      Token * check2 ;
      try {
        check1 = Evalexp( temp->left, 1, localsymlist ) ;
        check2 = Evalexp( temp->right->left, 1, localsymlist ) ;
      } // try
      catch ( Exception e ) {
        if ( e.mname == "NoReturnError" )
          throw UnboundParaError( e.mhead ) ;
      
        throw e ;
      } // catch
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
  
  Token * Equalnum( Token * temp, map < string, TokenPtr > & localsymlist ) {
    string check = "#t" ;
    while ( temp->right->type != NIL && temp->right != NULL ) {
      Token * check1 ;
      Token * check2 ;
      try {
        check1 = Evalexp( temp->left, 1, localsymlist ) ;
        check2 = Evalexp( temp->right->left, 1, localsymlist ) ;
      } // try
      catch ( Exception e ) {
        if ( e.mname == "NoReturnError" )
          throw UnboundParaError( e.mhead ) ;
      
        throw e ;
      } // catch
  
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

  Token * Strappend( Token * temp, map < string, TokenPtr > & localsymlist ) {
    string check = "" ;
    while ( temp->type != NIL && temp != NULL ) {
      Token * nstr ;
      try {
        nstr = Evalexp( temp->left, 1, localsymlist ) ;
      } // try
      catch ( Exception e ) {
        if ( e.mname == "NoReturnError" )
          throw UnboundParaError( e.mhead ) ;
      
        throw e ;
      } // catch
      
      if ( nstr->type != STRING )
        throw ArgTypeError( "string-append", nstr ) ;

      check = Mergestr( check, nstr->str ) ;
      temp = temp->right ;
    } // while

    return NewToken( check ) ;

  } // Strappend()

  Token * Strgreat( Token * temp, map < string, TokenPtr > & localsymlist ) {
    string check = "#t" ;
    while ( temp->right->type != NIL && temp->right != NULL ) {
      Token * check1 ;
      Token * check2 ;
      try {
        check1 = Evalexp( temp->left, 1, localsymlist ) ;
        check2 = Evalexp( temp->right->left, 1, localsymlist ) ;
      } // try
      catch ( Exception e ) {
        if ( e.mname == "NoReturnError" )
          throw UnboundParaError( e.mhead ) ;
      
        throw e ;
      } // catch
      
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

  Token * Strless( Token * temp, map < string, TokenPtr > & localsymlist ) {
    string check = "#t" ;
    while ( temp->right->type != NIL && temp->right != NULL ) {
      Token * check1 ;
      Token * check2 ;
      try {
        check1 = Evalexp( temp->left, 1, localsymlist ) ;
        check2 = Evalexp( temp->right->left, 1, localsymlist ) ;
      } // try
      catch ( Exception e ) {
        if ( e.mname == "NoReturnError" )
          throw UnboundParaError( e.mhead ) ;
      
        throw e ;
      } // catch
      
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

  Token * Strequal( Token * temp, map < string, TokenPtr > & localsymlist ) {
    string check = "#t" ;
    while ( temp->right->type != NIL && temp->right != NULL ) {
      Token * check1 ;
      Token * check2 ;
      try {
        check1 = Evalexp( temp->left, 1, localsymlist ) ;
        check2 = Evalexp( temp->right->left, 1, localsymlist ) ;
      } // try
      catch ( Exception e ) {
        if ( e.mname == "NoReturnError" )
          throw UnboundParaError( e.mhead ) ;
      
        throw e ;
      } // catch
      
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

  Token * Eqv( Token * temp, map < string, TokenPtr > & localsymlist ) {
    if ( Getsize( temp ) != 2 )
      throw ArgNumError( "eqv?" ) ;
    Token * check1 ;
    Token * check2 ;
    try {
      check1 = Evalexp( temp->left, 1, localsymlist ) ;
      check2 = Evalexp( temp->right->left, 1, localsymlist ) ;
    } // try
    catch ( Exception e ) {
      if ( e.mname == "NoReturnError" )
        throw UnboundParaError( e.mhead ) ;
    
      throw e ;
    } // catch
      
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

  Token * Equal( Token * temp, map < string, TokenPtr > & localsymlist ) {
    if ( Getsize( temp ) != 2 )
      throw ArgNumError( "equ?" ) ;
    
    Token * check1 ;
    Token * check2 ;
    try {
      check1 = Evalexp( temp->left, 1, localsymlist ) ;
      check2 = Evalexp( temp->right->left, 1, localsymlist ) ;
    } // try
    catch ( Exception e ) {
      if ( e.mname == "NoReturnError" )
        throw UnboundParaError( e.mhead ) ;
    
      throw e ;
    } // catch
      
    if ( Issame( check1, check2 ) ) 
      return NewToken( "#t" ) ;
    else 
      return NewToken( "nil" ) ;

  } // Equal()
  
  Token * Or( Token * temp, map < string, TokenPtr > & localsymlist ) {
    int size = Getsize( temp ) ;
    if ( Getsize( temp ) < 2 )
      throw ArgNumError( "or" ) ;
    
    Token * t = temp ;
    bool check, done ;
    Token * result ;
    while ( t != NULL && t->type != NIL ) {
      try {
        result = Evalexp( t->left, 1, localsymlist ) ;
      } // try
      catch ( Exception e ) {
        if ( e.mname == "NoReturnError" ) {
          throw UnboundCondiError( e.mhead ) ;
        } // if
        else {
          throw e ;
        } // else
      } // catch
    
      if ( result->type != NIL )
        return result ;

      t = t->right ;
    } // while

    return result ;

  } // Or()

  Token * And( Token * temp, map < string, TokenPtr > & localsymlist ) {
    int size = Getsize( temp ) ;
    if ( Getsize( temp ) < 2 )
      throw ArgNumError( "and" ) ;
    
    Token * t = temp ;
    bool check, done ;
    Token * result ;

    while ( t != NULL && t->type != NIL ) {
      try {
        result = Evalexp( t->left, 1, localsymlist ) ;
      } // try
      catch ( Exception e ) {
        if ( e.mname == "NoReturnError" ) {
          throw UnboundCondiError( e.mhead ) ;
        } // if
        else {
          throw e ;
        } // else
      } // catch
    
      
      if ( result->type == NIL )
        return result ;
      t = t->right ;
    } // while

    return result ;

  } // And()

  Token * Read( Token * temp, map < string, TokenPtr > & localsymlist ) {
    
    if ( Getsize( temp->right ) > 0 ) {
      throw ArgNumError( "read" ) ;
    } // if
    
    map< int, Token > origintree ;
    vector < Token > tokenlist ;
    Token * tokentree ;
    try {
      gLine = 1 ;  
      while ( Iswhitespace( gScanner.Getch() ) && gScanner.Getch() != '\n' && !gEnd )
        gScanner.Getchar() ; 
          
      gScanner.ReadSexp( tokenlist ) ;
      gTreemaker.Buildtree( tokenlist, origintree ) ; 
      tokentree = SetTree( 1, origintree ) ;
    } // try
    catch ( Exception e ) { 
      stringstream ss ;
      ss << "\"" << e.merrorstr << "\"" ;
      Token * errortoken = NewToken( ss.str() ) ;
      errortoken->type = ERROBJ ;
      return errortoken ;
    } // catch

    return tokentree ;

  } // Read()
  
  Token * Write( Token * temp, map < string, TokenPtr > & localsymlist ) {
    
    if ( Getsize( temp->right ) != 1 ) {
      throw ArgNumError( "write" ) ;
    } // if
    
    Token * check = Evalexp( temp->right->left, 1, localsymlist ) ;
    
    gPrinter.Printtree( check ) ;
    
    return check ;

  } // Write()
  
  Token * Displaystr( Token * temp, map < string, TokenPtr > & localsymlist ) {
    
    if ( Getsize( temp->right ) != 1 ) {
      throw ArgNumError( "display-string" ) ;
    } // if
    
    Token * check = Evalexp( temp->right->left, 1, localsymlist ) ;
    if ( check->type != STRING )
      throw ArgTypeError( "display-string", check ) ;
    
    string str = check->str ;
    
    str.erase( 0, 1 ) ;
    str.erase( str.size() - 1, 1 ) ;
    
    Token * ans = NewToken( str ) ;
    
    gPrinter.Printtree( ans ) ;
    
    return ans ;

  } // Displaystr()

  Token * Newline( Token * temp, map < string, TokenPtr > & localsymlist ) {
    if ( Getsize( temp->right ) != 0 )
      throw ArgNumError( "newline" ) ;
    
    printf( "\n" ) ;
    
    return NewToken( "nil" ) ;
  } // Newline()
  
  Token * Symtostr( Token * temp, map < string, TokenPtr > & localsymlist ) {
    
    if ( Getsize( temp->right ) != 1 ) {
      throw ArgNumError( "symbol->string" ) ;
    } // if
    
    Token * check = Evalexp( temp->right->left, 1, localsymlist ) ;
    if ( check->type != SYMBOL )
      throw ArgTypeError( "symbol->string", check ) ;
    
    string str = check->str ;
    
    str = "\"" + str ;
    str = str + "\"" ;
    
    Token * ans = NewToken( str ) ;
    
    return ans ;

  } // Symtostr()

  Token * Numtostr( Token * temp, map < string, TokenPtr > & localsymlist ) {
    
    if ( Getsize( temp->right ) != 1 ) {
      throw ArgNumError( "number->string" ) ;
    } // if
    
    Token * check = Evalexp( temp->right->left, 1, localsymlist ) ;
    if ( check->type != INT && check->type != FLOAT )
      throw ArgTypeError( "number->string", check ) ;
    
    string str = check->str ;
    
    str = "\"" + str ;
    str = str + "\"" ;
    
    Token * ans = NewToken( str ) ;
    
    return ans ;

  } // Numtostr()

  Token * Eval( Token * temp, map < string, TokenPtr > & localsymlist ) {
    if ( Getsize( temp->right ) != 1 )
      throw ArgNumError( "eval" ) ;
    
    Token * exp = Evalexp( temp->right->left, 1, localsymlist ) ;
    
    return Evalexp( exp, 0, localsymlist ) ;
  } // Eval()

  Token * Verbose( Token * temp, map < string, TokenPtr > & localsymlist ) {
    if ( Getsize( temp->right ) != 1 )
      throw ArgNumError( "verbose" ) ;
    Token * check ;
    try {
      check = Evalexp( temp->right->left, 1, localsymlist ) ;
    } // try
    catch ( Exception e ) {
      if ( e.mname == "NoReturnError" )
        throw UnboundParaError( e.mhead ) ;
    
      throw e ;
    } // catch

    
    if ( check->type != NIL ) {
      // cout << "turn to on" << endl ;
      gVerbose = true ;
      return NewToken( "#t" ) ;
    } // if
    else {
      // cout << "turn to off" << endl ;
      gVerbose = false ;
      return NewToken( "nil" ) ;
    } // else 
  } // Verbose()

  Token * IsVerb( Token * temp, map < string, TokenPtr > & localsymlist ) {
    if ( Getsize( temp->right ) != 0 )
      throw ArgNumError( "verbose?" ) ;

    if ( gVerbose ) {
      return NewToken( "#t" ) ;
    } // if
    else {
      return NewToken( "nil" ) ;
    } // else 
      
  } // IsVerb()
  
  Token * Createrrobj( Token * temp, map < string, TokenPtr > & localsymlist ) {
    if ( Getsize( temp->right ) != 1 )
      throw ArgNumError( "create-error-object" ) ;

    Token * check ;
    try {
      check = Evalexp( temp->right->left, 1, localsymlist ) ;
    } // try
    catch ( Exception e ) {
      if ( e.mname == "NoReturnError" )
        throw UnboundParaError( e.mhead ) ;
    
      throw e ;
    } // catch

    
    if ( check->type != STRING ) {
      throw ArgTypeError( "create-error-object", check ) ;
      
    } // if
     
    check->type = ERROBJ ;
    return check ;    
    
  } // Createrrobj()

  Token * Primitivepredicates( Token * temp, string str, map < string, TokenPtr > & localsymlist ) {
    if ( Getsize( temp ) != 1 )
      throw ArgNumError( str ) ;
    Token * check ;
    try {
      check = Evalexp( temp->left, 1, localsymlist ) ;
    } // try
    catch ( Exception e ) {
      if ( e.mname == "NoReturnError" )
        throw UnboundParaError( e.mhead ) ;
    
      throw e ;
    } // catch
      
    if ( str == "atom?" ) {
      if ( check->type != DOT )
        return NewToken( "#t" ) ;
      else return NewToken( "nil" ) ;
    } // if
    else if ( str == "pair?" ) {
      if ( check->type == DOT )
        return NewToken( "#t" ) ;
      else return NewToken( "nil" ) ;
    } // else if
    else if ( str == "list?" ) {
      while ( check->right != NULL ) 
        check = check->right ;    
      if ( check->type == NIL )
        return NewToken( "#t" ) ;
      else return NewToken( "nil" ) ;
    } // else if
    else if ( str == "null?" ) {
      if ( check->type == NIL )
        return NewToken( "#t" ) ;
      else return NewToken( "nil" ) ;
    } // else if
    else if ( str == "integer?" ) {
      if ( check->type == INT )
        return NewToken( "#t" ) ;
      else return NewToken( "nil" ) ;
    } // else if
    else if ( str == "real?" ) {
      if ( check->type == INT || check->type == FLOAT )
        return NewToken( "#t" ) ;
      else return NewToken( "nil" ) ;
    } // else if
    else if ( str == "number?" ) {
      if ( check->type == INT || check->type == FLOAT )
        return NewToken( "#t" ) ;
      else return NewToken( "nil" ) ;
    } // else if
    else if ( str == "string?" ) {
      if ( check->type == STRING )
        return NewToken( "#t" ) ;
      else return NewToken( "nil" ) ;
    } // else if
    else if ( str == "boolean?" ) {
      if ( check->type == NIL || check->type == T )
        return NewToken( "#t" ) ;
      else return NewToken( "nil" ) ;
    } // else if
    else if ( str == "symbol?" ) {
      if ( check->type == SYMBOL )
        return NewToken( "#t" ) ;
      else return NewToken( "nil" ) ;
    } // else if
    else if ( str == "error-object?" ) {
      if ( check->type == ERROBJ )
        return NewToken( "#t" ) ;
      else return NewToken( "nil" ) ;
    } // else if
    
    else return NewToken( "nil" ) ;

  } // Primitivepredicates()

  Token * Arith( Token * temp, string str, map < string, TokenPtr > & localsymlist ) {
    if ( Getsize( temp ) < 2 )
      throw ArgNumError( str ) ;
    stringstream ss;
    bool isdouble = false ;
    if ( str == "+" ) {
      double num = 0 ;
      num = Add( temp, isdouble, localsymlist ) ;
      if ( !isdouble )
        ss << ( int ) num ;
      else
        ss << Todouble( num ) ;
      return NewToken( ss.str() ) ;
    } // if
    else if ( str == "-" ) {
      double num = 0 ;
      num = Sub( temp, isdouble, localsymlist ) ;
      if ( !isdouble )
        ss << ( int ) num ;
      else
        ss << Todouble( num ) ;
      return NewToken( ss.str() ) ;
    } // else if
    else if ( str == "*" ) {
      double num = 0 ;
      num = Mul( temp, isdouble, localsymlist ) ;
      if ( !isdouble )
        ss << ( int ) num ;
      else
        ss << Todouble( num ) ;
      return NewToken( ss.str() ) ;
    } // else if
    else if ( str == "/" ) {
      double num = 0 ;
      num = Div( temp, isdouble, localsymlist ) ;
      if ( !isdouble )
        ss << ( int ) num ;
      else
        ss << Todouble( num ) ;
      return NewToken( ss.str() ) ;
    } // else if
    else if ( str == ">" ) {
      return Greater( temp, localsymlist ) ;
    } // else if
    else if ( str == "<" ) {
      return Less( temp, localsymlist ) ;
    } // else if
    else if ( str == ">=" ) {
      return Noless( temp, localsymlist ) ;
    } // else if
    else if ( str == "<=" ) {
      return Nogreater( temp, localsymlist ) ;
    } // else if
    else if ( str == "=" ) {
      return Equalnum( temp, localsymlist ) ;
    } // else if
    else if ( str == "string-append" ) {
      return Strappend( temp, localsymlist ) ;
    } // else if
    else if ( str == "string>?" ) {
      return Strgreat( temp, localsymlist ) ;
    } // else if
    else if ( str == "string<?" ) {
      return Strless( temp, localsymlist ) ;
    } // else if
    else if ( str == "string=?" ) {
      return Strequal( temp, localsymlist ) ;
    } // else if
    else if ( str == "or" ) {
      return Or( temp, localsymlist ) ;
    } // else if
    else if ( str == "and" ) {
      return And( temp, localsymlist ) ;
    } // else if
    else return NewToken( "nil" ) ;

  } // Arith()
 
  double Add( Token * temp, bool & isdouble, map < string, TokenPtr > & localsymlist ) {
    if ( temp->left != NULL ) {
      Token * check ;
      try {
        check = Evalexp( temp->left, 1, localsymlist ) ;
      } // try
      catch ( Exception e ) {
        if ( e.mname == "NoReturnError" )
          throw UnboundParaError( e.mhead ) ;
    
        throw e ;
      } // catch
      if ( check->type == FLOAT )
        isdouble = true ;
      else if ( check->type != INT && check->type != FLOAT )
        throw ArgTypeError( "+", check ) ;
      return Getnum( check ) + Add( temp->right, isdouble, localsymlist ) ;
    } // if
    else {
      double z = 0 ;
      return z ;
    } // else    
  } // Add()

  double Sub( Token * temp, bool & isdouble, map < string, TokenPtr > & localsymlist ) {
    if ( temp->left != NULL ) {
      Token * check ;
      try {
        check = Evalexp( temp->left, 1, localsymlist ) ;
      } // try
      catch ( Exception e ) {
        if ( e.mname == "NoReturnError" )
          throw UnboundParaError( e.mhead ) ;
    
        throw e ;
      } // catch
      
      double num = Getnum( check ) ;
      if ( check->type == FLOAT )
        isdouble = true ;
      check = temp->right ;
      while ( check != NULL && check->type != NIL ) {
        Token * check2 ;
        try {
          check2 = Evalexp( check->left, 1, localsymlist ) ;
        } // try
        catch ( Exception e ) {
          if ( e.mname == "NoReturnError" )
            throw UnboundParaError( e.mhead ) ;
      
          throw e ;
        } // catch
        if ( check2->type == FLOAT )
          isdouble = true ;
        else if ( check2->type != INT && check2->type != FLOAT )
          throw ArgTypeError( "-", check2 ) ;
        num -= Getnum( check2 ) ;

        check = check->right ;
        
      } // while

      return num ;

    } // if
    else {
      double z = 0 ;
      return z ;
    } // else    
  } // Sub()

  double Mul( Token * temp, bool & isdouble, map < string, TokenPtr > & localsymlist ) {
    if ( temp->left != NULL ) {
      Token * check ;
      try {
        check = Evalexp( temp->left, 1, localsymlist ) ;
      } // try
      catch ( Exception e ) {
        
        if ( e.mname == "NoReturnError" )
          throw UnboundParaError( e.mhead ) ;
    
        throw e ;
      } // catch
      if ( check->type == FLOAT )
        isdouble = true ;
      else if ( check->type != INT && check->type != FLOAT )
        throw ArgTypeError( "*", check ) ;

      return Getnum( check ) * Mul( temp->right, isdouble, localsymlist ) ;
    } // if
    else {
      double z = 1 ;
      return z ;
    } // else   
  } // Mul()

  double Div( Token * temp, bool & isdouble, map < string, TokenPtr > & localsymlist ) {
    if ( temp->left != NULL ) {
      Token * check ;
      try {
        check = Evalexp( temp->left, 1, localsymlist ) ;
      } // try
      catch ( Exception e ) {
        if ( e.mname == "NoReturnError" )
          throw UnboundParaError( e.mhead ) ;
    
        throw e ;
      } // catch
      double num = Getnum( check ) ;
      if ( check->type == FLOAT )
        isdouble = true ;
      check = temp->right ;
      while ( check != NULL && check->type != NIL ) {
        Token * check2 ;
        try {
          check2 = Evalexp( check->left, 1, localsymlist ) ;
        } // try
        catch ( Exception e ) {
          if ( e.mname == "NoReturnError" )
            throw UnboundParaError( e.mhead ) ;
      
          throw e ;
        } // catch
        if ( check2->type == FLOAT )
          isdouble = true ;
        else if ( check2->type != INT && check2->type != FLOAT )
          throw ArgTypeError( "/", check2 ) ;

        double number = Getnum( check2 ) ;
        if ( number == 0 )
          throw DivideZeroError() ;        

        num /= number ;

        check = check->right ;
        
      } // while

      return num ;

    } // if
    else {
      double z = 0 ;
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

  public:
  
  Token * Evalexp( Token * temp, int head, map < string, TokenPtr > & localsymlist ) {


    if ( Isatomtype( temp->type ) ) {
      string str = temp->str ;
      
      if ( temp->type == SYMBOL ) {
        if ( Findsymbol( str, localsymlist ) == 1 ) {
          Token * retoken = Symbols( temp, localsymlist ) ;
          return retoken ; 

        } // if
        else if ( Isspfunc( str ) || Isinternalfunc( str ) || Findsymbol( str, localsymlist ) == 2 ) {
          temp->iscomd = true ;
          return temp ;
        }  // else if
        else 
          throw UnboundError( str ) ;
      } // if
      else
        return temp ;
    } // if
    else if ( temp->left != NULL ) {
      if ( !Islist( temp ) ) {
        throw NonListError() ;
      } // if
      
      try {
        if ( temp->left->str == "clean-environment" ) {
          if ( head != 0 )
            throw LevelError( "CLEAN-ENVIRONMENT" ) ;
          // else if ( Getsize( temp->right ) != 0 )
          msymbollist.clear() ;
          
          if ( gVerbose )
            printf( "%s\n", "environment cleaned" ) ;
          return NULL ; 
        } // if
        else if ( temp->left->str == "define" ) {
          if ( head != 0 )
            throw LevelError( "DEFINE" ) ;
          
          return Define( temp, localsymlist ) ;    
        } // else if
        else if ( temp->left->str == "set!" ) {
          return Set( temp, localsymlist ) ;    
        } // else if
        else if ( temp->left->str == "exit" ) {
          if ( head != 0 )
            throw LevelError( "EXIT" ) ;
          return Exit( temp->right ) ;    
        } // else if
        else if ( temp->left->type == QUOTE || temp->left->str == "quote" )
          return Quote( temp->right ) ;   
        else if ( temp->left->str == "cons" )
          return Cons( temp->right, localsymlist ) ;
        else if ( temp->left->str == "list" )
          return List( temp->right, localsymlist ) ;
        else if ( temp->left->str == "car" )
          return Car( temp->right, localsymlist ) ;  
        else if ( temp->left->str == "cdr" )
          return Cdr( temp->right, localsymlist ) ; 
        else if ( temp->left->str == "not" )
          return Not( temp->right, localsymlist ) ; 
        else if ( Isprimitivepredicates( temp->left->str ) ) 
          return Primitivepredicates( temp->right, temp->left->str, localsymlist ) ; 
        else if ( IsArith( temp->left->str ) ) 
          return Arith( temp->right, temp->left->str, localsymlist ) ; 
        else if ( temp->left->str == "eqv?" ) {
          return Eqv( temp->right, localsymlist ) ;
        } // else if
        else if ( temp->left->str == "equal?" ) {
          return Equal( temp->right, localsymlist ) ;
        } // else if
        else if ( temp->left->str == "if" ) {
          return If( temp, localsymlist ) ;
        } // else if
        else if ( temp->left->str == "cond" ) {
          return Cond( temp, localsymlist ) ;
        } // else if
        else if ( temp->left->str == "begin" ) {
          return Begin( temp->right, localsymlist ) ;
        } // else if
        else if ( temp->left->str == "lambda" ) {
          if ( temp->left->right == NULL )
            return Setlambda( temp, head, localsymlist ) ;
          else
            return Lambda( temp, head, localsymlist ) ;
        } // else if
        else if ( temp->left->str == "let" ) {
          return Let( temp, localsymlist ) ;
        } // else if
        else if ( temp->left->str == "eval" ) {
          return Eval( temp, localsymlist ) ;
        } // else if
        else if ( temp->left->str == "read" ) {
          return Read( temp, localsymlist ) ;
        } // else if
        else if ( temp->left->str == "write" ) {
          return Write( temp, localsymlist ) ;
        } // else if
        else if ( temp->left->str == "display-string" ) {
          return Displaystr( temp, localsymlist ) ;
        } // else if
        else if ( temp->left->str == "newline" ) {
          return Newline( temp, localsymlist ) ;
        } // else if
        else if ( temp->left->str == "symbol->string" ) {
          return Symtostr( temp, localsymlist ) ;
        } // else if
        else if ( temp->left->str == "number->string" ) {
          return Numtostr( temp, localsymlist ) ;
        } // else if
        else if ( temp->left->str == "verbose" ) {
          return Verbose( temp, localsymlist ) ;
        } // else if
        else if ( temp->left->str == "verbose?" ) {
          return IsVerb( temp, localsymlist ) ;
        } // else if
        else if ( temp->left->str == "create-error-object" ) {
          return Createrrobj( temp, localsymlist ) ;
        } // else if
        else if ( temp->left->type == SYMBOL ) {

          string str = temp->left->str ;
          int mode = Findsymbol( str, localsymlist ) ;
          Token * sym = Symbols( temp->left, localsymlist ) ;
          
          if ( mode != 0 ) {
            if ( mode == 2 ) {

              return Customfunc( temp, head, localsymlist ) ;
            } // if
            else if ( Findsymbol( sym->str, localsymlist ) == 2 ) {
              Token * ntemp = NewToken( "." ) ;
              ntemp->left = Symbols( temp->left, localsymlist ) ;
              ntemp->right = temp->right ;
              return Evalexp( ntemp, head, localsymlist ) ;
            } // else if
            else if ( ( Isspfunc( sym->str ) || Isinternalfunc( sym->str ) ) && sym->iscomd ) {
              // cout <<"gogo" ;
              Token * ntemp = NewToken( "." ) ;
              ntemp->left = Symbols( temp->left, localsymlist ) ;
              ntemp->right = temp->right ;
              
              return Evalexp( ntemp, head, localsymlist ) ;
            } // else if
            else if ( sym->left != NULL && sym->left->str == "lambda" ) {
              Token * ntemp = NewToken( "." ) ;
              ntemp->left = Symbols( temp->left, localsymlist ) ;
              ntemp->right = temp->right ;
              
              return Evalexp( ntemp, head, localsymlist ) ;
            } // else if
            else {
              throw NonFuncError( Symbols( temp->left, localsymlist ) ) ;
            } // else
          } // if
          else {
            string str = temp->left->str ;
            throw UnboundError( str ) ;
          } // else
          
          
          return temp ;
        } // else if
        else if ( temp->left->type == DOT ) {
          // if ( temp->left->left->str == "lambda" ) {
          if ( 0 == 1 ) {
            return Lambda( temp, head, localsymlist ) ;

          } // if
          else {
            Token * check ;
            try {
              check = Evalexp( temp->left, head + 1, localsymlist ) ;
            } // try 
            catch ( Exception e ) {
              if ( e.mname == "NoReturnError" ) {

                e.mname = "NoReturnErrorgogo" ;

                throw e ;
              } // if
              
              throw e ;
            } // catch
            temp->left = check ;
            if ( check->type == SYMBOL  ) {
              if ( !check->iscomd )
                throw NonFuncError( temp->left ) ;
            } // if
            else if ( check->left != NULL && check->left->str != "lambda" )
              throw NonFuncError( temp->left ) ;

            return Evalexp( temp, head, localsymlist ) ;
          } // else

        } // else if
        else {
          throw NonFuncError( temp->left ) ;
        } // else
      } // try
      catch ( Exception e ) {
        if ( e.mname == "NoReturnError" )
          e.mhead = temp ;
        throw e ;
      } // catch        
      
    } // else if         
    else 
      return temp ;
      
      
  } // Evalexp()
  
};

class Interpreter{
  public:
   
  
  Interpreter() {
    // mch = '\0' ;
    // gPrinter = Printer() ;
    // gScanner = Scanner() ;
    // gTreemaker = Treemaker() ;
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
        while ( Iswhitespace( gScanner.Getch() ) && gScanner.Getch() != '\n' && !gEnd )
          gScanner.Getchar() ; 
        
        gScanner.ReadSexp(  mtokenlist ) ;
        
      } // try
      catch ( Stringerror e ) {
        printf( "%s\n", e.merrorstr.c_str() ) ;
        gReading = false ;
        while ( !gEnd && gScanner.Getch() != '\n' )
          gScanner.Getchar() ;
          
        mtokenlist.clear() ;
        err = true ;
      } // catch
      
      gReading = false ;
      

      if ( gEnd && mtokenlist.size() == 1 ) {
        if ( mtokenlist.at( 0 ).str[0] == '\0' )
          throw EndOfFileError() ;
      } // if
      
      if ( !err ) {
        bool evalerr = false ;
        gTreemaker.Buildtree( mtokenlist, morigintree ) ; 
        mtokentree = SetTree( 1, morigintree ) ;
        Token * outtree ;
        // vector< Symbol > localsymlist ;
        map < string, TokenPtr > localsymlist ;
        localsymlist.clear() ;
        try {
          
          outtree = mevaler.Evalexp( mtokentree, 0, localsymlist ) ;
           
          
        }
        catch ( Exception e ) {
          if ( e.mname == "LevelError" || e.mname == "ArgNumError" ||
               e.mname == "UnboundError" || e.mname == "DivideZeroError" ) {
            printf( "%s", e.merrorstr.c_str() ) ;
            evalerr = true ;
          } // if
          else if ( e.mname == "NonFuncError" || e.mname == "ArgTypeError" ||
                    e.mname == "NoReturnError" || e.mname == "UnboundParaError" ) {
            printf( "%s", e.merrorstr.c_str() ) ;
            evalerr = true ;
            gPrinter.Printtree( e.mhead ) ; 
          } // else if
          else if ( e.mname == "NoReturnErrorgogo" ) {
            printf( "%s", e.merrorstr.c_str() ) ;
            evalerr = true ;
            gPrinter.Printtree( e.mhead ) ; 
          } // else if
          else if ( e.mname == "FormatError" || e.mname == "UnboundCondiError" ) {
            printf( "%s", e.merrorstr.c_str() ) ;
            evalerr = true ;
            gPrinter.Printtree( e.mhead ) ; 
          } // else if
          else if ( e.mname == "NonListError" ) {
            printf( "%s", e.merrorstr.c_str() ) ;
            evalerr = true ;
            gPrinter.Printtree( mtokentree ) ;
          } // else if
          else { // if ( e.mname == "Callend" || e.mname == "EndOfFileError" )
            throw e ;
          } // else if
          
          printf( "\n" ) ;
        } // catch
        
        if ( !evalerr && outtree != NULL ) {
          gPrinter.Printtree( outtree ) ; 
          printf( "\n" ) ;
        } // if
          
        
      } // if

      printf( "\n" ) ;
      
      int c = gColumn ;
      char ch = gScanner.Getch() ;
      while ( Iswhitespace( ch ) && ch != '\n' ) {
        gScanner.Getchar() ;
        ch = gScanner.Getch() ;
      } // while
      
      ch = gScanner.Getch() ;
      if ( ch == ';' ) {
        while ( ch != '\n' && !gEnd ) {
          gScanner.Getchar() ;
          ch = gScanner.Getch() ;
        } // while
      } // if

      if ( gScanner.Getch() == '\n' )
        gColumn = 0 ; 
      else
        gColumn = gColumn - c + 1 ;
         

    } // while()
    

  } // Gettokenlist()
  
  private:
  

  Evaler mevaler ;

  vector<Token> mtokenlist ;
  map< int, Token > morigintree ;
  Token * mtokentree ;  
    
};

int main() {
  
  Interpreter interpreter = Interpreter() ;
  gPrinter = Printer() ;
  gScanner = Scanner() ;
  gTreemaker = Treemaker() ;
  char t ;
  scanf( "%d",  &gTestNum ) ;
  scanf( "%c",  &t ) ;
  
  Token * temp = NULL ;

  printf( "Welcome to OurScheme!\n\n" ) ;
  try {
    // if ( gTestNum != 2 ) 
    interpreter.Gettokenlist() ;
  } // try
  catch ( Exception e ) {
    if ( e.mname == "EndOfFileError" )
      printf( "%s", e.merrorstr.c_str() ) ;
  } // catch

  printf( "\nThanks for using OurScheme!" ) ;
  
  // cout << gCount << endl ;
} // main()
