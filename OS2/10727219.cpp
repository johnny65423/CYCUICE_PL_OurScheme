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
			sort(processlist.begin(),processlist.end(), check) ;
			
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
				case 6 :
					FCFS() ;	
				/*case 2 :
					RR();
				case 3 :
					SRTF();
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
			queue<Process> pqueue ;
			int nowwork = -1 ;
			int remaindertime = 0 ;
			while ( !pqueue.empty() || !processlist.empty() ) {
				for ( int i = 0 ; i < processlist.size() ; i++ ) {
					if( processlist.at(i).arrivaltime == time ) {
						pqueue.push( processlist.at(i) ) ;
						// cout << "push: " << processlist.at(i).id << endl ;
						processlist.erase(processlist.begin() + i) ;
						i = -1 ;
					}
				}
				
				if( remaindertime == 0 && !pqueue.empty() ) {
					Process temp = pqueue.front() ;
					pqueue.pop() ;
					remaindertime = temp.cpuburst ;	
					
					nowwork = temp.id ;
				}
				else if( remaindertime == 0 && pqueue.empty() ) {
					nowwork = -1 ;
				}
					

				// cout << remaindertime ;
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
} ;

int main() {
	Scheduler scheduler = Scheduler() ;
	string filename ;
	int method ;
	int timeslice ;
	cout << "½Ð¿é¤JÀÉ¦W¡G" ;
	cin >> filename;
	// cout << filename ;
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

