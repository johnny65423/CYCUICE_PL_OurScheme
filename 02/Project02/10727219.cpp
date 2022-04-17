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

enum Type { NUM, IDENT, OTHER, QUIT, UNKNOWN, INT, FLOAT };

int gLine = 1 ;              // 「下一個要讀進來的字元」所在的line number
int gColumn = 0 ;            // 「下一個要讀進來的字元」所在的column number
int gTestNum ;
bool gEnd = false ;

struct Token {
  string str ;
  
  int line ;
  int column ;
  int intnum ;
  float floatnum ;
  Type type ;
  
};

typedef Token * TokenPtr ;




bool Isletter( char ch ) {
  if ( ch >= 'A' && ch <= 'Z' ) return true ;
  else if ( ch >= 'a' && ch <= 'z' ) return true ;
  else return false ;
} // Isletter()

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

bool Isboolop( string str ) {
  if ( str == "=" || str == "<>" || str == ">" ) return true ;
  else if ( str == "<" || str == ">=" || str == "<=" ) return true ;
  else return false ;
} // Isboolop()

bool Isarithop( string str ) {
  if ( str == "+" || str == "*" ) return true ;
  else if ( str == "*" || str == "/" ) return true ;
  else return false ;
} // Isarithop()

bool Issign( string str ) {
  if ( str == "+" || str == "-" || str == "!" ) return true ;
  else return false ;
} // Issign()

bool Isotherhead( char ch ) {
  if ( ch == '+' || ch == '-' ) return true ;
  else if ( ch == '*' || ch == '/' ) return true ;
  else if ( ch == '>' || ch == '<' ) return true ;
  else if ( ch == '(' || ch == ')' ) return true ;
  else if ( ch == '=' || ch == ';' ) return true ;
  else if ( ch == ':' ) return true ;
  else return false ;
} // Isotherhead()

bool Istypespec( string str ) {
  if ( str == "int" || str == "float" || str == "char" ) return true ;
  else if ( str == "string" || str == "bool" ) return true ;
  else return false ;
} // Istypespec()

bool Isassignop( string str ) {
  if ( str == "=" || str == "+=" || str == "-=" ) return true ;
  else if ( str == "*=" || str == "/=" || str == "%=" ) return true ;
  else return false ;
} // Isassignop()

bool Issysfunc( string str ) {
  if ( str == "ListAllVariables" || str == "ListAllFunctions" ) return true ;
  else if ( str == "ListVariable" || str == "ListFunction" || str == "Done" ) return true ;
  else return false ;
} // Isassignop()

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
    // str.erase( 0, 1 ) ;
    positive = -1 ;
  } // else if
  
  if ( str[0] == '.' )
    str = "0" + str ;
  
  string ch = "" ;


  return atof( str.c_str() ) ;
} // Decodefloat()

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

void Settokenvalue( Token &token ) {
  if ( token.type == INT ) {
    token.intnum = Decodeint( token.str ) ;
  } // if
  else {
    token.floatnum = Decodefloat( token.str ) ;
  } // else
} // Settokenvalue()

void Printvalue( Token token ) {
  if ( token.type == INT ) {
    printf( "%d", token.intnum ) ;
  } // if
  else if ( token.type == FLOAT ) {
    printf( "%.3f", token.floatnum ) ;
  } // else if
  else {
    printf( "%s", token.str.c_str() ) ;
  } // else
} // Printvalue()

class Exception {
public:
  string mname ;
  string merrorstr;
  Token mtoken ;
  
  Exception() {  }

  Exception( string str ) {
    merrorstr = str;
  } // Exception()
  
  

}; // Exception

class Callend : public Exception {
public:
  Callend() {
    mname = "Callend" ;
  } // Callend()
};

class LexicalError : public Exception {
public:
  LexicalError( char ch ) {
    mname = "LexicalError" ;
    stringstream ss ;

    ss << "Unrecognized token with first char : '" << ch << "'" ;

    merrorstr = ss.str() ;
  } // LexicalError()
};

class SyntacticError : public Exception {
public:
  SyntacticError( string str ) {
    mname = "LexicalError" ;
    stringstream ss ;

    ss << "Unexpected token : '" << str << "'" ;

    merrorstr = ss.str() ;
  } // SyntacticError()
};

class SemanticError : public Exception {
public:
  SemanticError( string str ) {
    mname = "LexicalError" ;
    stringstream ss ;

    ss << "Undefined identifier : '" << str << "'" ;

    merrorstr = ss.str() ;
  } // SemanticError()
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
  
  Token Peektoken() {
    
    Token temp = Gettoken() ;
    cin.putback( mch );
    gColumn = temp.column ;
    gLine = temp.line ;
    string str = temp.str ;
    for ( int i = str.size() - 1 ; i >= 0 ; i-- ) {
      // ungetc( str[i], stdin ) ;
      cin.putback( str[i] );
    } // for 
    
    Getchar();
    return temp ;
  } // Peektoken()

  void Cenceltoken( Token temp ) {
    cin.putback( mch );
    gColumn = temp.column ;
    gLine = temp.line ;
    string str = temp.str ;
    for ( int i = str.size() - 1 ; i >= 0 ; i-- ) {
      // ungetc( str[i], stdin ) ;
      cin.putback( str[i] );
    } // for 

    Getchar();
  } // Cenceltoken()

  Token Gettoken() {
    Token retoken ;
    Type type ;
    Readnwschar() ; 
    retoken.column = gColumn ;
    retoken.line = gLine ;
    retoken.str = Gettokenstr( type ) ;
    retoken.type = type ;
    while ( retoken.str == "//" ) {
      while ( mch != '\n' && !gEnd )
        Getchar() ;
      
      /*
      if ( gEnd )  
        throw EndOfFileError() ;
      else
        Readnwschar() ; 
      */
      Readnwschar() ; 

      retoken.column = gColumn ;
      retoken.line = gLine ;
      retoken.str = Gettokenstr( type ) ;
      retoken.type = type ;
    } // while
    

    // if ( retoken.str == "quit" )
    //   retoken.type = QUIT ;

    Settokenvalue( retoken ) ;


    return retoken ;
  } // Gettoken()
  
  void ReadSexp( vector<Token> & tokenlist ) {
    Token temp ;
    Readnwschar() ;
    temp = Gettoken() ;


    
  } // ReadSexp()  

  void Test() {
    while ( !gEnd ) {
      Token t = Gettoken() ;
      cout << t.str << endl ;
    } // while
  } // Test()
  
  private:  
  char mch ; 

  string Gettokenstr( Type &type ) {
    Readnwschar() ;
    
    if ( Isletter( mch ) || mch == '_' ) {
      type = IDENT ;
      return Getstring( mch ) ;
    } // if
    else if ( Isdigit( mch ) || mch == '.' ) {
      type = NUM ;
      return Getnum( mch, type ) ;
    } // else if
    else if ( Isotherhead( mch ) ) {
      type = OTHER ;
      return Getothers( mch ) ;
    } // else if
    else {
      throw LexicalError( mch ) ;
    } // else
  } // Gettokenstr() 
  
  string Getstring( char & mch ) {
    string temp = "" ;
    Token tk ;
    tk.line = gLine ;  
    tk.column = gColumn ;

    while ( Isdigit( mch ) || Isletter( mch ) || mch == '_' ) {
      temp += mch ;
      Getchar() ;

      if ( gEnd ) {
        return temp ;
      } // if
        
    } // while
    
    return temp ;
  } // Getstring() 

  string Getnum( char & mch, Type & t ) {
    string temp = "" ;
    bool dot = false ; 
    t = INT ;
    while ( Isdigit( mch ) || ( mch == '.' && !dot ) ) {
      if ( mch == '.' ) {
        dot = true ;
        t = FLOAT ;
      } // if
        
      temp += mch ; 
      Getchar() ;

      if ( gEnd )
        return temp ;
    } // while
    
    return temp ;
  } // Getnum() 
  
  string Getothers( char & mch ) {
    string temp = "" ; 
    temp += mch ;
    Getchar() ;
    // if ( temp == "=" || temp == "!" || temp == "*" || temp == ":" ) {
    if ( temp == "*" || temp == ":" ) {
      if ( mch == '=' ) {
        temp += mch ;
        Getchar() ;
      } // if
    } // if
    else if ( temp == "/" ) {
      if ( mch == '=' || mch == '/' ) {
        temp += mch ;
        Getchar() ;
      } // if
    } // else if
    else if ( temp == "&" ) {
      if ( mch == '&' ) {
        temp += mch ;
        Getchar() ;
      } // if
    } // else if
    else if ( temp == "|" ) {
      if ( mch == '|' ) {
        temp += mch ;
        Getchar() ;
      } // if
    } // else if
    else if ( temp == ">" ) {
      if ( mch == '=' || mch == '>' ) {
        temp += mch ;
        Getchar() ;
      } // if
    } // else if
    else if ( temp == "<" ) {
      if ( mch == '=' || mch == '<' || mch == '>' ) {
        temp += mch ;
        Getchar() ;
      } // if
    } // else if
    else if ( temp == "-" ) {
      if ( mch == '>' || mch == '-' || mch == '=' ) {
        temp += mch ;
        Getchar() ;
      } // if
    } // else if
    else if ( temp == "+" ) {
      if ( mch == '=' || mch == '+' ) {
        temp += mch ;
        Getchar() ;
      } // if
    } // else if
      
    return temp ;
  } // Getothers() 
  
    
  
};



class Interpreter{
  public:
   
  
  Interpreter() {
    // mch = '\0' ;
    // gPrinter = Printer() ;
    mScanner = Scanner() ;
    // gTreemaker = Treemaker() ;
  } // Interpreter() 
  
  void Gettokenlist() {
    
    while ( 0 == 0 ) {
      mtokenlist.clear() ;
      bool err = false ;
      Token retoken ;


      printf( "> " ) ;
      if ( gEnd )
        throw EndOfFileError() ;

      try {
        // ReadCmd(  mtokenlist, retoken ) ;
        // Printvalue( retoken ) ;  
        Readuserinput( mtokenlist, retoken ) ;
      } // try
      catch ( LexicalError e ) {
        printf( "%s", e.merrorstr.c_str() ) ;
        while ( !gEnd && mScanner.Getch() != '\n' )
          mScanner.Getchar() ;
      }  // catch
      catch ( SyntacticError e ) {
        printf( "%s", e.merrorstr.c_str() ) ;
        while ( !gEnd && mScanner.Getch() != '\n' )
          mScanner.Getchar() ;
      }  // catch
      catch ( SemanticError e ) {
        printf( "%s", e.merrorstr.c_str() ) ;
        while ( !gEnd && mScanner.Getch() != '\n' )
          mScanner.Getchar() ;
      }  // catch

      /*
      for ( int i = 0 ; i < mtokenlist.size() ; i++ ) {
        Printvalue( mtokenlist.at( i ) ) ;
      } // for
      */
      

      printf( "\n" ) ;
      
      int c = gColumn ;
      char ch = mScanner.Getch() ;
      while ( Iswhitespace( ch ) && ch != '\n' ) {
        mScanner.Getchar() ;
        ch = mScanner.Getch() ;
      } // while
      /*
      ch = gScanner.Getch() ;
      if ( ch == ';' ) {
        while ( ch != '\n' && !gEnd ) {
          gScanner.Getchar() ;
          ch = gScanner.Getch() ;
        } // while
      } // if
      */
      if ( mScanner.Getch() == '\n' )
        gColumn = 0 ; 
      else
        gColumn = gColumn - c + 1 ;
         

    } // while()
    

  } // Gettokenlist()
  
  private:
  vector < Token > mtokenlist ;
  map< string, Token > mVarmap ;
  Scanner mScanner ;

  void Readuserinput( vector<Token> & tokenlist, Token & retoken ) {
    Token temp = mScanner.Peektoken() ;
    if ( Issysfunc( temp.str ) ) {
      temp = mScanner.Gettoken() ;
      throw Callend();
    } // if
    else if ( Istypespec( temp.str ) ) {
      temp = mScanner.Gettoken() ;
      Printvalue( temp ) ;
    } // else if
    else {
      temp = mScanner.Gettoken() ;
      cout << "->" ;
      Printvalue( temp ) ;
    } // else
    
  } // Readuserinput()

  void ReadCmd( vector<Token> & tokenlist, Token & retoken ) {
    
    Token temp = mScanner.Peektoken() ;
    Token temp2 ;
    Token ctemp1, ctemp2 ;
    string name = "";
    if ( temp.str == "quit" ) {
      // temp = mScanner.Gettoken() ;
      throw Callend();
    } // if
    else if ( temp.type == IDENT ) {
      temp = mScanner.Gettoken() ;
      tokenlist.push_back( temp ) ;
      // :=->接exp else->無頭exporbool ;
      temp2 = mScanner.Peektoken() ;
      if ( temp2.str == ":=" ) {
        name = temp.str ;
        temp = mScanner.Gettoken() ;
        tokenlist.push_back( temp ) ;
        Exp( tokenlist, ctemp1 ) ;

        // Define( name, ctemp1 ) ;
        // Getvalue( retoken, ctemp1 ) ;
      } // if
      else if ( Isarithop( temp2.str ) ) {
        mScanner.Cenceltoken( temp ) ;
        
        Exp( tokenlist, ctemp1 ) ;
        temp = mScanner.Peektoken() ;
        if ( Isboolop( temp.str ) ) {
          string op = temp.str ;
          temp = mScanner.Gettoken() ;
          tokenlist.push_back( temp ) ;
          Exp( tokenlist, ctemp2 ) ;
          
          Boolwork( ctemp1, ctemp2, op ) ;

        } // if

        Getvalue( retoken, ctemp1 ) ;



        /*
        string op ;
        Getvalue( ctemp1, temp ) ;
        while ( Isarithop( temp2.str ) ) {
          
          temp = mScanner.Gettoken() ;
          op = temp2.str ;
          tokenlist.push_back( temp2 ) ;
          if ( Issign( temp2.str ) ) {
            Term( tokenlist, ctemp2 ) ;
          } // if
          else {
            Exp( tokenlist, ctemp2 ) ;
          } // else
          
          Arithwork( ctemp1, ctemp2, op ) ; 

          temp2 = mScanner.Peektoken() ;

        } // while

        if ( Isboolop( temp2.str ) ) {
          temp2 = mScanner.Gettoken() ;
          op = temp2.str ;
          tokenlist.push_back( temp2 ) ;
          Exp( tokenlist, ctemp2 ) ;

          Boolwork( ctemp1, ctemp2, op ) ;

        } // if

        Getvalue( retoken, ctemp1 ) ;
        */
      } // else if
      else if ( temp2.str == ";" ) {
        Getvalue( retoken, temp ) ;
      } // else if
    } // else if
    else if ( temp.type == INT || temp.type == FLOAT || Issign( temp.str ) || temp.str == "(" ) {

      Exp( tokenlist, ctemp1 ) ;
      temp = mScanner.Peektoken() ;
      if ( Isboolop( temp.str ) ) {
        string op = temp.str ;
        temp = mScanner.Gettoken() ;
        tokenlist.push_back( temp ) ;
        Exp( tokenlist, ctemp2 ) ;
        
        Boolwork( ctemp1, ctemp2, op ) ;

      } // if

      Getvalue( retoken, ctemp1 ) ;

    } // else if
    else { // error
      temp = mScanner.Gettoken() ;
      throw SyntacticError( temp.str ) ;

    } // else

    temp = mScanner.Peektoken() ;
    if ( temp.str == ";" ) {
      temp = mScanner.Gettoken() ;
      tokenlist.push_back( temp ) ;

      if ( name != "" ) {
        Define( name, ctemp1 ) ;
        Getvalue( retoken, ctemp1 ) ;
      } // if

    } // if
    else {
      temp = mScanner.Gettoken() ;
      throw SyntacticError( temp.str ) ;
    } // else

    
  } // ReadCmd()  

  void Exp( vector<Token> & tokenlist, Token & retoken ) {
    Token ttemp1, ttemp2 ;
    Term( tokenlist, ttemp1 ) ;
    Token temp = mScanner.Peektoken() ;
    while ( temp.str == "+" || temp.str == "-" ) {
      string op = temp.str ;
      temp = mScanner.Gettoken() ;
      tokenlist.push_back( temp ) ;

      Term( tokenlist, ttemp2 ) ;
      
      Arithwork( ttemp1, ttemp2, op ) ;

      temp = mScanner.Peektoken() ;
    } // while
    
    Getvalue( retoken, ttemp1 ) ;
    
  } // Exp()

  void Term( vector<Token> & tokenlist, Token & retoken ) {
    Token ftemp1, ftemp2 ;
    Factor( tokenlist, ftemp1 ) ;
    Token temp = mScanner.Peektoken() ;
    while ( temp.str == "*" || temp.str == "/" ) {
      string op = temp.str ;
      temp = mScanner.Gettoken() ;
      tokenlist.push_back( temp ) ;

      Factor( tokenlist, ftemp2 ) ;

      Arithwork( ftemp1, ftemp2, op ) ;

      temp = mScanner.Peektoken() ;
    } // while
    
    Getvalue( retoken, ftemp1 ) ;

  } // Term()

  void Factor( vector<Token> & tokenlist, Token & retoken ) {
    Token temp = mScanner.Peektoken() ;
    if ( temp.str == "(" ) {
      temp = mScanner.Gettoken() ;
      tokenlist.push_back( temp ) ;

      Exp( tokenlist, retoken ) ;
      
      temp = mScanner.Peektoken() ;
      if ( temp.str == ")" ) {
        temp = mScanner.Gettoken() ;
        tokenlist.push_back( temp ) ;
      } // if
      else {
        temp = mScanner.Gettoken() ;
        throw SyntacticError( temp.str ) ;
      } // else

    } // if
    else if ( Issign( temp.str ) ) {
      temp = mScanner.Gettoken() ;
      // tokenlist.push_back( temp ) ;
      string sign = temp.str ;
      temp = mScanner.Peektoken() ;
      if ( temp.type == INT || temp.type == FLOAT ) {
        temp = mScanner.Gettoken() ;
        Setsign( temp, sign ) ;
        Getvalue( retoken, temp ) ;
        tokenlist.push_back( temp ) ;
      } // if
      else {
        temp = mScanner.Gettoken() ;
        throw SyntacticError( temp.str ) ;
      } // else
    } // else if
    else if ( temp.type == INT || temp.type == FLOAT || temp.type == IDENT ) {
      // 如果有sign
      temp = mScanner.Gettoken() ;
      Getvalue( retoken, temp ) ;
      tokenlist.push_back( temp ) ;
    } // else if
    else {
      temp = mScanner.Gettoken() ;
      throw SyntacticError( temp.str ) ;
    } // else

  } // Factor()

  void Getvalue( Token & retoken, Token origin ) {
    
    if ( origin.type == IDENT ) {
      if ( mVarmap.find( origin.str ) != mVarmap.end() ) {
        Token temp = mVarmap[ origin.str ] ;
        retoken.str = temp.str ;
        retoken.type = temp.type ;
        retoken.intnum = temp.intnum ;
        retoken.floatnum = temp.floatnum ;
      } // if
      else {
        throw SemanticError( origin.str ) ;
      } // else

    } // if
    else {
      retoken.str = origin.str ;
      retoken.type = origin.type ;
      retoken.intnum = origin.intnum ;
      retoken.floatnum = origin.floatnum ;
    } // else

  } // Getvalue()

  void Setsign( Token & retoken, string sign ) {
    if ( sign == "-" ) {
      retoken.str = sign + retoken.str ;
      Settokenvalue( retoken ) ;
    } // if

  } // Setsign()

  void Arithwork( Token & token1, Token & token2, string op ) {
    stringstream ss ;
    if ( token1.type != token2.type ) {
      if ( token1.type == INT ) {
        token1.type = FLOAT ;
        if ( op == "+" )
          token1.floatnum = ( ( float ) token1.intnum ) + token2.floatnum ;
        else if ( op == "-" )
          token1.floatnum = ( ( float ) token1.intnum ) - token2.floatnum ;
        else if ( op == "*" )
          token1.floatnum = ( ( float ) token1.intnum ) * token2.floatnum ;
        else if ( op == "/" )
          token1.floatnum = ( ( float ) token1.intnum ) / token2.floatnum ;
        token1.intnum = 0 ;
        ss << Tofloat( token1.floatnum ) ;
        token1.str = ss.str() ;
      } // if
      else {
        token1.type = FLOAT ;
        if ( op == "+" )
          token1.floatnum = token1.floatnum + ( ( float ) token2.intnum ) ;
        else if ( op == "-" )
          token1.floatnum = token1.floatnum - ( ( float ) token2.intnum ) ;
        else if ( op == "*" )
          token1.floatnum = token1.floatnum * ( ( float ) token2.intnum ) ;
        else if ( op == "/" )
          token1.floatnum = token1.floatnum / ( ( float ) token2.intnum ) ;
        ss << Tofloat( token1.floatnum ) ;
        token1.str = ss.str() ;
      } // else
    } // if
    else {
      if ( token1.type == INT ) {
        if ( op == "+" )
          token1.intnum = token1.intnum + token2.intnum ;
        else if ( op == "-" )
          token1.intnum = token1.intnum - token2.intnum ;
        else if ( op == "*" )
          token1.intnum = token1.intnum * token2.intnum ;
        else if ( op == "/" )
          token1.intnum = token1.intnum / token2.intnum ;

        ss << token1.intnum ;
        token1.str = ss.str() ;
      } // if
      else {
        if ( op == "+" )
          token1.floatnum = token1.floatnum + token2.floatnum ;
        else if ( op == "-" )
          token1.floatnum = token1.floatnum - token2.floatnum ;
        else if ( op == "*" )
          token1.floatnum = token1.floatnum * token2.floatnum ;
        else if ( op == "/" )
          token1.floatnum = token1.floatnum / token2.floatnum ;

        ss << Tofloat( token1.floatnum ) ;
        token1.str = ss.str() ;
      } // else
    } // else

  } // Arithwork()

  void Boolwork( Token & token1, Token & token2, string op ) {
    bool ans = true;
    if ( token1.type != token2.type ) {
      if ( token1.type == INT ) {
        float num = ( float ) token1.intnum ;
        ans = Boolfloat( num, token2.floatnum, op ) ;
      } // if
      else {
        float num = ( float ) token2.intnum ;
        ans = Boolfloat( token1.floatnum, num, op ) ;
      } // else
    } // if
    else {
      if ( token1.type == INT ) {
        ans = Boolint( token1.intnum, token2.intnum, op ) ;
      } // if
      else {
        ans = Boolfloat( token1.floatnum, token2.floatnum, op ) ;
      } // else
    } // else

    token1.type = OTHER ;
    token1.intnum = 0 ;
    token1.floatnum = 0.0 ;
    if ( ans )
      token1.str = "true" ;
    else
      token1.str = "false" ;

  } // Boolwork()

  bool Boolint( int num1, int num2, string op ) {
    if ( op == "=" ) {
      if ( num1 == num2 )
        return true ;
    } // if
    else if ( op == "<>" ) {
      if ( num1 != num2 )
        return true ;
    } // else if
    else if ( op == ">" ) {
      if ( num1 > num2 )
        return true ;
    } // else if
    else if ( op == "<" ) {
      if ( num1 < num2 )
        return true ;
    } // else if
    else if ( op == ">=" ) {
      if ( num1 >= num2 )
        return true ;
    } // else if
    else if ( op == "<=" ) {
      if ( num1 <= num2 )
        return true ;
    } // else if
    
    return false ;
  } // Boolint()

  bool Boolfloat( float num1, float num2, string op ) {

    float tolerance = 0.0001 ;
    float l, s ;

    bool larger, equal ;
    if ( num1 > num2 ) {
      larger = true ;
      l = num1 ;
      s = num2 ;
    } // if
    else {
      larger = false ;
      l = num2 ;
      s = num1 ;
    } // else
    
    // cout << l * ( 1.0 - tolerance ) << " " << s << endl ;
    // if ( l * ( 1.0 - tolerance ) <= s )
    if ( l - s <= tolerance )
      equal = true ;
    else
      equal = false;

    // if ( equal ) cout << "eq" ;
    
    if ( op == "=" ) {
      if ( equal )
        return true ;
    } // if
    else if ( op == "<>" ) {
      if ( !equal )
        return true ;
    } // else if
    else if ( op == ">" ) {
      if ( larger && !equal )
        return true ;
    } // else if
    else if ( op == "<" ) {
      if ( !larger && !equal )
        return true ;
    } // else if
    else if ( op == ">=" ) {
      if ( larger || equal )
        return true ;
    } // else if
    else if ( op == "<=" ) {
      if ( !larger || equal )
        return true ;
    } // else if
    
    return false ;
  } // Boolfloat()

  void Define( string name, Token temp ) {
    Token ntoken ;
    
    ntoken.str = temp.str ;
    ntoken.intnum = temp.intnum ;
    ntoken.floatnum = temp.floatnum ;
    ntoken.type = temp.type ;
    mVarmap[ name ] = ntoken ;

  } // Define()

};

int main() {
  
  Interpreter interpreter = Interpreter() ;

  
  char t ;
  scanf( "%d",  &gTestNum ) ;
  scanf( "%c",  &t ) ;
  
  printf( "Program starts...\n" ) ;
  try {
    // gScanner.Test();
    interpreter.Gettokenlist();
  } // try
  catch ( Exception e ) {
    // cout << e.mname ;
    if ( e.mname == "EndOfFileError" )
      printf( "%s", e.merrorstr.c_str() ) ;
  } // catch

  printf( "Program exits..." ) ;

} // main()
