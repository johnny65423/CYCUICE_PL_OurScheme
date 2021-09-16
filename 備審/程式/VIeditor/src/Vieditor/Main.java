
package Vieditor;

import CYICE.* ;

import java.util.Vector ;

class G {
  static ICEInputStream sIn = null ;
  static int sTestNum = -1 ;
  
} // G 

class Editor {
  private Vector<Buffer> mbuffer ; 
  private Buffer mnowbuffer ;
  
  Editor() throws Throwable {
    mbuffer = new Vector<Buffer>(); 
    mnowbuffer = new Buffer();
    mnowbuffer.mname = "Buffer-0" ;
    mbuffer.add( mnowbuffer ) ; 
  } // Editor()
  
  int Getsize() {
    return mbuffer.size(); 
  } // Getsize()
  
  boolean Isint( String cmd ) {
    int len = cmd.length(); 
    int a = 0; 
    for ( a = 0; a < len && cmd.charAt( a ) <= '9' && cmd.charAt( a ) >= '0' ; a++ ) {
    } // for
    
    if ( a == len ) {
      return true; 
    } // if
    
    return false;
  } // Isint()
  
  void Work ( String comd, int goalint, String goalstr ) throws Throwable {
    int templine = mnowbuffer.Getline();
    
    if( goalint != -1 ) {
      if( goalint > mnowbuffer.Getsize() ) {
        comd = "outsize";
      } // if
      else {
        mnowbuffer.Setline( goalint ) ;
      } // else
    } // if

    if( goalstr.equals( "$" ) ) mnowbuffer.Setline( mnowbuffer.Getsize() );
    
    if ( comd.equals( "a" ) ) {
      mnowbuffer.Add();
    } // if 
    else if ( comd.equals( "i" ) ) {
      if ( mnowbuffer.Getsize() == 0 ) mnowbuffer.Add();
      else mnowbuffer.Insert();
    } // else if 
    else if ( comd.equals("c" ) ) {
      mnowbuffer.Change ( ) ; 
    } // else if 
    else if ( comd.equals("d")) {
      mnowbuffer.Delete(); 
    } // else if 
    else if ( comd.equals( "p" ) ) {
      mnowbuffer.Print();
      mnowbuffer.Setline( templine );
    } // else if
    else if ( comd.equals("%p" ) ) {
      mnowbuffer.Printall();
    } // else if 
    else if ( comd.equals( "$" ) ) {
      goalint = mnowbuffer.Getsize() ; 
      mnowbuffer.Changeline ( goalint ) ; 
      System.out.print( mnowbuffer.Getline () + " : " ) ;
      mnowbuffer.Print(); 
    } // else if
    else if ( comd.equals(".")) {
      System.out.print( mnowbuffer.Getline () + " : ") ;
      mnowbuffer. Print();
    } // else if 
    else if ( comd.equals( "lb" ) ) {
      Bufferlistprint();
    } // else if 
    else if ( comd.equals("b" ) ) { 
      if ( Cmd.sname.equals("")) {
        System.out.print( mnowbuffer.Getname() + " " + mnowbuffer.Getsize() + "\n" );
      } // else if
      else mnowbuffer.Changename(); 
    } // else if
    else if ( comd.equals("nb" ) ) {
       Newbuffer(); 
    } // else if
    else if ( comd.equals("cb" ) ) {
      Changebuffer(); 
    } // else if 
    else if ( Isint ( comd ) ) {
      goalint = Integer.parseInt( comd ) ; 
      if( goalint > mnowbuffer.Getsize() ) {
        System.out.print( "Error :  Line number out of range! Please try again."  ) ;
      } // if
      else {
        mnowbuffer.Changeline ( goalint ); 
        System.out.print( mnowbuffer.Getline () + " : "); 
        mnowbuffer.Print();
      } // else
    } // else if 
    else if ( comd.equals( "outsize" ) ) {
      System.out.print( "Error :  Line number out of range! Please try again." ); 
    } // else if
    else {
      System.out.print("Error : Unprocessable command! Please try again.\n" );
    } // else 
    
  
    
  } // Work()
  
  void Bufferlistprint() {
      Buffer temp = null ; 
    for (int i = 0; i < mbuffer.size(); i++) {
    temp = mbuffer.get(i);
    System.out.print( temp.Getname() + " " + temp.Getsize() + "\n"); 
    } // for
  } // Bufferlistprint()
  
  void Newbuffer() {
    Buffer newbuffer = new Buffer(); 
    mnowbuffer = newbuffer ; 
    if ( Cmd.sname.equals("")) {
      mnowbuffer.mname = "Buffer-" + mbuffer.size();
    } // if
    else {
      mnowbuffer.mname = Cmd.sname ; 
    } // else
      
    mbuffer.add( newbuffer ) ;
    
  } // Newbuffer()
  
  void Changebuffer() {
    Buffer temp = null;
    boolean find = false; 
    int i; 
    for (i = 0 ; i < mbuffer.size() ; i++) {
      temp = mbuffer.get(i); 
      if ( temp.Getname().equals( Cmd.sname ) ) {
        mnowbuffer = temp ;
        find = true ;
        
      } // if
      
    } // for

    if( !find )
      System.out.print( "Error : There is no such buffer!" ) ;
  } // Changebuffer()
  
} // class Editor

        


class Buffer {
  private Vector<String> mdata ;
  private int mnowline = -1 ;
  public String mname ;
  
  Buffer() {
    mdata = new Vector<String>() ;
    mnowline = 0 ;
  } // Buffer()
  
  int Getsize() {
    return mdata.size() ;
  } // Getsize()
  
  String Getname() {
    return mname ;
  } // Getname()
  
  int Getline() {
    return mnowline ;
  } // Getline()
  
  void Setline( int i ) {
    mnowline = i ;
  } // Setline()
  
  void Add() throws Throwable {
    String temp = "" ;
    char ch = '\0' ;
    
    ch = G.sIn.ReadChar() ;
    while (ch != '\n') {
      temp += ch ;
      ch = G.sIn.ReadChar() ;
    } // while
    
    while ( !temp.equals( "." ) ) {
      mdata.add( mnowline, temp );
      mnowline++ ;
      temp = "" ;
      ch = G.sIn.ReadChar() ;
      while (ch != '\n') {
        temp += ch ;
        ch = G.sIn.ReadChar() ;
        
      } // while
      
    } // while
    
  } // Add()
  
  void Insert() throws Throwable {
    
    mnowline-- ;
    Add() ;

    
  } // Insert()
  
  void Delete() {
    int i = mdata.size() ;
    if( i == 0 ) {
      System.out.print( "Error : There is no data!" );
    } // if
      else {
      if( i == mnowline ) {  
        mdata.remove( mnowline - 1 )  ;
        mnowline = mnowline - 1 ;
      } // if
      else {  
        mdata.remove( mnowline - 1 )  ;
      } // else
      
    } // else
  } // Delete()
  
  void Change() throws Throwable {
    int i = mdata.size() ;
    if( i == 0 ) System.out.print( "Error : There is no data!" );
    else {
      if( i == mnowline ) {  
        Delete()  ;
        Add() ;
      } // if
      else {  
        Delete()  ;
        Insert() ;
      } // else
      
      // if( mdata.size() + 1 == i )
        //mnowline ++ ;
      // 若刪掉應跟d一樣指向下一行
      
    } // else
    
  } // Change()
  
  void Printall() {
    int i = mdata.size() ;
    for ( int j = 0 ; j < i ; j++ ) {
      System.out.print( mdata.get( j ) + "\n" );
    } // for
  } // Printall()
  
  void Print() {
    int i = mdata.size() ;
    if( i == 0 ) System.out.print( "Error : There is no data!" );
    else {
        System.out.print( mdata.get( mnowline - 1 ) + "\n" );
    } // else
    
  } // Print()
  
  void Changeline( int i ) {
    mnowline = i ;
  } // Changeline()
  
  void Changename() {
    mname = new String( Cmd.sname ) ;
  } // Changename()
  
  
} // Buffer 


class Cmd  {
  static String scmd = ""; 
  static String sname = ""; 
  static int snum = -1;
  static boolean sfinishcmd = false ;
  
  static String Getname( String temp ) {
    String restr = "" ;
    char ch = '\0', before = '\0' ;
    int i ;
    boolean findb = false ;
    boolean find = false ;
    
    i = temp.indexOf(  "b" ) + 1 ;
    
    if( i != -1 && i < temp.length() ) {
      while(temp.charAt( i ) == ' ' )
        i++ ;
      while ( !find && i < temp.length() ) {
        ch = temp.charAt( i ) ;

        if( ch == ' ' )
          find = true ;
        else {
          restr += ch ;
          i++ ;
        } // else
      
      } // while

    } // if
    return restr ;
  } // Getname()
  
  static boolean Isint( String cmd ) {
    int len = cmd.length(); 
    int a ; 
    for ( a = 0; a < len && cmd.charAt( a ) <= '9' && cmd.charAt( a ) >= '0' ; a++ )
    
    if ( a == len ) {
      return true; 
    } // if
    
    return false;
  } // Isint()
  
  static void Getcmd2() throws Throwable {
    scmd = ""; 
    sname = ""; 
    snum = -1 ;
    sfinishcmd = false ;
    // int finish = -1 ; // 命令格數, 0是拉基 -1為沒找到
    String temp ;
    String tempread ;
    temp = G.sIn.ReadInputLine(); 
    Trytofind( Clean( temp ), temp  ) ;  //拉基也是已找到
    
    while( !sfinishcmd ) {
      tempread = G.sIn.ReadInputLine(); 
      temp = temp + tempread ;
      Trytofind( Clean( temp ), temp ) ;
      
    } // while
    
    // System.out.print(     scmd  + "/" + sname + "/" + snum + "\n" );
    // System.out.print( temp ) ;

  } // Getcmd2()
  
    static void Getcmd() throws Throwable {
    scmd = ""; 
    sname = ""; 
    snum = -1 ;
    char ch  = '\0';
    String temp1 ;
    String temp2 ;
    scmd = G.sIn.ReadString(); 
    ch = G.sIn.ReadChar(); 
    while ( ch != '\n') { 
      if ( ch != ' ' ) {
        sname += ch ; 
      } // if
      ch = G.sIn.ReadChar(); 
    } // while
  } // Getcmd()
  
  static boolean Legalcmdhead( char ch ) {
    if( ch == '.' || ch == '$' || ch == '%' )
      return true ;
    
    else if( ch == 'a' || ch == 'i' || ch == 'c' )
      return true ; 
    
    else if( ch == 'b' || ch == 'n' || ch == 'l' )
      return true ; 
    
    else if( ch == 'd' || ch == 'p' || ch == 'q' )
      return true ; 
    
    else if( Character.isDigit( ch ) )
      return true ; 
    
    return false ;
    
    
  } // Legalcmd()
 
  static boolean Notfinish( String temp )  {
    if( temp.equals( "n" ) || temp.equals( "l" ) ) {
      return true ; 
    } // if
    else if( temp.equals( "%" ) ) {
      return true ; 
    } // else
    
    return false ;
   
  } // Notfinish()
  
  static boolean Intandint( String temp ) {
    int i = 0 ;
    char ch = temp.charAt( i ) ;
    String restr = "" ;
    boolean find = false ;
    while ( ch == ' ' ) {
      i++;
      ch = temp.charAt( i ) ;
    } // while
    
    if( Character.isDigit( ch ) ) {
      restr += ch ;
      i++ ;
      ch = temp.charAt( i ) ;
      while( Character.isDigit( ch ) ) {
        restr += ch ;
        i++ ;
        ch = temp.charAt( i ) ;
      } // while
      
      if( ch == ' ' ) {
        i++ ;
        ch = temp.charAt( i ) ;
        while ( ch == ' ' ) {
          i++ ;
          ch = temp.charAt( i ) ;
        } // while
        if( Character.isDigit( ch ) ) {
          scmd = restr ; 
          find = true ;
        } // if
      } // if
    } // if
    
    return find ;
  } // Intandint()
  
  static boolean Intandmayor( String temp ) {
    String numstr = "" ;
    int i = 0 ;
    if( Character.isDigit( temp.charAt( i ) ) ) {
      numstr += temp.charAt( i ) ;
      i++;
      while( Character.isDigit( temp.charAt( i ) ) ) {
        numstr += temp.charAt( i ) ;
        i++;
      } // while
    } // if
    
    if( Mayorcmd( temp.charAt( i ) ) && !numstr.equals( "" ) ) {
      scmd = Character.toString( temp.charAt( i ) ) ;
      snum = Integer.parseInt( numstr );
      return true ;
    } // if
    else if( !Mayorcmd( temp.charAt( i ) ) && !numstr.equals( "" ) ) {
      scmd = numstr ;
      return true ;
    } // else if
    
    return false ;
    
  } // Intandmayor()
  
  static boolean Mayorcmd( char ch ) {
    if( ch == 'a' || ch == 'i' || ch == 'p' )
      return true ; 
    else if( ch == 'd' || ch == 'c' )
      return true ; 

    
    return false ;
    
    
  } // Mayorcmd()
  
  static boolean Headofb( char ch ) {
    if( ch == 'l' || ch == 'n' )
      return true ; 
    else if( ch == 'c' )
      return true ; 

    
    return false ;
    
    
  } // Headofb()
  
  static boolean Vicecmd( char ch ) {
    if( ch == '.' || ch == '$' )
      return true ;
    
    return false ;
    
    
  } // Vicecmd()
  
  boolean Maybestill( String cmd ) {
    if( cmd.equals( "n" ) || cmd.equals( "c" ) )
      return true ;
    else if( cmd.equals( "l" ) || cmd.equals( "%" ) )
      return true ;
    
    return true ;
    
  } // Maybestill()
  
  static String Clean( String temp ){
    String returnstr = new String( "" ) ;
    char tempch ;
    for ( int i = 0 ; i < temp.length() ; i ++ ) {
      tempch = temp.charAt( i ) ;
      if( tempch != ' ' )
        returnstr += tempch ;
    } // for
    
    return returnstr ;
    
  } // Clean()
  
  static void Trytofind( String temp, String origin ) {
    String set = "" ;
    if ( temp.length() == 0 )  ;    // no
    else if( !Legalcmdhead( temp.charAt( 0 ) ) ) {
      scmd = "trash" ;  //拉基
      sfinishcmd = true ;
    } // if
    else if( Notfinish( temp ) ) sfinishcmd = false ;
    else {
      if( temp.length() == 1 ) {
        scmd = temp ;  //一個
        sfinishcmd = true ;
      } // if
      else {
          if( Mayorcmd( temp.charAt( 1 ) ) && Vicecmd( temp.charAt( 0 ) ) ) {
            scmd = Character.toString( temp.charAt( 1 ) ) ;  //一個
            sname = Character.toString( temp.charAt( 0 ) ) ;
            sfinishcmd = true ;
          } // if
          else if( Isint( temp ) ) {
            scmd = temp ;
            sfinishcmd = true ;
          } // else if
          else if( !Mayorcmd( temp.charAt( 1 ) ) && Vicecmd( temp.charAt( 0 ) ) ) {
            scmd = Character.toString( temp.charAt( 0 ) ) ;  //一個
            sfinishcmd = true ;
          } // else if
          else if( temp.charAt( 0 ) == '%' && temp.charAt( 1 ) == 'p'  ) {
            scmd = "%p"  ;  //一個
            sfinishcmd = true ;
          } // else if
          else if( temp.charAt( 0 ) == '%' && temp.charAt( 1 ) != 'p'  ) {
            scmd = "trash"  ;  //一個
            sfinishcmd = true ;
          } // else if
          else if( Headofb( temp.charAt( 0 ) ) && temp.charAt( 1 ) != 'b' ) {
            scmd = "trash"  ;
            sfinishcmd = true ;
          } // else if
          else if( Headofb( temp.charAt( 0 ) ) && temp.charAt( 1 ) == 'b' ) {
            scmd = Character.toString( temp.charAt( 0 ) ) ;
            scmd +=  Character.toString( temp.charAt( 1 ) ) ;  //一個
            sname = Getname( origin ) ;
            sfinishcmd = true ;
          } // else if
          else if( temp.charAt( 0 ) == 'b' ) {
            scmd = "b" ;
            sname = Getname( origin ) ;
            sfinishcmd = true ;
          } // else if
          else if( Mayorcmd( temp.charAt( 0 ) ) ) {
            scmd = Character.toString( temp.charAt( 0 ) ) ;  //一個
            sfinishcmd = true ;
          } // else if
          else if( Intandint( origin ) ) {
            sfinishcmd = true ;      
          } // else   
          else if( Intandmayor( temp ) ) {
            sfinishcmd = true ;      
          } // else
      }  // else
    } // else
    
 
  } // Trytofind()
  

  

} // class Cmd


class Main {


  public static void main(String[] args) throws Throwable {
    Buffer test = new Buffer() ;
    
    Cmd comd = new Cmd() ;
    Editor editor = new Editor() ;
    G.sIn = new ICEInputStream() ;
    
    
    System.out.print( "歡迎使用中原資工Line editor (許志仲)..." );
    System.out.print( "\n> " ) ;
    Cmd.Getcmd2() ;

    while ( !comd.scmd.equals( "q" )) {
      editor.Work( comd.scmd, comd.snum, comd.sname ) ;
      System.out.print( "> " ) ;
      Cmd.Getcmd2() ;
    } // while
    
    System.out.print( "謝謝使用中原資工Line editor！" );
  
    // Cmd.Getcmd2();
    
  }
  
}
