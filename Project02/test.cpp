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
bool gEnd = false ;

struct Token {
  string str ;
  
  int line ;
  int column ;
  int intnum ;
  float floatnum ;
  Type type ;
  
  Token * left ;
  Token * right ;
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
  else if ( str == "'" || str == "quote" ) return QUOTE ;
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
  NonFuncError( string str ) {
    stringstream ss ;

    ss << "ERROR (attempt to apply non-function) : " << str ;

    merrorstr = ss.str() ;
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

class Evaler {
  private:
    
  vector< Symbol > msymbollist ;
    
  Token * NewToken( string str ) {
    Token * retoken = new Token ;
    retoken->str = Setstr( str ) ;
    retoken->type = Gettype( retoken->str ) ;
    retoken->intnum = Decodeint( retoken->str ) ;
    retoken->floatnum = Decodefloat( retoken->str ) ;
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
  
  bool Findsymbol( string str ) {
    int i = msymbollist.size() - 1 ;
    
    while ( i >= 0 ) {
      if ( msymbollist.at( i ).name == str )
        return true ;
      i-- ;  
      
    } // while
    
    return false ;
  } // Findsymbol()
  
  Token * Interfunc( Token * temp ) {
    stringstream ss ;
    ss << "#<procedure " << temp->str << ">" ;
    return NewToken( ss.str() ) ;
  } // Interfunc()
  
  Token * Symbols( Token * temp ) {
    
    int i = msymbollist.size() - 1 ;
    bool find = false ;
    while ( i >= 0 ) {
      // cout << msymbollist.at(i).name << endl ;
      if ( msymbollist.at( i ).name == temp->str )  {
        find = true ;
        return msymbollist.at( i ).info ;
      } // if
      
    
      i-- ;  
      
    } // while
    
    if ( !find ) {
    
      throw UnboundError( temp->str ) ;
    
    } // if 
    
    return NULL ;
  } // Symbols()

  Token * Define( Token * temp ) {
    
    if ( Getsize( temp ) != 2 ) {
      throw ArgNumError( "define" ) ;
    } // if
    
    Symbol newsymbol ;
    string name = temp->left->str ;
    newsymbol.name = name ;
<<<<<<< HEAD

    newsymbol.info = Evalexp( temp->right->left ) ;

      
    if ( !Findsymbol( name ) )
      msymbollist.push_back( newsymbol ) ;  
    else {
      for ( int i = msymbollist.size() - 1 ; i >= 0 ; i-- ) {

        if ( msymbollist.at( i ).name == name ) {

          msymbollist.at( i ).info = temp->right->left ;
        } // if
      } // for

    } // else

=======
    newsymbol.info = Evalexp( temp->right->left ) ;
    msymbollist.push_back( newsymbol ) ;  
    
>>>>>>> parent of 829c85e (>)
    return NewToken( name + " defined" ) ;
  } // Define()

  Token * Quote( Token * temp ) {
    return temp->left ; 
  } // Quote()

  Token * Cons( Token * temp ) {
    if ( Getsize( temp ) != 2 )
      throw ArgNumError( "cons" ) ;
    
    Token * retoken = NewToken( "." ) ;
    retoken->left = Evalexp( temp->left ) ;
    retoken->right = Evalexp( temp->right->left );
    return retoken ;  
  } // Cons()

  Token * List( Token * temp ) {
    // if(Getsize(temp) != 2)
    //   throw ArgNumError("cons") ;
    // not done
    Token * retoken = NewToken( "." ) ;
    Token * ret = retoken ;
    Token * t = temp ;

    while ( t->type != NIL ) {
      ret->left = Evalexp( t->left ) ; 
    
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
    
    return Evalexp( temp->left )->left ;
    
  } // Car()

  Token * Cdr( Token * temp ) {
    if ( Getsize( temp ) != 1 )
      throw ArgNumError( "cdr" ) ;
    
    return Evalexp( temp->left )->right ;
    
  } // Cdr()
  
  Token * Not( Token * temp ) {
    if ( Getsize( temp ) != 1 )
      throw ArgNumError( "not" ) ;
    
    if ( Evalexp( temp->left )->type == NIL )     
      return NewToken( "#t" ) ;
    else 
      return NewToken( "nil" ) ;
    
  } // Not()
  
  Token * Greater( Token * temp ) {
    string check = "#t" ;
    while ( temp->right->type != NIL && temp->right != NULL ) {
      if ( Getnum( Evalexp( temp->left ) ) <= Getnum( Evalexp( temp->right->left ) ) )
        check = "nil" ;
      
      temp = temp->right ;
    } // while

    return NewToken( check ) ;

  } // Greater()

  Token * Less( Token * temp ) {
    string check = "#t" ;
    while ( temp->right->type != NIL && temp->right != NULL ) {
      if ( Getnum( Evalexp( temp->left ) ) >= Getnum( Evalexp( temp->right->left ) ) )
        check = "nil" ;
      
      temp = temp->right ;
    } // while

    return NewToken( check ) ;

  } // Less()  

  Token * Nogreater( Token * temp ) {
    string check = "#t" ;
    while ( temp->right->type != NIL && temp->right != NULL ) {
      if ( Getnum( Evalexp( temp->left ) ) > Getnum( Evalexp( temp->right->left ) ) )
        check = "nil" ;
      
      temp = temp->right ;
    } // while

    return NewToken( check ) ;

  } // Nogreater()

  Token * Noless( Token * temp ) {
    string check = "#t" ;
    while ( temp->right->type != NIL && temp->right != NULL ) {
      if ( Getnum( Evalexp( temp->left ) ) < Getnum( Evalexp( temp->right->left ) ) )
        check = "nil" ;
      
      temp = temp->right ;
    } // while

    return NewToken( check ) ;

  } // Noless()
  
  Token * Equalnum( Token * temp ) {
    string check = "#t" ;
    while ( temp->right->type != NIL && temp->right != NULL ) {
      if ( Getnum( Evalexp( temp->left ) ) != Getnum( Evalexp( temp->right->left ) ) )
        check = "nil" ;
      
      temp = temp->right ;
    } // while

    return NewToken( check ) ;

  } // Equalnum()

  Token * Strappend( Token * temp ) {
    string check = "" ;
    while ( temp->type != NIL && temp != NULL ) {
      check = Mergestr( check, Evalexp( temp->left )->str ) ;
      temp = temp->right ;
    } // while

    return NewToken( check ) ;

  } // Strappend()

  Token * Strgreat( Token * temp ) {
    string check = "#t" ;
    while ( temp->right->type != NIL && temp->right != NULL ) {
      if ( Evalexp( temp->left )->str.compare( Evalexp( temp->right->left )->str ) <= 0 )
        check = "nil" ;
      
      temp = temp->right ;
    } // while

    return NewToken( check ) ;

  } // Strgreat()

  Token * Strless( Token * temp ) {
    string check = "#t" ;
    while ( temp->right->type != NIL && temp->right != NULL ) {
      if ( Evalexp( temp->left )->str.compare( Evalexp( temp->right->left )->str ) >= 0 )
        check = "nil" ;
      
      temp = temp->right ;
    } // while

    return NewToken( check ) ;

  } // Strless()

  Token * Strequal( Token * temp ) {
    string check = "#t" ;
    while ( temp->right->type != NIL && temp->right != NULL ) {
      if ( Evalexp( temp->left )->str.compare( Evalexp( temp->right->left )->str ) != 0 )
        check = "nil" ;
      
      temp = temp->right ;
    } // while

    return NewToken( check ) ;

  } // Strequal()

  Token * Eqv( Token * temp ) {
    if ( Getsize( temp ) != 2 )
      throw ArgNumError( "equ?" ) ;
    Token * check1 = Evalexp( temp->left ) ;
    Token * check2 = Evalexp( temp->right->left ) ;
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
    
    Token * check1 = Evalexp( temp->left ) ;
    Token * check2 = Evalexp( temp->right->left ) ;
    if ( Issame( check1, check2 ) ) 
      return NewToken( "#t" ) ;
    else 
      return NewToken( "nil" ) ;

  } // Equal()
  
  Token * If( Token * temp ) {
    int size = Getsize( temp ) ;
    if ( size != 2 && size != 3 )
      throw ArgNumError( "if" ) ;
    
    Token * check = Evalexp( temp->left ) ;
    Token * result ;

    if ( check->type != NIL ) {
      result = Evalexp( temp->right->left ) ;
    } // if
    else {
      if ( size == 2 )
        throw ArgNumError( "if" ) ;
      else {
        result = Evalexp( temp->right->right->left ) ;
      } // else

    } // else

    return result ;

  } // If()

  Token * Decide( Token * temp, bool last, bool & done ) {

    if ( Getsize( temp ) < 1 )
      throw ArgNumError( "cond" ) ;

    Token * check ;
    Token * result ;
    if ( ( last && temp->left->str == "else" ) || Evalexp( temp->left )->type != NIL ) {
      done = true ;
      return Begin( temp->right ) ;
    } // if
    else {
      done = false ;
      return NewToken( "nil" ) ;
    } // else

  } // Decide()

  Token * Cond( Token * temp ) {
    int size = Getsize( temp ) ;
    if ( Getsize( temp ) < 1 )
      throw ArgNumError( "cond" ) ;
    
    Token * t = temp ;
    bool check, done ;
    Token * result ;

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

    throw ArgNumError( "noreturn" ) ;

  } // Cond()
  
  Token * Begin( Token * temp ) {
    int size = Getsize( temp ) ;
    if ( Getsize( temp ) < 1 )
      throw ArgNumError( "Begin" ) ;
    
    Token * t = temp ;
    bool check, done ;
    Token * result ;

    while ( t != NULL && t->type != NIL ) {

      result = Evalexp( t->left ) ;
      t = t->right ;
    } // while

    return result ;

  } // Begin()

  Token * Or( Token * temp ) {
    int size = Getsize( temp ) ;
    if ( Getsize( temp ) < 2 )
      throw ArgNumError( "Or" ) ;
    
    Token * t = temp ;
    bool check, done ;
    Token * result ;
    while ( t != NULL && t->type != NIL ) {

      result = Evalexp( t->left ) ;
      if ( result->type != NIL )
        return result ;

      t = t->right ;
    } // while

    return result ;

  } // Or()

  Token * And( Token * temp ) {
    int size = Getsize( temp ) ;
    if ( Getsize( temp ) < 2 )
      throw ArgNumError( "Begin" ) ;
    
    Token * t = temp ;
    bool check, done ;
    Token * result ;

    while ( t != NULL && t->type != NIL ) {

      result = Evalexp( t->left ) ;
      if ( result->type == NIL )
        return result ;
      t = t->right ;
    } // while

    return result ;

  } // And()

  Token * Primitivepredicates( Token * temp, string str ) {
    if ( Getsize( temp ) != 1 )
      throw ArgNumError( "***" ) ;
    
    if ( str == "atom?" ) {
      if ( Evalexp( temp->left )->type != DOT )
        return NewToken( "#t" ) ;
      else return NewToken( "nil" ) ;
    } // if
    else if ( str == "pair?" ) {
      Token * check = Evalexp( temp->left ) ;
      if ( check->type == DOT )
        return NewToken( "#t" ) ;
      else return NewToken( "nil" ) ;
    } // else if
    else if ( str == "list?" ) {
      Token * check = Evalexp( temp->left ) ;
      while ( check->right != NULL ) 
        check = check->right ;    
      if ( check->type == NIL )
        return NewToken( "#t" ) ;
      else return NewToken( "nil" ) ;
    } // else if
    else if ( str == "null?" ) {
      Token * check = Evalexp( temp->left ) ;
      if ( check->type == NIL )
        return NewToken( "#t" ) ;
      else return NewToken( "nil" ) ;
    } // else if
    else if ( str == "integer?" ) {
      Token * check = Evalexp( temp->left ) ;
      if ( check->type == INT )
        return NewToken( "#t" ) ;
      else return NewToken( "nil" ) ;
    } // else if
    else if ( str == "real?" ) {
      Token * check = Evalexp( temp->left ) ;
      if ( check->type == INT || check->type == FLOAT )
        return NewToken( "#t" ) ;
      else return NewToken( "nil" ) ;
    } // else if
    else if ( str == "number?" ) {
      Token * check = Evalexp( temp->left ) ;
      if ( check->type == INT || check->type == FLOAT )
        return NewToken( "#t" ) ;
      else return NewToken( "nil" ) ;
    } // else if
    else if ( str == "string?" ) {
      Token * check = Evalexp( temp->left ) ;
      if ( check->type == STRING )
        return NewToken( "#t" ) ;
      else return NewToken( "nil" ) ;
    } // else if
    else if ( str == "boolean?" ) {
      Token * check = Evalexp( temp->left ) ;
      if ( check->type == NIL || check->type == T )
        return NewToken( "#t" ) ;
      else return NewToken( "nil" ) ;
    } // else if
    else if ( str == "symbol?" ) {
      Token * check = Evalexp( temp->left ) ;
      if ( check->type == SYMBOL )
        return NewToken( "#t" ) ;
      else return NewToken( "nil" ) ;
    } // else if
    else return NewToken( "nil" ) ;

  } // Primitivepredicates()

  Token * Arith( Token * temp, string str ) {
    if ( Getsize( temp ) < 2 )
      throw ArgNumError( "***" ) ;
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
    string str = Symbols( temp->left )->str ;
    throw NonFuncError( str ) ;

  } // Func()
 
  float Add( Token * temp, bool & isfloat ) {
    if ( temp->left != NULL ) {
      Token * check = Evalexp( temp->left ) ;
      if ( check->type == FLOAT )
        isfloat = true ;
      return Getnum( check ) + Add( temp->right, isfloat ) ;
    } // if
    else {
      float z = 0 ;
      return z ;
    } // else    
  } // Add()

  float Sub( Token * temp, bool & isfloat ) {
    if ( temp->left != NULL ) {
      Token * check = Evalexp( temp->left ) ;
      float num = Getnum( check ) ;
      if ( check->type == FLOAT )
        isfloat = true ;
      check = temp->right ;
      while ( check != NULL && check->type != NIL ) {
        Token * check2 = Evalexp( check->left ) ;
        if ( check2->type == FLOAT )
          isfloat = true ;
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
      Token * check = Evalexp( temp->left ) ;
      if ( check->type == FLOAT )
        isfloat = true ;
      return Getnum( check ) * Mul( temp->right, isfloat ) ;
    } // if
    else {
      float z = 1 ;
      return z ;
    } // else   
  } // Mul()

  float Div( Token * temp, bool & isfloat ) {
    if ( temp->left != NULL ) {
      Token * check = Evalexp( temp->left ) ;
      float num = Getnum( check ) ;
      if ( check->type == FLOAT )
        isfloat = true ;
      check = temp->right ;
      while ( check != NULL && check->type != NIL ) {
        Token * check2 = Evalexp( check->left ) ;
        if ( check2->type == FLOAT )
          isfloat = true ;
        num /= Getnum( check2 ) ;

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

  public:
  /*
  Token * Evalexp( Token * temp ) {
    
    if ( temp->left != NULL ) {
      if ( temp->left->str == "clean-environment" ) {
        msymbollist.clear() ;
        return NewToken( "environment cleaned" ) ;  
      } // if
      else if ( temp->left->str == "define" )
        return Define( temp->right ) ;    
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
        return If( temp->right ) ;
      } // else if
      else if ( temp->left->str == "cond" ) {
        return Cond( temp->right ) ;
      } // else if
      else if ( temp->left->str == "begin" ) {
        return Begin( temp->right ) ;
      } // else if
      else
        throw NonFuncError( Symbols( temp->left )->str ) ;
    } // if 
    else if ( temp->type == SYMBOL ) {
      return Symbols( temp );
    } // else if
            
    else 
      return temp ;
      
      
  } // Evalexp()
  */

  Token * Evaluate( Token * temp, Token * check ) {
    
    if ( check != NULL ) {
      if ( check->str == "clean-environment" ) {
        msymbollist.clear() ;
        return NewToken( "environment cleaned" ) ;  
      } // if
      else if ( check->str == "define" )
        return Define( temp->right ) ;    
      else if ( check->type == QUOTE || check->str == "quote" )
        return Quote( temp->right ) ;   
      else if ( check->str == "cons" )
        return Cons( temp->right ) ;
      else if ( check->str == "list" )
        return List( temp->right ) ;
      else if ( check->str == "car" )
        return Car( temp->right ) ;  
      else if ( check->str == "cdr" )
        return Cdr( temp->right ) ; 
      else if ( check->str == "not" )
        return Not( temp->right ) ; 
      else if ( Isprimitivepredicates( check->str ) ) {
        // cout << "*" << check->str << endl ;
        return Primitivepredicates( temp->right, check->str ) ;
      } // else if   
      else if ( IsArith( check->str ) ) 
        return Arith( temp->right, check->str ) ; 
      else if ( check->str == "eqv?" ) {
        return Eqv( temp->right ) ;
      } // else if
      else if ( check->str == "equal?" ) {
        return Equal( temp->right ) ;
      } // else if
      else if ( temp->left->str == "if" ) {
        return If( temp->right ) ;
      } // else if
      else if ( temp->left->str == "cond" ) {
        return Cond( temp->right ) ;
      } // else if
      else if ( temp->left->str == "begin" ) {
        return Begin( temp->right ) ;
      } // else if
      else
        throw NonFuncError( Symbols( temp->left )->str ) ;
    } // if 
    else if ( temp->type == SYMBOL ) {
      return Symbols( temp );
    } // else if
            
    else 
      return temp ;
      
      
  } // Evaluate()
  
  Token * Evalexp( Token * temp ) {
    if ( Isatomtype( temp->type ) && temp->type != SYMBOL )
      return temp ;
    else if ( temp->type == SYMBOL ) {
      if ( Findsymbol( temp->str ) )
        return Symbols( temp ) ;
      else if ( Isinternalfunc( temp->str ) )
        return Interfunc( temp ) ;
      else 
        throw UnboundError( temp->str ) ;
      
    } // else if
    else { // what is being evaluated is >>>(...)<<< ; we call it the main S-expression below
      
      if ( !Islist( temp ) ) // (...) is not a (pure) list
        throw NonListError() ;  // (...)要pretty print
      else if ( Isatomtype( temp->left->type ) && temp->left->type != SYMBOL )
        throw NonFuncError( temp->left->str ) ;
      else if ( temp->left->type == SYMBOL || temp->left->type == QUOTE ) {
        string sym = temp->left->str ;
        // check whether SYM is the name of a function (i.e., check whether 「SYM has a
        //                                binding, and that binding is an internal function」)
        
        // cout << sym ;
        
        if ( Isinternalfunc( sym ) || Isspfunc( sym ) ) { // SYM is the name of a known function
          
          // if ( the current level is not the top level, and SYM is 'clean-environment' or 
          // 'define' or　'exit' ) {
          //   ERROR (level of CLEAN-ENVIRONMENT) / ERROR (level of DEFINE) / ERROR (level of EXIT)
          if ( 1 == 0 ) ;
          else if ( sym == "define" || sym == "set!" || sym == "let" || 
                    sym == "cond" || sym == "lambda" ) { 
                    // SYM is 'define' or 'set!' or 'let' or 'cond' or 'lambda'
                    
          // check the format of this expression // 注意：此時尚未check num-of-arg
          // (define symbol    // 注意：只能宣告或設定 非primitive的symbol (這是final decision!)
          //         S-expression
          // )
          // (define ( one-or-more-symbols )
          //           one-or-more-S-expressions
          // )
          // (set! symbol
          //       S-expression
          // )
          // (lambda (zero-or-more-symbols)
          //           one-or-more-S-expressions
          // )
          // (let (zero-or-more-PAIRs)
          //        one-or-more-S-expressions
          // )
          // (cond one-or-more-AT-LEAST-DOUBLETONs
          // )
          // where PAIR df= ( symbol S-expression )
          //        AT-LEAST-DOUBLETON df= a list of two or more S-expressions
                    
            /*
            if ( 1 == 0 ) { // format error (包括attempting to redefine system primitive)
              ERROR (COND format) : <the main S-exp> 
              or
              ERROR (DEFINE format) : <the main S-exp> // 有可能是因為redefining primitive之故
              or
              ERROR (SET! format) : <the main S-exp>    // 有可能是因為redefining primitive之故
              or
              ERROR (LET format) : <the main S-exp>     // 有可能是因為redefining primitive之故
              or
              ERROR (LAMBDA format) : <the main S-exp>  // 有可能是因為redefining primitive之故
            } // if
            */
            

            return Evaluate( temp, temp->left ) ;
          } // else if
          else if ( sym == "if" || sym == "and" || sym == "or" ) { // SYM is 'if' or 'and' or 'or'
            /*
            check whether the number of arguments is correct

            if number of arguments is NOT correct
              ERROR (incorrect number of arguments) : if
            */
            return Evaluate( temp, temp->left ) ;
          } // else if
          else { // SYM is a known function name 'abc', which is neither 
                 // 'define' nor 'let' nor 'cond' nor 'lambda'

            /*
            check whether the number of arguments is correct

            if number of arguments is NOT correct
              ERROR (incorrect number of arguments) : abc
            */     

            return Evaluate( temp, NewToken( sym ) ) ;          
          } // else
        } // if ( SYM is the name of a known function )
        else if ( !Findsymbol( sym ) ) {
          throw UnboundError( sym ) ;
        } // else if
        else {
          return NewToken( "error" ) ;
          // throw NonFuncError( sym ) ;
        } // else
      } // else if 
      else { // the first argument of ( ... ) is ( 。。。 ), i.e., it is ( ( 。。。 ) ...... )

        Token * checktemp = Evalexp( temp->left ) ;

        // if any error occurs during the evaluation of ( 。。。 ), we just output an
        // an appropriate error message, and we will not proceed any further
        if ( 1 == 1 ) {

          // check whether the evaluated result (of ( 。。。 )) is an internal function
          if ( Isinternalfunc( checktemp->str ) ) {
          // the evaluated result (of ( 。。。 )) is an internal function )
            /*
            check whether the number of arguments is correct

            if num-of-arguments is NOT correct
              ERROR (incorrect number of arguments) : name-of-the-function
              or
              ERROR (incorrect number of arguments) : lambda expression 
              // in the case of nameless functions

            */
            Evalexp( temp->left ) ;
          } // if
          else // the evaluated result (of ( 。。。 )) is not an internal function
            throw NonFuncError( checktemp->left->str ) ; //  ☆ is the evaluated result
        } // if
      } // else end of 「else the first argument of ( ... ) is ( 。。。 )」
        
      /*  
      eval the second argument S2 of (the main S-expression) ( ... )

      if the type of the evaluated result is not correct 
        ERROR (xxx with incorrect argument type) : the-evaluated-result
        // xxx must be the name of some primitive function!

      if no error
        eval the third argument S3 of (the main S-expression) ( ... )

      if the type of the evaluated result is not correct 
        ERROR (xxx with incorrect argument type) : the-evaluated-result

      ...

      if ( no error ) {

        apply the binding of the first argument (an internal function) to S2-eval-result, 
        S3-eval-result, ... 

        if ( no error ) {
          if there is an evaluated result to be returned
            return the evaluated result
          else
            ERROR (no return result) : name-of-this-function
            or
            ERROR (no return result) : lambda expression // if there is such a case ...
        } // if
      } // if
      */
    } // else what is being evaluated is (...) ; we call it the main S-expression
    
    return NewToken( "fail" ) ;
    
  } // Evalexp()
  
};

=======
>>>>>>> parent of 27a5156 (>)
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
    else 
      printf( "%s", token->str.c_str() ) ;
         
  } // Printtoken()
  
  public:
  void Printtree( Token * tokentree ) {
  	Token * temp = tokentree ;
  	if( temp->right == NULL && temp->left == NULL ){
  	  Printtoken(temp);
  	  printf( "\n" );
    }
  	  
  	else  
    	PrintRe( temp, 0 ) ;
  } // Printtree()
  
  void PrintRe( Token * temp, int spacenum ) {

    Type type = temp->type ;

    if ( type == DOT ) {

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
            
            printf(".") ;        
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
      
    } // if
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

  } // Printtree()  
  
  
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
    else if ( str == "'" || str == "quote" ) return QUOTE ;
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
    
    if ( Isend( tokentree ) )
      throw Callend() ; 
    
    // printer.Printtree( tokentree, point, 0 ) ;
  
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
    retoken.type = Gettype( retoken.str ) ;
    retoken.str = Setstr( retoken.str ) ;
    if ( retoken.type == INT )
      retoken.intnum = Decodeint( retoken.str ) ;
    else if ( retoken.type == FLOAT )
      retoken.floatnum = Decodefloat( retoken.str ) ;
      
    return retoken ;
  } // Maktoken()

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
    
};

class Interpreter{
  public:
  Token * NewToken( string str ) {
    Token * retoken = new Token ;
    retoken->str = Setstr( str ) ;
    retoken->type = Gettype( retoken->str ) ;
    retoken->intnum = Decodeint( retoken->str ) ;
    retoken->floatnum = Decodefloat( retoken->str ) ;
    retoken->left = NULL ;
    retoken->right = NULL ;
      
    return retoken ;
  } // NewToken()
  
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

            
        mtreemaker.Buildtree( mtokenlist, morigintree ) ; 

        mtokentree = SetTree( 1 ) ;
        Token * outtree ;
        try {

          outtree = mevaler.Evalexp( mtokentree ) ;
           
          
        }
        catch ( ArgNumError e ) {
          printf( "%s\n", e.merrorstr.c_str() ) ;
          evalerr = true ;
        } // catch
        catch ( UnboundError e ) {
          printf( "%s\n", e.merrorstr.c_str() ) ;
          evalerr = true ;
        } // catch
        catch ( NonFuncError e ) {
          printf( "%s\n", e.merrorstr.c_str() ) ;
          evalerr = true ;
        } // catch
        catch ( NonListError e ) {
          printf( "%s\n", e.merrorstr.c_str() ) ;
          mprinter.Printtree( mtokentree ) ; 
          evalerr = true ;
        } // catch
        // cout << "done" << endl ;
        if ( !evalerr ) {
          Token * t = outtree ;
          /*
          while ( t != NULL ) {
            cout << t->str << endl;
            t = t->right ;
          } // while
          */

          mprinter.Printtree( outtree ) ;
        } // if
           
=======
        
        for( int i = 0 ; i < 1000 ; i++ )  {
          if( morigintree.find(i) != morigintree.end() )
            cout << i << " " << morigintree.find(i)->second.str << endl ;
          // else cout << i << endl ;
        }
>>>>>>> parent of 27a5156 (>)
        
        mtokentree = SetTree(1) ;
        mprinter.Printtree( mtokentree ) ; 
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
        while ( ch != '\n' ) {
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
    retoken->left = NULL ;
    retoken->right = NULL ;
  	
  	return retoken ;
  } // NetToken()
  
  Token * SetTree( int index ) {
    if( morigintree.find(index) == morigintree.end() )
      return NULL ;
    else {
      Token * temp = NewToken( morigintree.find(index)->second ) ;
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
