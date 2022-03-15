//資訊三乙 10727219 許志仲 
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <vector>
#include <algorithm>

using namespace std;


struct table {
	string token ;
};

struct info {
	int table ;
	int index ;
};

class Hash{
	
	
	public:
		Hash(){
			for( int i = 0 ; i < 100 ; i++ )
				str[i] = "" ;
			size = 0 ;
			
		}
		
		void p() {
			cout << size << endl ;
			for( int i = 0 ; i < 100 ; i++ )
				cout << str[i] << endl ;
			
		}
		
		bool insert( string token, int & value ){
			int index = value ;
			if( size < 100 ) {
				while( str[index].compare("") != 0 && str[index].compare( token ) != 0 ){
					index++ ;
					index = index % 100 ;
				}
				
				if ( str[index].compare("") == 0 ){
					str[index] = token ;
					size++ ;
				}
				
				value = index ;	
				return false ;
			}
			else
				return true ;
		}
	private:
		string str[100] ;
		int size ;
		
		
};

class Token{
	public:
		void readalltable() {
			readtable( "1", table1 );
			readtable( "2", table2 );
			readtable( "3", table3 );
			readtable( "4", table4 );

		}
		
		bool readFile(){	
			string inputfilename ;
			bool open ;
			do {
				cout << "Please enter FileName：";
				cin >> inputfilename ;
				fin.open( inputfilename.c_str()); // 開檔 
				if ( inputfilename == "q" ) cout << "quit" << endl;
				else if ( fin.fail() ) { 
	      			cout << inputfilename << " Open Failed, Please try again or press q to quit." << endl; // 不能開 
	    		} // if ( 找不到檔案 )
			}while(fin.fail() && inputfilename != "q" ) ;
			
			if( !fin.fail() )
				return true ;
			else
				return false ;
			
		}//readFile()
		
		void cut() {
			//fout.open( "resultOutput.txt", ios::trunc );
			fout.open( "ResultOutput.txt", ios::out );

			string line ;
			getline(fin, line) ;
			while( !fin.eof() ) {
				cout << line << endl ;
				fout << line << endl ;
				cuttoken( line + " " ) ;
				line = "" ;
				getline(fin, line) ;
			}
			if( !line.empty() ){
				cout << line << endl ;
				fout << line << endl ;
				cuttoken( line + " " ) ;
			}
			fout.close() ;
			//table6.p() ;
		}
		
		Token() {
			Hash table5 = Hash() ;
			Hash table6 = Hash() ;
			Hash table7 = Hash() ;
			table1.push_back("") ;
			table2.push_back("") ;
			table3.push_back("") ;
			table4.push_back("") ;
		}
		
		
	private:
		fstream fin;
		fstream fout;
		
		vector<string> table1 ;
  		vector<string> table2 ;
	  	vector<string> table3 ;
  		vector<string> table4 ;
  		Hash table5 ;
  		Hash table6 ;
  		Hash table7 ;

  		
  		vector<info> outlist ;
  		
  		void readtable( string num, vector<string> & table ) {
			string str = "" ;
			string filename = "Table" + num + ".table" ;
			fin.open( filename.c_str(), ios::in ) ;
			
			char ch = '\0' ;
			fin.get(ch) ;	
			while ( !fin.eof() ){
				
				if ( ch == '\n' ) {
					table.push_back(str) ;
					str = "" ;	
				}
				else {
					if( !iswhitespace( ch ) )
						str += ch ;
				}
				
				fin.get(ch) ;
			}
			
			table.push_back(str) ;
			fin.close() ;
			
			/*for ( int i = 0 ; i < table.size() ; i++ ) {
				cout << table.at(i) << endl ;
			}*/

		}
		
		void cuttoken( string line ) {
			string token = "" ;
			int table = -1 ;
			int index = -1 ;
			int next = -1 ;
			int i = 0 ;
			while( iswhitespace(line[i]) )
					i++ ;
					
			while( i < line.size() ) {
				table = -1 ;
				index = -1 ;
				token = getstring(i, line) ;
				//cout << "\"" << token << "\"" << endl ;
				if( ( lowtoup(token).compare("C") == 0 || lowtoup(token).compare("X") == 0 ) && 
						i < line.size() && line[i] == '\'' ){
					if( lowtoup(token).compare("C") == 0 ){
						next = 7 ;
					}
					else if( lowtoup(token).compare("X") == 0 ){
						next = 6 ;
					}
							
				}
					
				else{
					checktable( token, table, index, next ) ;
					next = -1 ;
				}	
					
				
				if( token.compare(".") == 0 )
					i = line.size() ;
				
				while( iswhitespace(line[i]) )
					i++ ;
			}
			/*if( line.compare( " " ) != 0 ){
				cout << endl ;
				fout << endl ;	
			}*/
			cout << endl ;
			fout << endl ;
			
		}
		
		string getstring( int & i, string line ) {
			if ( isletter( line[i] )  ) 
				return getstr( i, line ) ;
			else if ( isdight( line[i] ) )
				return getnum( i, line ) ;
			else if ( line[i] == '\'' )
				return getstr2( i, line ) ;
			else if ( isdelimiter( line[i] ) ) 
				return getdelimiter( i, line ) ;
			
			// else if ( ch == '/' ) return Getcomment( ch ) ;
		}
		
		string getstr( int & i, string line ) {
			string restr = "" ; // 初始化
			
			while ( isletter( line[i] ) || isdigit( line[i] ) ) {
				restr += line[i] ;
				i++ ;
			}
		  
			
			return restr ;		  
		}
		
		string getstr2( int & i, string line ) {
			string restr = "" ; // 初始化
			restr += line[i] ;
			i++ ;
			while ( !(line[i] == '\'' && line[i-1] != '\\' ) && i < line.size() ) {
				restr += line[i] ;
				i++ ;
			}
			
			restr += line[i] ;
			i++ ;
			return restr ;		  
		}
		
		string getnum( int & i, string line ) {
			string restr = "" ; // 初始化
			
			while ( isdigit( line[i] ) ) {
				restr += line[i] ;
				i++ ;
			}
		  
			
			return restr ;		  
		}
		
		string getdelimiter( int & i, string line ) {
			string restr = "" ; // 初始化
			restr += line[i] ;
			i++ ;
			
			return restr ;		  
		}
		
		string lowtoup(string str){
			string upper = "" ;
			for ( int i = 0 ; i < str.size() ; i++){
				if( str[i] >= 97 && str[i] <= 122 ) // a~z to A~Z
					upper = upper + char( str[i] - 32) ;					
				else  upper = upper + str[i] ;			 
			}//for
			
			return upper ;
		}//upToLowCase()
		
		bool isletter( char ch ) {
			if ( ch >= 'A' && ch <= 'Z' ) return true ;
			else if ( ch >= 'a' && ch <= 'z' ) return true ;
			else if (  ch == '_' ) return true ;
			else return false ;
		  
		}
		
		bool isdight( char ch ) {
			if ( ch >= '0' && ch <= '9' ) return true ;
			else return false ;
		  
		}
		
		bool isdelimiter( char ch ) {
			string temp = "" ;
			temp += ch ;
			for( int i = 0 ; i < table4.size() ; i++ ) {
				if( temp.compare( table4.at(i) ) == 0 )
					return true ;
			}
			
			return false ;
		  
		}
		
		bool iswhitespace( char ch ) {
			if ( ch == ' ' ) return true ;
			else if ( ch == '\t' ) return true ;
			else return false ;
  
		}
		
		void checktable( string token, int & table, int & index, int next ) {
			int i = findtable1to3( token, index ) ;
			//cout << i << endl ;
			bool full = false ;
			if ( findtable4( token, index ) ){
				table = 4 ;
			}
			else if ( token[0] == '\'' ){
				string temp = "" ;
				temp += token[0] ;
				checktable( temp, table, index, -1 ) ;
				if ( token[token.size()-1] == '\'' ){
					temp = token.substr( 1, token.size() - 2 ) ;
					//cout << temp ;
					if( !temp.empty() ) {
						checktable( temp, table, index, next ) ;
					}
					
					temp = "" ;
					temp += token[token.size() - 1] ;
					checktable( temp, table, index, -1 ) ;
					
				}
				i = 4;
			}
			else if ( next == 6 || next == 7 ){
				table = next ;
				
				if( table == 6 ) {
					index = counthashvalue( lowtoup(token) ) ;
					full = table6.insert(lowtoup(token), index) ;					
				}
				else if( table == 7 ) {
					index = counthashvalue( token ) ;
					full = table7.insert(token, index) ;
				}
			}
			else if( i != -1 ){
				table = i ;
			}
			else if ( isnum(token) ){
				table = 6 ;
				index = counthashvalue( token ) ;
				full = table6.insert(token, index) ;
			}
			else{
				//cout << "\"" << token << "\"" << endl ;
				table = 5 ;
				index = counthashvalue( token ) ;
				full = table5.insert(token, index) ;
				
			}
				
			if( i != 4 && !token.empty() && !full ) {
				cout << "(" << table << "," << index << ")" ;	
				fout << "(" << table << "," << index << ")" ;
			}
			
		}
		
		int findtable1to3( string token, int & index ) {
			string temp = lowtoup( token ) ;
			for( int i = 0 ; i < table1.size() ; i++ ) {
				if( temp.compare( lowtoup( table1.at(i) ) ) == 0 ){
					index = i ;
					return 1 ;	
				}

			}
			for( int i = 0 ; i < table2.size() ; i++ ) {
				if( temp.compare( lowtoup( table2.at(i) ) ) == 0 ){
					index = i ;
					return 2 ;	
				}
			}			
			for( int i = 0 ; i < table3.size() ; i++ ) {
				if( temp.compare( lowtoup( table3.at(i) ) ) == 0 ){
					index = i ;
					return 3 ;	
				}
			}
			
			return -1 ;
						
		}
		
		bool findtable4( string token, int & index ) {
			string temp = lowtoup( token ) ;
			for( int i = 0 ; i < table4.size() ; i++ ) {
				if( temp.compare( lowtoup( table4.at(i) ) ) == 0 ){
					index = i ;
					return true ;	
				}

			}
	
			return false ;
						
		}
		
		bool isnum( string token ){
			for ( int i = 0 ; i < token.size() ; i++){
				if( !isdigit( token[i] ) )
					 return false ;
		 
			}//for
			
			return true ;
		}
		
		int counthashvalue( string token ) {
			long long num = 0 ;
			for ( int i = 0 ; i < token.size() ; i++){
				num = num + int(token[i]) ;			  
			}//for
			num = num % 100 ;
			//cout << token << ":" << num ;
			return int(num) ;
		}
};


		
int main() {
	Token token ;
	token.readalltable() ;
	if( token.readFile() ){
		token.cut() ;	
		
	}
		

	
	return 0;
}
