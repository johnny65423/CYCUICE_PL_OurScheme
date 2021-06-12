#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <vector> 
#include <queue>
using namespace std ;

static fstream file ;
static fstream out ;


void FIFO( vector < char > list, int frame ) {
	
	out << "--------------FIFO-----------------------" << endl;
	
	int faultnum = 0 ;
  char temp[frame] ;
	
	for ( int i = 0 ; i < frame ; i++ )
	  temp[i] = '*' ;
	
	for (int i = 0 ; i < list.size( ) ; i++ ) {
		char ch = list.at(i) ;
		
		out << ch << "\t" ;
		
		int index = -1 ;
		bool fault = false ;
		
		for ( int j = 0 ; j < frame ; j++ ) {
			if ( ch == temp[j] ) {
				index = j ;
			}
		}


		if ( index == -1 ) {
			fault = true ;
			faultnum++ ;
			for ( int j = frame - 1 ; j > 0; j-- ) {
				temp[j] = temp[j - 1] ;
			}
			temp[0] = ch ;
		}
    
		for ( int j = 0 ; j < frame ; j++ ) {
			if ( temp[j] != '*' )
			  out << temp[j] ;
		}
    
		if (fault)
			out << "\tF";


		out << endl;
	}
  
	out << "Page Fault = " << faultnum << "  Page Replaces = " << faultnum - frame << "  Page Frames = " << frame << endl;
  out << endl ;
}

void LRU( vector < char > list, int frame ) {
	
	out << "--------------LRU-----------------------" << endl;
	
	int faultnum = 0 ;
  char temp[frame] ;
	
	for ( int i = 0 ; i < frame ; i++ )
	  temp[i] = '*' ;
	
	for (int i = 0 ; i < list.size( ) ; i++ ) {
		char ch = list.at(i) ;
		
		out << ch << "\t" ;
		
		int index = -1 ;
		bool fault = false ;
		
		for ( int j = 0 ; j < frame ; j++ ) {
			if ( ch == temp[j] ) {
				index = j ;
			}
		}


		if ( index == -1 ) {
			fault = true ;
			faultnum++ ;
      for ( int j = frame - 1 ; j > 0; j-- ) {
				temp[j] = temp[j - 1] ;
			}
			temp[0] = ch ;

		}
    else {
			for ( int j = index ; j > 0; j-- ) {
				temp[j] = temp[j - 1] ;
			}
			temp[0] = ch ;
    }     
    
		for ( int j = 0 ; j < frame ; j++ ) {
			if ( temp[j] != '*' )
			  out << temp[j] ;
		}
    
		if (fault)
			out << "\tF";


		out << endl;
	}
  
	out << "Page Fault = " << faultnum << "  Page Replaces = " << faultnum - frame << "  Page Frames = " << frame << endl;
  out << endl ;
}

void LFU( vector < char > list, int frame ) {
	
	out << "--------------Least Frequently Used Page Replacement-----------------------" << endl;
	
	int faultnum = 0 ;
  char temp[frame] ;
	int freq[frame] ;
	for ( int i = 0 ; i < frame ; i++ ) {
	  temp[i] = '*' ;
	  freq[i] = 0 ;
  }
  
	for (int i = 0 ; i < list.size( ) ; i++ ) {
		char ch = list.at(i) ;
		int chnum = (int)ch - 48 ;
    if ( chnum >9 || chnum < 0 )
      chnum == 0 ;
		out << ch << "\t" ;
		
		int index = -1 ;
		bool fault = false ;
		
		for ( int j = 0 ; j < frame ; j++ ) {
			if ( ch == temp[j] ) {
				index = j ;
			}
		}


		if ( index == -1 ) {
			fault = true ;
			faultnum++ ;
      
      int f = 0 ;
      while ( f < frame && temp[f] != '*' )
        f++ ;
       
      if ( f != frame ) {
        for ( int j = frame - 1 ; j > 0; j-- ) {
          temp[j] = temp[j - 1] ;
          freq[j] = freq[j - 1] ;
        }
        temp[0] = ch ;
        freq[0] = 0 ;
      }
      else {
        int min = freq[frame - 1] ;
        int ind = frame - 1;
        for ( int j = frame - 1 ; j >= 0; j-- ){

        	
          if ( freq[j] < min ) {
            ind = j ;
            min = freq[j] ;     
          }            
        } // for

        for ( int j = ind ; j > 0; j-- ) {
          temp[j] = temp[j - 1] ;
          freq[j] = freq[j - 1] ;
        }
        temp[0] = ch ;
        freq[0] = 0 ;
        
        
      } // else

		}
    else {
			freq[ index ] +=1 ;
    }     
    
    
    
    
    
    
		for ( int j = 0 ; j < frame ; j++ ) {
			if ( temp[j] != '*' )
			  out << temp[j] ;
		}
    
		if (fault)
			out << "\tF";


		out << endl;
	}
  
	out << "Page Fault = " << faultnum << "  Page Replaces = " << faultnum - frame << "  Page Frames = " << frame << endl;
  out << endl ;
}

void MFU( vector < char > list, int frame ) {
	
	out << "--------------Most Frequently Used Page Replacement -----------------------" << endl;
	
	int faultnum = 0 ;
  char temp[frame] ;
	int freq[frame] ;
	for ( int i = 0 ; i < frame ; i++ ) {
	  temp[i] = '*' ;
	  freq[i] = 0 ;
  }
  
	for (int i = 0 ; i < list.size( ) ; i++ ) {
		char ch = list.at(i) ;
		int chnum = (int)ch - 48 ;
    if ( chnum >9 || chnum < 0 )
      chnum == 0 ;
		out << ch << "\t" ;
		
		int index = -1 ;
		bool fault = false ;
		
		for ( int j = 0 ; j < frame ; j++ ) {
			if ( ch == temp[j] ) {
				index = j ;
			}
		}


		if ( index == -1 ) {
			fault = true ;
			faultnum++ ;
      
      int f = 0 ;
      while ( f < frame && temp[f] != '*' )
        f++ ;
       
      if ( f != frame ) {
        for ( int j = frame - 1 ; j > 0; j-- ) {
          temp[j] = temp[j - 1] ;
          freq[j] = freq[j - 1] ;
        }
        temp[0] = ch ;
        freq[0] = 0 ;
      }
      else {
        int max = freq[frame - 1] ;
        int ind = frame - 1;
        for ( int j = frame - 1 ; j >= 0; j-- ){

        	
          if ( freq[j] > max ) {
            ind = j ;
            max = freq[j] ;     
          }            
        } // for

        for ( int j = ind ; j > 0; j-- ) {
          temp[j] = temp[j - 1] ;
          freq[j] = freq[j - 1] ;
        }
        temp[0] = ch ;
        freq[0] = 0 ;
        
        
      } // else

		}
    else {
			freq[ index ] +=1 ;
    }     
    
    
    
    
    
    
		for ( int j = 0 ; j < frame ; j++ ) {
			if ( temp[j] != '*' )
			  out << temp[j] ;
		}
    
		if (fault)
			out << "\tF";


		out << endl;
	}
  
	out << "Page Fault = " << faultnum << "  Page Replaces = " << faultnum - frame << "  Page Frames = " << frame << endl;
  
}

void LFULRU( vector < char > list, int frame ) {
	
	out << "--------------Least Frequently Used LRU Page Replacement-----------------------" << endl;
	
	int faultnum = 0 ;
  char temp[frame] ;
	int freq[frame] ;
	for ( int i = 0 ; i < frame ; i++ ) {
	  temp[i] = '*' ;
	  freq[i] = 0 ;
  }
  
	for (int i = 0 ; i < list.size( ) ; i++ ) {
		char ch = list.at(i) ;
		int chnum = (int)ch - 48 ;
    if ( chnum >9 || chnum < 0 )
      chnum == 0 ;
		out << ch << "\t" ;
		
		int index = -1 ;
		bool fault = false ;
		
		for ( int j = 0 ; j < frame ; j++ ) {
			if ( ch == temp[j] ) {
				index = j ;
			}
		}


		if ( index == -1 ) {
			fault = true ;
			faultnum++ ;
      
      int f = 0 ;
      while ( f < frame && temp[f] != '*' )
        f++ ;
       
      if ( f != frame ) {
        for ( int j = frame - 1 ; j > 0; j-- ) {
          temp[j] = temp[j - 1] ;
          freq[j] = freq[j - 1] ;
        }
        temp[0] = ch ;
        freq[0] = 0 ;
      }
      else {
        int min = freq[frame - 1] ;
        int ind = frame - 1;
        for ( int j = frame - 1 ; j >= 0; j-- ){

        	
          if ( freq[j] < min ) {
            ind = j ;
            min = freq[j] ;     
          }            
        } // for

        for ( int j = ind ; j > 0; j-- ) {
          temp[j] = temp[j - 1] ;
          freq[j] = freq[j - 1] ;
        }
        temp[0] = ch ;
        freq[0] = 0 ;
        
        
      } // else

		}
    else {
			freq[ index ] +=1 ;
			char tc = temp[index] ;
			int tf = freq[index] ;
			
			for ( int j = index ; j > 0; j-- ) {
          temp[j] = temp[j - 1] ;
          freq[j] = freq[j - 1] ;
        }
      temp[0] = tc ;  
      freq[0] = tf ;
    }     
    
    
    
    
    
    
		for ( int j = 0 ; j < frame ; j++ ) {
			if ( temp[j] != '*' )
			  out << temp[j] ;
		}
    
		if (fault)
			out << "\tF";


		out << endl;
	}
  
	out << "Page Fault = " << faultnum << "  Page Replaces = " << faultnum - frame << "  Page Frames = " << frame << endl;
  out << endl;
}

void MFULRU( vector < char > list, int frame ) {
	
	out << "--------------Most Frequently Used LRU Page Replacement -----------------------" << endl;
	
	int faultnum = 0 ;
  char temp[frame] ;
	int freq[frame] ;
	for ( int i = 0 ; i < frame ; i++ ) {
	  temp[i] = '*' ;
	  freq[i] = 0 ;
  }
  
	for (int i = 0 ; i < list.size( ) ; i++ ) {
		char ch = list.at(i) ;
		int chnum = (int)ch - 48 ;
    if ( chnum >9 || chnum < 0 )
      chnum == 0 ;
		out << ch << "\t" ;
		
		int index = -1 ;
		bool fault = false ;
		
		for ( int j = 0 ; j < frame ; j++ ) {
			if ( ch == temp[j] ) {
				index = j ;
			}
		}


		if ( index == -1 ) {
			fault = true ;
			faultnum++ ;
      
      int f = 0 ;
      while ( f < frame && temp[f] != '*' )
        f++ ;
       
      if ( f != frame ) {
        for ( int j = frame - 1 ; j > 0; j-- ) {
          temp[j] = temp[j - 1] ;
          freq[j] = freq[j - 1] ;
        }
        temp[0] = ch ;
        freq[0] = 0 ;
      }
      else {
        int max = freq[frame - 1] ;
        int ind = frame - 1;
        for ( int j = frame - 1 ; j >= 0; j-- ){

        	
          if ( freq[j] > max ) {
            ind = j ;
            max = freq[j] ;     
          }            
        } // for

        for ( int j = ind ; j > 0; j-- ) {
          temp[j] = temp[j - 1] ;
          freq[j] = freq[j - 1] ;
        }
        temp[0] = ch ;
        freq[0] = 0 ;
        
        
      } // else

		}
    else {
			freq[ index ] +=1 ;
			char tc = temp[index] ;
			int tf = freq[index] ;
			
			for ( int j = index ; j > 0; j-- ) {
          temp[j] = temp[j - 1] ;
          freq[j] = freq[j - 1] ;
        }
      temp[0] = tc ;  
      freq[0] = tf ;
    }     
    
    
    
    
    
    
		for ( int j = 0 ; j < frame ; j++ ) {
			if ( temp[j] != '*' )
			  out << temp[j] ;
		}
    
		if (fault)
			out << "\tF";


		out << endl;
	}
  
	out << "Page Fault = " << faultnum << "  Page Replaces = " << faultnum - frame << "  Page Frames = " << frame << endl;

}


int main() {

	string filename ;
	int method ;
	int timeslice ;
	cout << "½Ð¿é¤JÀÉ¦W¡G" ;
	// cin >> filename;

	// string name = filename + ".txt" ;
	string name = "input2.txt" ;
	file.open( name.c_str(), ios::in ) ;
	if(file) {
	  char ch ;
	  int frame ;
	  vector < char > list ;

    file >> frame ;

		while ( !file.eof() ) {
		  file >> ch ; 
			if ( !file.eof() )	
  		  list.push_back( ch ) ;
		}
	  

		
		name = "out_" + name ;
		out.open( name.c_str(), ios::out ) ;
		FIFO( list, frame ) ;
    LRU( list, frame ) ;
    LFU( list, frame ) ;
    MFU( list, frame ) ;
    LFULRU( list, frame ) ;
    MFULRU( list, frame ) ;
    out.close() ;
	}
	else {
		cout << "File open Error" ;
	}	
	

} // main()	

