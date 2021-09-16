/* 
 * File:   main.cpp
 * Author: asus
 *
 * Created on 2019年3月30日, 下午 1:21
 */

#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <string.h>

using namespace std;

enum TokenType { IDENTIFIER = 34512, CONSTANT = 87232, SPECIAL = 29742 } ;


typedef char * CharPtr ;

struct Column {
  int column ;                  // 此token(的字首)在本line有出現於此column
  Column * next;                // 下一個此token(在本line)有出現的column
} ; // Column

typedef Column * ColumnPtr ;


struct Line {                   // struct Line 記錄了4種資訊
  int line ;                    // 此token放在哪一 line
  ColumnPtr firstAppearAt ;     // 指到此token第一次出現在此line的哪一column
  ColumnPtr lastAppearAt ;      // 指到此token最後出現在此line的哪一column
  Line *  next;                 // 同一 token 下一次出現在哪一 line
} ;


typedef Line * LinePtr ;

struct Token {                  // Struct token 紀錄了4種資訊
  CharPtr tokenStr ;            // tokenStr 放你切下來的token
  TokenType type;               // type 紀錄此token是屬於哪一個case
  LinePtr firstAppearOn ;       // 此 token 第一次出現在哪一 line
  LinePtr lastAppearOn ;        // 此 token 最後出現在哪一 line
  Token * next ;                // 指向下一個token
} ; // struct Token


typedef Token * TokenPtr ;

TokenPtr gFront = NULL, gRear = NULL ;  // 分別指向Token串列的頭跟尾

typedef char Str100[ 100 ] ; // 此型別是打算用來宣告一個陣列變數、以供讀字串之用

int gLine = 1 ;              // 「下一個要讀進來的字元」所在的line number

int gColumn = 1 ;            // 「下一個要讀進來的字元」所在的column number

bool grun = true ;
char Getchar() ;
void Readnwschar( char & ch ) ;
bool Isletter( char ch ) ;
bool Isdight( char ch ) ;
bool Iswhitespace( char ch ) ;
bool Iscase3( char ch ) ;
TokenType Gettype( char ch ) ;
CharPtr Getstr( char & ch ) ;
CharPtr Getcase1str( char & ch ) ;
CharPtr Getcase2str( char & ch ) ;
CharPtr Getcase3str( char & ch ) ;
CharPtr Getcomment( char & ch ) ;
void Printtoken(  TokenPtr head ) ;
void Printline(  LinePtr head ) ;
void Printcolumn(  ColumnPtr head, int line ) ;
void GettokenList( TokenPtr & head, char & ch ) ;
TokenPtr Gettoken( char & ch ) ;
void Inserttoken( TokenPtr & head, TokenPtr & temp ) ;
void Insertline( LinePtr & head, LinePtr & temp ) ;
void Insertcolumn( ColumnPtr & head, ColumnPtr & temp ) ;
void Printoption() ; 
void Getcommend ( TokenPtr head ) ;
void Howmanytoken ( TokenPtr head ) ;
void Howmanyforcase( TokenPtr head ) ;
void Tokentofind( TokenPtr head ) ;
void Linetofind ( TokenPtr head ) ;
void Printcase( int c, int num ) ; 

int main() {
  TokenPtr temp = NULL, head = NULL ;
  char ch = '\0' ;
  Readnwschar( ch ) ;
  GettokenList( head, ch ) ;
  Printtoken( head ) ;
  Getcommend ( head ) ;
  return 0;
}


char Getchar(){
  char ch = '\0' ;
  
  scanf( "%c", &ch ) ;
  gColumn++ ;
  if( ch == '\n' ) {
    gColumn = 1 ;
    gLine++ ;
  }
  return ch ;
  
}

bool Isletter( char ch ) {

  if ( ch >= 'A' && ch <= 'Z' ) return true ;
  else if ( ch >= 'a' && ch <= 'z' ) return true ;
  else return false ;
  
}

bool Iswhitespace( char ch ) {
  if ( ch == ' ' ) return true ;
  else if ( ch == '\n' ) return true ;
  else if ( ch == '\t' ) return true ;
  else return false ;
  
}

bool Isnumber( char ch ) {
  if ( ch >= '0' && ch <= '9' ) return true ;
  else return false ;
  
}

void Readnwschar( char & ch ){
  while( ch == ' ' || ch == '\n' || ch == '\t' || ch == '\0' ) {
    ch = Getchar() ;
  }
}

CharPtr Getcase1str( char & ch ) {
  Str100 temp = { 0 } ; // 初始化
  int i = 0 ;
  CharPtr goalstr = NULL ;
  
  temp[i] = ch ;
  ch = Getchar() ;
  while ( Isdight( ch ) || Isletter( ch ) || ch == '_' ) {
    i++ ;
    temp[i] = ch ;
    ch = Getchar() ;
  }
  
  i++ ;
  temp[i] = '\0' ;
  goalstr = new char[ strlen( temp ) + 1 ] ;
  strcpy( goalstr, temp ) ;
  return goalstr ;
  
}

CharPtr Getcase2str( char & ch ) {
  Str100 temp = { 0 } ; // 初始化
  int i = 0 ;
  CharPtr goalstr = NULL ;
  
  temp[i] = ch ;
  ch = Getchar() ;
  if ( Isdight( temp[i] ) ) {
    while ( Isdight( ch ) || ch == '.' ) {
      i++ ;
      temp[i] = ch ;
      ch = Getchar() ;
    }
  } // if
  else if ( temp[0] == '\"' ) { // "
    while ( ch != '\"' ) {
      i++ ;
      temp[i] = ch ;
      ch = Getchar() ;
      if( temp[i] == '\\' ) {
        i++ ;
        temp[i] = ch ;
        ch = Getchar() ;
      }
    }
    i++ ;
    temp[i] = '\"' ;
    ch = Getchar() ;
  } // else if
  else if ( temp[0] == '\'' ) { // '
    while ( ch != '\'' ) {
      i++ ;
      temp[i] = ch ;
      ch = Getchar() ;
      if( temp[i] == '\\' ) {
        i++ ;
        temp[i] = ch ;
        ch = Getchar() ;
      }
    }
    i++ ;
    temp[i] = '\'' ;
    ch = Getchar() ;
  } // else if
  
  i++ ;
  temp[i] = '\0' ;
  goalstr = new char[ strlen( temp ) + 1 ] ;
  strcpy( goalstr, temp ) ;
  return goalstr ;
  
}

CharPtr Getcase3str( char & ch ) {
  Str100 temp = { 0 } ; // 初始化
  int i = 0 ;
  CharPtr goalstr = NULL ;
  
  temp[i] = ch ;
  ch = Getchar() ;
  
  if( temp[i] == '=' || temp[i] == '!' || temp[i] == '*' ) {
    if ( ch == '=' ) {
      i++ ;
      temp[i] = ch ;
      ch = Getchar() ;
    }
  }
  else if( temp[i] == '&' ) {
    if ( ch == '&' ) {
      i++ ;
      temp[i] = ch ;
      ch = Getchar() ;
    }
  }
  else if( temp[i] == '|' ) {
    if ( ch == '|' ) {
      i++ ;
      temp[i] = ch ;
      ch = Getchar() ;
    }
  }
  else if( temp[i] == '>' ) {
    if ( ch == '=' || ch == '>' ) {
      i++ ;
      temp[i] = ch ;
      ch = Getchar() ;
    }
  }
  else if( temp[i] == '<' ) {
    if ( ch == '=' || ch == '<' ) {
      i++ ;
      temp[i] = ch ;
      ch = Getchar() ;
    }
  }
  else if( temp[i] == '-' ) {
    if ( ch == '>' || ch == '-' ) {
      i++ ;
      temp[i] = ch ;
      ch = Getchar() ;
    }
  }
  else if( temp[i] == '+' ) {
    if ( ch == '=' || ch == '+' ) {
      i++ ;
      temp[i] = ch ;
      ch = Getchar() ;
    }
  }
  
  i++ ;
  temp[i] = '\0' ;
  goalstr = new char[ strlen( temp ) + 1 ] ;
  strcpy( goalstr, temp ) ;
  return goalstr ;
  
}

CharPtr Getcomment( char & ch ) {
  Str100 temp = { 0 } ; // 初始化
  int i = 0 ;
  CharPtr goalstr = NULL ;
  
  temp[i] = ch ;
  ch = Getchar() ;
  if ( ch == '=' ) {
    i++ ;
    temp[i] = ch ;
    ch = Getchar() ;
    i++ ;
    temp[i] = '\0' ;
    goalstr = new char[ strlen( temp ) + 1 ] ;
    strcpy( goalstr, temp ) ;
    return goalstr ;
  }
  else if( ch == '*' ) {
    char beforech ;
    ch = Getchar() ;
    beforech = ch ;
    ch = Getchar() ;
    while( ch != '/' || beforech != '*' ) {
      beforech = ch ;
      ch = Getchar() ; 
    } //while
    
    ch = Getchar() ; 
    return NULL ;
       
  } // if( ch == '*' )
  else if ( ch == '/' ) {
    ch = Getchar() ;
    while ( ch != '\n' ) {
      ch = Getchar() ;
    }
    ch = Getchar() ;

    return NULL ; 
  }
  else {
    goalstr = new char[ 2 ] ;
    strcpy( goalstr, "/" ) ;
    return goalstr ;
  }
  
}

bool Iscase3( char ch ) {
  if ( Isletter( ch ) ) return false ;
  else if ( Iswhitespace( ch ) ) return false ;
  else if ( Isdight( ch ) ) return false ;
  else if ( ch == '/' ) return false ;
  else return true ;
  
}

bool Isdight( char ch ) {
  if( ch >= '0' && ch <= '9' ) return true ;
  else return false ;
}

CharPtr Getstr( char & ch ) {
  if ( Isletter( ch ) || ch == '_'  ) 
    return Getcase1str( ch ) ;
  else if ( Isdight( ch ) || ch == '\"' || ch == '\'' )
    return Getcase2str( ch ) ;
  else if ( ch == '/' ) return Getcomment( ch ) ;
  else if ( Iscase3( ch ) ) return Getcase3str( ch ) ;
}

TokenType Gettype( char ch ) {
  if ( Isletter( ch ) || ch == '_'  ) 
    return IDENTIFIER ;
  else if ( Isdight( ch ) || ch == '"' || ch == '\'' )
    return CONSTANT ;
  else if ( ch == '/' )
    return SPECIAL ;
  else if ( Iscase3( ch ) ) 
    return SPECIAL ;
}

void Printtoken(  TokenPtr head ) {
  if ( head != NULL ) {
    
    printf( "%s ", head->tokenStr ) ;
    Printline( head->firstAppearOn ) ;
    printf( "\n" ) ;
    Printtoken( head->next ) ;
  } 
}

void Printline(  LinePtr head ) {
  if ( head != NULL ) {
    Printcolumn( head->firstAppearAt, head->line ) ;
    Printline( head->next ) ;
  }
}

void Printcolumn(  ColumnPtr head, int line ) {
  
  if ( head != NULL ) {
    
    printf( "(%d,%d)", line, head->column ) ;
    Printcolumn( head->next, line ) ;
  }

}

void GettokenList( TokenPtr & head, char & ch ) {
  TokenPtr temp  = NULL ;
  
  temp = Gettoken( ch ) ;
  while( strcmp( temp->tokenStr, "END_OF_FILE" ) != 0 ) {
    Inserttoken( head, temp ) ;

    temp  = NULL ;
    temp = Gettoken( ch ) ;
  }
  
}

TokenPtr Gettoken( char & ch ) {
  TokenPtr temp = NULL ;
  Readnwschar( ch ) ;
  temp = new Token ;
  temp->next = NULL ;
  temp->firstAppearOn = new Line ;
  temp->firstAppearOn->line = gLine ;
  temp->firstAppearOn->next = NULL ;
  temp->firstAppearOn->firstAppearAt = new Column ;
  temp->firstAppearOn->firstAppearAt->column   = gColumn - 1 ;
  temp->firstAppearOn->firstAppearAt->next = NULL ;
  temp->type = Gettype( ch ) ;
  temp->tokenStr = Getstr( ch ) ;
  if ( temp->tokenStr != NULL ) {
    temp->next = NULL ;
    return temp ;
  }
  else return Gettoken( ch ) ;
}

void Inserttoken( TokenPtr & head, TokenPtr & temp ) {
  TokenPtr insert = NULL ;
  if ( head == NULL ) {
    head == new Token ;
    head = temp ;
    head->next = NULL ;
  }
  else if ( strcmp( head->tokenStr, temp->tokenStr ) > 0 ) {
    insert = temp ;
    insert->next = head ;
    head = insert ;
  }
  else if ( strcmp( head->tokenStr, temp->tokenStr ) < 0 ) {
    Inserttoken( head->next, temp ) ;
  }
  else if ( strcmp( head->tokenStr, temp->tokenStr ) == 0 ) {
    Insertline( head->firstAppearOn, temp->firstAppearOn ) ;
  }
  else ;
}

void Insertline( LinePtr & head, LinePtr & temp ) {
  LinePtr insert = NULL ;
  if ( head == NULL ) {
    head = new Line ;
    head = temp ;
    head->next = NULL ;
  }
  else if ( head->line > temp->line ) {
    insert = new Line ;
    insert->line = temp->line ;
    insert->next = NULL ;
    insert->firstAppearAt = new Column ;
    insert->firstAppearAt->column = temp->firstAppearAt->column ;
    insert->firstAppearAt->next = NULL ;
    insert->next = head ;
    head = insert ;
  }
  else if ( head->line == temp->line )
    Insertcolumn( head->firstAppearAt, temp->firstAppearAt ) ;
  else Insertline( head->next, temp ) ;
}

void Insertcolumn( ColumnPtr & head, ColumnPtr & temp ) {
  ColumnPtr insert = NULL ;
  if ( head == NULL ) {
    head = new Column ;
    head = temp ;
    head->next = NULL ;
  }
  else if ( head->column > temp->column ) {
    insert = new Column ;
    insert->column = temp->column ;
    insert->next = NULL ;
    insert->next = head ;
    head = insert ;
  }
  else Insertcolumn( head->next, temp ) ;
}

void Printoption() {
  printf( "\n" ) ;
  printf( "請輸入指令 : \n" ) ;
  printf( "1.總共有多少種 token\n" ) ;
  printf( "2.三種case各有多少 token\n" ) ;
  printf( "3.印出指定 token 的位置 (line number,column number) (要排序)\n" ) ;
  printf( "4.印出所指定的那一 line 出現哪些 token (要排序)\n" ) ;
  printf( "5.結束\n" ) ;
  
}

void Getcommend ( TokenPtr head ) {
  int cmd = 0 ;
  
  Printoption() ;
  printf( "> " ) ;
  scanf( "%d", & cmd ) ;
  
  while( cmd != 5 ) {
    if ( cmd == 1 ) {
      printf( "\n" ) ;
      Howmanytoken ( head ) ;
    }
    else if ( cmd == 2 ) {
      printf( "\n" ) ;
      Howmanyforcase( head ) ;
    }
    else if ( cmd == 3 ) {
      printf( "\n" ) ;
      Tokentofind( head ) ;
    }
    else if ( cmd == 4 ) {
      printf( "\n" ) ;
      Linetofind ( head ) ;
    }
    
    Printoption() ;
     printf( "> " ) ;
    scanf( "%d", & cmd ) ;

  }
  
  printf( "byebye" ) ;
}

void Howmanytoken ( TokenPtr head ) {
  int i = 0 ;
  TokenPtr temp = NULL;
  temp = head ;
  while ( temp != NULL ) {
    i++ ;
    temp = temp->next ;
  }
  
  
  printf( "總共%d種\n", i ) ;
}

void Howmanyforcase( TokenPtr head ) {
  int a = 0, b = 0, c = 0 ;
   TokenPtr temp = NULL;
  temp = head ;
  while ( temp != NULL ) {
    if( temp->type == IDENTIFIER ) a++ ;
    else if( temp->type == CONSTANT ) b++;
    else if( temp->type == SPECIAL ) c++;
    temp = temp->next ;
  }
  
  Printcase( 1, a ) ; 
  Printcase( 2, b ) ; 
  Printcase( 3, c ) ; 
  
}

void Tokentofind( TokenPtr head ) {
  TokenPtr temp = NULL;
  temp = head ;
  Str100 goal ;
  bool find = false ;
  printf( "請輸入要搜尋的 token : \n" ) ;
  scanf( "%s", & goal ) ;
  while ( temp != NULL ) {
    if ( strcmp( goal, temp->tokenStr ) == 0 && find == false ) {
     
      printf( "%s ", temp->tokenStr ) ;
      Printline( temp->firstAppearOn ) ;
      printf( "\n" ) ;
      find = true ;
    }
    temp = temp->next ;
  }
  
  if( !find ) printf( "查無此token : %s\n", goal ) ;
}

void Linetofind( TokenPtr head ) {
  TokenPtr temp = NULL;
  LinePtr templine = NULL ;
  ColumnPtr tempcolumn = NULL ;
  temp = head ;
  int goal = 0 ;
  printf( "請輸入要指定的 line : \n" ) ;
  scanf( "%d", & goal ) ;
  while ( temp != NULL ) {
    templine = temp->firstAppearOn ;
    while( templine != NULL ) {
      tempcolumn = templine->firstAppearAt ;
      while ( tempcolumn != NULL ) {
        if( goal == templine->line )
          
          printf( "%s\n", temp->tokenStr ) ;
        tempcolumn = tempcolumn->next ;
      }
      
      templine = templine->next ;
    }
    
    temp = temp->next ;
  }
  
}

void Printcase( int c, int num ) {
  int n = 0 ;
  int temp ;
  temp = num ;
  if( c == 1 ) printf( "Case1 共 " ) ;
  else if( c == 2 )printf( "Case2 共 " ) ;
  else if( c == 3 ) printf( "Case3 共 " ) ;
  
  printf( "%d個\n", num ) ;
}
