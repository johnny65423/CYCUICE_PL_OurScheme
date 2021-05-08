#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <vector> 
#include <queue>
using namespace std ;

static fstream file ;

struct Process{
	int id ;
	int cpuburst ;
	int arrivaltime ;
	int priority ;
	
};

bool check(const Process& x, const Process& y){
	if( x.arrivaltime != y.arrivaltime )
		return x.arrivaltime < y.arrivaltime ;
	else 
		return x.id < y.id ;
	
}

struct checkSRTF{
	bool operator() (const Process& x, const Process& y){
		if( x.cpuburst != y.cpuburst )
			return x.cpuburst > y.cpuburst ;
		else {
			if( x.arrivaltime != y.arrivaltime )
				return x.arrivaltime > y.arrivaltime ;
			else 
				return x.id > y.id ;
		}
	}
};

struct checkPPRR{
	bool operator() (const Process& x, const Process& y){

		  return x.priority > y.priority ;
 
	}
};

char getid( int num ) {
	if ( num > 35 || num < 0 )
		return '#' ;
	else if ( num < 10 )
		return num + 48 ;
	else 
		return num + 55 ;	

}

class Scheduler {
	public:
		void readfile(){
		  string info ;
			file >> method ;
			file >> timeslice ;
			// cout << method << " " << timeslice << endl ;
			getline( file, info ) ;
			getline( file, info ) ;

			while( !file.eof() ) {
				readprocess() ;
			}	

		}
		
		void setfile(){
			// sort(processlist.begin(),processlist.end(), check) ;
			
			for ( int i = 0 ; i < processlist.size() ; i++ ) {
				cout << processlist.at(i).id << "\t" ;
				cout << processlist.at(i).cpuburst << "\t" ;
				cout << processlist.at(i).arrivaltime << "\t" ;
				cout << processlist.at(i).priority << endl ;
			}
		}	

		void makeschedule(){
			// cout << method ;
			switch ( method ) {
				case 1 :
					FCFS() ;
				case 2 :
					RR() ;
				case 3 :
					SRTF();
				case 6 :
					PPRR() ;	
				/*
				
				case 4 :
					PPRR();
				case 5 :
					HRRN();
				case 6 :
					allmethod() ;
				*/
			
			} // switch
			
		}

	private:
		vector<Process> processlist ;
		int method ;
		int timeslice ;
		
		void readprocess() {
			Process temp ;
			
			file >> temp.id >> temp.cpuburst >> temp.arrivaltime >> temp.priority ;
			if (!file.eof())
				processlist.push_back( temp ) ;
			
		}	

		void FCFS(){
			cout << "FCFS" << endl ;
			int time = 0 ;
			vector< Process > list ;
			list.assign( processlist.begin(), processlist.end() );
			sort( list.begin(),list.end(), check ) ;
			queue<Process> pqueue ;
			int nowwork = -1 ;
			int remaindertime = 0 ;
			while ( !pqueue.empty() || !list.empty() ) {
				for ( int i = 0 ; i < list.size() ; i++ ) {
					if( list.at(i).arrivaltime == time ) {
						pqueue.push( list.at(i) ) ;
						list.erase(list.begin() + i) ;
						i = -1 ;
					}
				}
				
				if( remaindertime == 0 && !pqueue.empty() ) {
					Process temp = pqueue.front() ;
					pqueue.pop() ;
					remaindertime = temp.cpuburst ;	
					nowwork = temp.id ;
				}
				else if( remaindertime == 0 && pqueue.empty() )
					nowwork = -1 ;
					
				if( nowwork != -1 )
					cout << getid(nowwork) ;
				else 
					cout << '-' ;	

				time++ ;
				if( nowwork != -1 )
					remaindertime-- ;
			}

			while( remaindertime > 0 ) {
				if( nowwork != -1 )
					cout << getid(nowwork) ;
				else 
					cout << '-' ;	
				remaindertime-- ;
			}

		}
		
		void RR(){
			cout << "RR" << endl ;
			int time = 0 ;
			vector< Process > list ;
			list.assign( processlist.begin(), processlist.end() );
			sort( list.begin(),list.end(), check ) ;
			queue<Process> pqueue ;
			int nowwork = -1 ;
			int remaindertime = 0 ;
			int runtime = 0 ;
			while ( !pqueue.empty() || !list.empty() ) {
				for ( int i = 0 ; i < list.size() ; i++ ) {
					if( list.at(i).arrivaltime == time ) {
						pqueue.push( list.at(i) ) ;
						list.erase(list.begin() + i) ;
						i = -1 ;
					}
				}
				// cout << runtime << " " << timeslice << " " << remaindertime << endl ;
				if( runtime == timeslice && remaindertime != 0 ) {
					// cout << "cut" << endl ;
					Process temp ;
					temp.id = nowwork ;
					temp.cpuburst = remaindertime ;
					pqueue.push( temp ) ;
					runtime = 0 ;
					temp = pqueue.front() ;
					pqueue.pop() ;
					remaindertime = temp.cpuburst ;	
					nowwork = temp.id ;
				}	
				
				if( remaindertime == 0 && !pqueue.empty() ) {
					Process temp = pqueue.front() ;
					pqueue.pop() ;
					remaindertime = temp.cpuburst ;	
					nowwork = temp.id ;
					runtime = 0 ;
				}
				else if( remaindertime == 0 && pqueue.empty() )
					nowwork = -1 ;
					
				if( nowwork != -1 )
					cout << getid(nowwork) ;
				else 
					cout << '-' ;	

				time++ ;
				runtime++ ;
				if( nowwork != -1 )
					remaindertime-- ;
			}

			while( remaindertime > 0 ) {
				if( nowwork != -1 )
					cout << getid(nowwork) ;
				else 
					cout << '-' ;	
				remaindertime-- ;
			}

		}
		
		void SRTF(){
			cout << "SRTF" << endl ;
			int time = 0 ;
			vector< Process > list ;
			list.assign( processlist.begin(), processlist.end() );
			sort( list.begin(),list.end(), check ) ;
			// queue<Process> pqueue ;
			priority_queue<Process, vector<Process>, checkSRTF > pqueue;
			int nowwork = -1 ;
			int arrtime ;
			int remaindertime = 0 ;
			while ( !pqueue.empty() || !list.empty() ) {
				for ( int i = 0 ; i < list.size() ; i++ ) {
					if( list.at(i).arrivaltime == time ) {
						pqueue.push( list.at(i) ) ;
						list.erase(list.begin() + i) ;
						i = -1 ;
					}
				}
				//if( !pqueue.empty() )
				//	cout << "*" << pqueue.size() << endl;
				if( !pqueue.empty() && nowwork != -1 && remaindertime > pqueue.top().cpuburst ) {
					// cout << "cut" << endl ;
					Process temp ;
					temp.id = nowwork ;
					temp.cpuburst = remaindertime ;
					temp.arrivaltime = arrtime ;
					pqueue.push( temp ) ;
					temp = pqueue.top() ;
					pqueue.pop() ;
					remaindertime = temp.cpuburst ;	
					nowwork = temp.id ;
				}	
				
				if( remaindertime == 0 && !pqueue.empty() ) {
					Process temp = pqueue.top() ;
					pqueue.pop() ;
					remaindertime = temp.cpuburst ;	
					arrtime = temp.arrivaltime ;
					nowwork = temp.id ;
				}
				else if( remaindertime == 0 && pqueue.empty() )
					nowwork = -1 ;
					
				if( nowwork != -1 )
					cout << getid(nowwork) ;
				else 
					cout << '-' ;	

				time++ ;
				if( nowwork != -1 )
					remaindertime-- ;
			}

			while( remaindertime > 0 ) {
				if( nowwork != -1 )
					cout << getid(nowwork) ;
				else 
					cout << '-' ;	
				remaindertime-- ;
			}

		}
		
		void ins( vector<Process> & list, Process temp ) {
			if( list.empty() )
					list.push_back(temp) ;
			else {
				int index = list.size() - 1 ;
				while ( index >= 0 && list.at(index).priority > temp.priority ) {
					index-- ;
				}
				index++ ;
				list.insert( list.begin()+index, temp ) ;
				
			}		
			
			
		}
		
		void PPRR(){
			cout << "PPRR" << endl ;
			int time = 0 ;
			vector< Process > list ;
			list.assign( processlist.begin(), processlist.end() );
			sort( list.begin(),list.end(), check ) ;
			// priority_queue<Process, vector<Process>, checkPPRR > pqueue;
			vector< Process > pqueue ;
			int nowwork = -1 ;
			int remaindertime = 0 ;
			int runtime = 0 ;
			int arrtime ;
			int p ;
			for ( int i = 0 ; i < list.size() ; i++ ) {
				cout << getid(list.at(i).id) << "\t" ;
				cout << list.at(i).cpuburst << "\t" ;
				cout << list.at(i).arrivaltime << "\t" ;
				cout << list.at(i).priority << endl ;
			}
			
			while ( !pqueue.empty() || !list.empty() ) {
				//for ( int i = 0 ; i < pqueue.size() ; i++ )
				//  cout << getid(pqueue.at(i).id) << " " << pqueue.at(i).priority << ", " ;
				//cout << endl ;  
				for ( int i = 0 ; i < list.size() ; i++ ) {
					if( list.at(i).arrivaltime == time ) {
						//cout << "push :" << list.at(i).id << endl ;
						ins(pqueue, list.at(i) ) ;
						list.erase(list.begin() + i) ;
						i = -1 ;
					}
				}
				/*
				if( time == 29 ) {
					while(!pqueue.empty()) {
						cout << getid(pqueue.top().id) << " " << pqueue.top().priority << endl ;
						pqueue.pop() ; 
						
					}
					
				}
				*/
				
				if( runtime == timeslice && remaindertime != 0 && !pqueue.empty() && p == pqueue.at(0).priority ) {
					Process temp ;
					temp.id = nowwork ;
					temp.cpuburst = remaindertime ;
					temp.priority = p ;
					temp.arrivaltime = arrtime ;
					//cout << "push :" << temp.id << endl ;
					ins(pqueue, temp ) ;
					runtime = 0 ;
					temp = pqueue.at(0) ;
					pqueue.erase(pqueue.begin()) ;
					remaindertime = temp.cpuburst ;	
					nowwork = temp.id ;
					p = temp.priority ;
					arrtime = temp.arrivaltime ;
					//cout << endl << p << " " << nowwork << endl; 
				}	
				
				else if( !pqueue.empty() && nowwork != -1 && p > pqueue.at(0).priority ) {
					Process temp ;
					temp.id = nowwork ;
					temp.cpuburst = remaindertime ;
					temp.priority = p ;
					temp.arrivaltime = arrtime ;
					
					ins(pqueue, temp ) ;
					temp = pqueue.at(0) ;
					pqueue.erase(pqueue.begin()) ;
					remaindertime = temp.cpuburst ;	
					nowwork = temp.id ;
					p = temp.priority ;
					arrtime = temp.arrivaltime ;
				}
				
				
				if( remaindertime == 0 && !pqueue.empty() ) {
					Process temp = pqueue.at(0) ;
					pqueue.erase(pqueue.begin()) ;
					remaindertime = temp.cpuburst ;	
					nowwork = temp.id ;
					arrtime = temp.arrivaltime ;
					p = temp.priority ;
					runtime = 0 ;
				}
				else if( remaindertime == 0 && pqueue.empty() )
					nowwork = -1 ;
					
				if( nowwork != -1 )
					cout << getid(nowwork) ;
				else 
					cout << '-' ;	


				time++ ;
				runtime++ ;
				if( nowwork != -1 )
					remaindertime-- ;
					
					
			}

			while( remaindertime > 0 ) {
				if( nowwork != -1 )
					cout << getid(nowwork) ;
				else 
					cout << '-' ;	
				remaindertime-- ;
			}

		}
		
		
		
		
} ;

int main() {
	Scheduler scheduler = Scheduler() ;
	string filename ;
	int method ;
	int timeslice ;
	cout << "½Ð¿é¤JÀÉ¦W¡G" ;
	cin >> filename;

	string name = filename + ".txt" ;
	
	file.open( name.c_str(), ios::in ) ;
	if(file) {
		scheduler.readfile() ;
		scheduler.setfile() ;
		scheduler.makeschedule() ;
	}
	else {
		cout << "File open Error" ;
	}	
	

} // main()	

