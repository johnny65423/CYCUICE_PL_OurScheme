#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <vector> 
#include <queue>
using namespace std ;

static fstream file ;
static fstream out ;
struct Process{
	int id ;
	int cpuburst ;
	int arrivaltime ;
	int priority ;
	int waiting ;
	float ratio ;
	
	int waittime[5] ;
	int turntime[5] ;
	
};

bool checkre(const Process& x, const Process& y){
		return x.id < y.id ;
}

bool check(const Process& x, const Process& y){
	if( x.arrivaltime != y.arrivaltime )
		return x.arrivaltime < y.arrivaltime ;
	else 
		return x.id < y.id ;
	
}

bool checkHRRN(const Process& x, const Process& y){
	if( x.ratio != y.ratio )
		return x.ratio > y.ratio ;
	else {
		if( x.arrivaltime != y.arrivaltime )
			return x.arrivaltime < y.arrivaltime ;
		else 
			return x.id < y.id ;
	}

	
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
			cout << method ;
			switch ( method ) {
				case 1 :
					out << "FCFS" << endl ;
					FCFS() ;
					break ;
				case 2 :
					out << "RR" << endl ;
					RR() ;
					break ;
				case 3 :
					out << "SRTF" << endl ;
					SRTF();
					break ;
				case 4 :
					out << "PPRR" << endl ;
					PPRR();
					break ;
				case 5 :
					out << "HRRN" << endl ;
					HRRN();
					break ;
				case 6 :
					out << "All" << endl ;
					FCFS() ;
					RR() ;
					SRTF() ;
					PPRR() ;
					HRRN() ;
					break ;
				
			
			} // switch
			cout << "done"<< endl ;
			sort(processlist.begin(),processlist.end(), checkre) ;
			
			for(int i = 0 ; i < 59 ; i++ )
				out << "=" ;
			out << endl << endl ;
			cout << endl ;
			
			out << "waiting" << endl ;
			switch ( method ) {
				case 1 :
					out << "ID	FCFS" << endl ;
					break ;
				case 2 :
					out << "ID	RR" << endl ;
					break ;
				case 3 :
					out << "ID	SRTF" << endl ;
					break ;
				case 4 :
					out << "ID	PPRR" << endl ;
					break ;
				case 5 :
					out << "ID	HRRN" << endl ;
					break ;
				case 6 :
					out << "ID	FCFS	RR	SRTF	PPRR	HRRN" << endl ;
					break ;
					
			} // switch
			for(int i = 0 ; i < 59 ; i++ )
				out << "=" ;
			out << endl ;
			switch ( method ) {
				case 1 :
					for ( int i = 0 ; i < processlist.size() ; i++ ) {
						out << processlist.at(i).id ;
						out << "\t" <<  processlist.at(i).waittime[0] ;
						out << endl ;
					}
					break ;
				case 2 :
					for ( int i = 0 ; i < processlist.size() ; i++ ) {
						out << processlist.at(i).id ;
						out << "\t" <<  processlist.at(i).waittime[1] ;
						out << endl ;
					}
					break ;
				case 3 :
					for ( int i = 0 ; i < processlist.size() ; i++ ) {
						out << processlist.at(i).id ;
						out << "\t" <<  processlist.at(i).waittime[2] ;
						out << endl ;
					}
					break ;
				case 4 :
					for ( int i = 0 ; i < processlist.size() ; i++ ) {
						out << processlist.at(i).id ;
						out << "\t" <<  processlist.at(i).waittime[3] ;
						out << endl ;
					}
					break ;
				case 5 :
					for ( int i = 0 ; i < processlist.size() ; i++ ) {
						out << processlist.at(i).id ;
						out << "\t" <<  processlist.at(i).waittime[4] ;
						out << endl ;
					}
					break ;
				case 6 :
					for ( int i = 0 ; i < processlist.size() ; i++ ) {
						out << processlist.at(i).id ;
						for( int j = 0 ; j < 5 ; j++ )
							out << "\t" <<  processlist.at(i).waittime[j] ;
						out << endl ;
					}
					break ;
					
			} // switch
			
			for(int i = 0 ; i < 59 ; i++ )
				out << "=" ;
			out << endl << endl ;
			cout << endl ;
			
			out << "Turnaround Time" << endl ;
			switch ( method ) {
				case 1 :
					out << "ID	FCFS" << endl ;
					break ;
				case 2 :
					out << "ID	RR" << endl ;
					break ;
				case 3 :
					out << "ID	SRTF" << endl ;
					break ;
				case 4 :
					out << "ID	PPRR" << endl ;
					break ;
				case 5 :
					out << "ID	HRRN" << endl ;
					break ;
				case 6 :
					out << "ID	FCFS	RR	SRTF	PPRR	HRRN" << endl ;
					break ;
					
			} // switch
			for(int i = 0 ; i < 59 ; i++ )
				out << "=" ;
			out << endl ;
			switch ( method ) {
				case 1 :
					for ( int i = 0 ; i < processlist.size() ; i++ ) {
						out << processlist.at(i).id ;
						out << "\t" <<  processlist.at(i).turntime[0] ;
						out << endl ;
					}
					break ;
				case 2 :
					for ( int i = 0 ; i < processlist.size() ; i++ ) {
						out << processlist.at(i).id ;
						out << "\t" <<  processlist.at(i).turntime[1] ;
						out << endl ;
					}
					break ;
				case 3 :
					for ( int i = 0 ; i < processlist.size() ; i++ ) {
						out << processlist.at(i).id ;
						out << "\t" <<  processlist.at(i).turntime[2] ;
						out << endl ;
					}
					break ;
				case 4 :
					for ( int i = 0 ; i < processlist.size() ; i++ ) {
						out << processlist.at(i).id ;
						out << "\t" <<  processlist.at(i).turntime[3] ;
						out << endl ;
					}
					break ;
				case 5 :
					for ( int i = 0 ; i < processlist.size() ; i++ ) {
						out << processlist.at(i).id ;
						out << "\t" <<  processlist.at(i).turntime[4] ;
						out << endl ;
					}
					break ;
				case 6 :
					for ( int i = 0 ; i < processlist.size() ; i++ ) {
						out << processlist.at(i).id ;
						for( int j = 0 ; j < 5 ; j++ )
							out << "\t" <<  processlist.at(i).turntime[j] ;
						out << endl ;
					}
					break ;
					
			} // switch
			if( method == 6 ) {
				for(int i = 0 ; i < 59 ; i++ )
					out << "=" ;
				out << endl << endl ;
			}
			
		}

	private:
		vector<Process> processlist ;
		int method ;
		int timeslice ;
		
		void readprocess() {
			Process temp ;
			
			file >> temp.id >> temp.cpuburst >> temp.arrivaltime >> temp.priority ;
			temp.ratio = 0 ;
			temp.waiting = 0 ;
			if (!file.eof())
				processlist.push_back( temp ) ;
			
		}	

		void FCFS(){
			
			cout << "FCFS" << endl ;
			if( method == 6 )
				out << "==        FCFS==" << endl ;
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
					insertresult( nowwork, time, 0 ) ;
					Process temp = pqueue.front() ;
					pqueue.pop() ;
					remaindertime = temp.cpuburst ;	
					nowwork = temp.id ;
				}
				else if( remaindertime == 0 && pqueue.empty() ) {
					insertresult( nowwork, time, 0 ) ;
					nowwork = -1 ;
				}
					
					
				if( nowwork != -1 ) {
					cout << getid(nowwork) ;
					out << getid(nowwork) ;
					
				}
				else {
					cout << '-' ;
					out << '-' ;
				}
				time++ ;
				if( nowwork != -1 )
					remaindertime-- ;
			}

			while( remaindertime > 0 ) {
				if( nowwork != -1 ) {
					cout << getid(nowwork) ;
					out << getid(nowwork) ;
					
				}
				else {
					cout << '-' ;
					out << '-' ;
				}	
				time++ ;
				remaindertime-- ;
			}
			insertresult( nowwork, time, 0 ) ;
			out << endl ;
			cout << endl ;
		}
		
		void RR(){
			cout << "RR" << endl ;
			if( method == 6 )
				out << "==          RR==" << endl ;
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
					insertresult( nowwork, time, 1 ) ;
					Process temp = pqueue.front() ;
					pqueue.pop() ;
					remaindertime = temp.cpuburst ;	
					nowwork = temp.id ;
					runtime = 0 ;
				}
				else if( remaindertime == 0 && pqueue.empty() ){
					insertresult( nowwork, time, 1 ) ;
					nowwork = -1 ;
				}
					
					
				if( nowwork != -1 ) {
					cout << getid(nowwork) ;
					out << getid(nowwork) ;
					
				}
				else {
					cout << '-' ;
					out << '-' ;
				}	

				time++ ;
				runtime++ ;
				if( nowwork != -1 )
					remaindertime-- ;
			}

			while( remaindertime > 0 ) {
				if( nowwork != -1 ) {
					cout << getid(nowwork) ;
					out << getid(nowwork) ;
					
				}
				else {
					cout << '-' ;
					out << '-' ;
				}
				time++ ;				
				remaindertime-- ;
			}
			insertresult( nowwork, time, 1 ) ;
			
			out << endl ;
			cout << endl ;
		}
		
		void SRTF(){
			cout << "SRTF" << endl ;
			if( method == 6 )
				out << "==        SRTF==" << endl ;
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
					insertresult( nowwork, time, 2 ) ;
					Process temp = pqueue.top() ;
					pqueue.pop() ;
					remaindertime = temp.cpuburst ;	
					arrtime = temp.arrivaltime ;
					nowwork = temp.id ;
				}
				else if( remaindertime == 0 && pqueue.empty() ){
					insertresult( nowwork, time, 2 ) ;
					nowwork = -1 ;
				}
					
				if( nowwork != -1 ) {
					cout << getid(nowwork) ;
					out << getid(nowwork) ;
					
				}
				else {
					cout << '-' ;
					out << '-' ;
				}		

				time++ ;
				if( nowwork != -1 )
					remaindertime-- ;
			}

			while( remaindertime > 0 ) {
				if( nowwork != -1 ) {
					cout << getid(nowwork) ;
					out << getid(nowwork) ;
					
				}
				else {
					cout << '-' ;
					out << '-' ;
				}		
				remaindertime-- ;
				time++ ;
			}
			insertresult( nowwork, time, 2 ) ;
			out << endl ;
			cout << endl ;
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
			if( method == 6 )
				out << "==        PPRR==" << endl ;
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

			
			while ( !pqueue.empty() || !list.empty() ) {
 
				for ( int i = 0 ; i < list.size() ; i++ ) {
					if( list.at(i).arrivaltime == time ) {
						//cout << "push :" << list.at(i).id << endl ;
						ins(pqueue, list.at(i) ) ;
						list.erase(list.begin() + i) ;
						i = -1 ;
					}
				}

				
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
					insertresult( nowwork, time, 3 ) ;
					Process temp = pqueue.at(0) ;
					pqueue.erase(pqueue.begin()) ;
					remaindertime = temp.cpuburst ;	
					nowwork = temp.id ;
					arrtime = temp.arrivaltime ;
					p = temp.priority ;
					runtime = 0 ;
				}
				else if( remaindertime == 0 && pqueue.empty() ) {
					insertresult( nowwork, time, 3 ) ;
					nowwork = -1 ;
				}
				if( nowwork != -1 ) {
					cout << getid(nowwork) ;
					out << getid(nowwork) ;
					
				}
				else {
					cout << '-' ;
					out << '-' ;
				}	


				time++ ;
				runtime++ ;
				if( nowwork != -1 )
					remaindertime-- ;
					
					
			}

			while( remaindertime > 0 ) {
				if( nowwork != -1 ) {
					cout << getid(nowwork) ;
					out << getid(nowwork) ;
					
				}
				else {
					cout << '-' ;
					out << '-' ;
				}		
				time++ ;
				remaindertime-- ;
			}
			
			insertresult( nowwork, time, 3 ) ;
			out << endl ;
			cout << endl ;
		}
		
		void HRRN(){
			cout << "HRRN" << endl ;
			if( method == 6 )
				out << "==        HRRN==" << endl ;
			int time = 0 ;
			vector< Process > list ;
			list.assign( processlist.begin(), processlist.end() );
			sort( list.begin(),list.end(), check ) ;
			vector<Process> pqueue ;
			int nowwork = -1 ;
			int remaindertime = 0 ;
			while ( !pqueue.empty() || !list.empty() ) {
				for ( int i = 0 ; i < list.size() ; i++ ) {
					if( list.at(i).arrivaltime == time ) {
						pqueue.push_back( list.at(i) ) ;
						list.erase(list.begin() + i) ;
						i = -1 ;
					}
				}
				
				for( int i = 0 ; i < pqueue.size() ; i++ ) {
					pqueue.at(i).ratio = (float) ( pqueue.at(i).waiting + pqueue.at(i).cpuburst ) / pqueue.at(i).cpuburst ;
					//cout << "*" << pqueue.at(i).ratio << " " << getid(pqueue.at(i).id) << endl ;
				} 	
				sort( pqueue.begin(),pqueue.end(), checkHRRN ) ;
					
				
				if( remaindertime == 0 && !pqueue.empty() ) {
					insertresult( nowwork, time, 4 ) ;
					Process temp = pqueue.at(0) ;
					pqueue.erase( pqueue.begin() ) ;
					remaindertime = temp.cpuburst ;	
					nowwork = temp.id ;
				}
				else if( remaindertime == 0 && pqueue.empty() ) {
					insertresult( nowwork, time, 4 ) ;
					nowwork = -1 ;
				}
				
				if( nowwork != -1 ) {
					cout << getid(nowwork) ;
					out << getid(nowwork) ;
					
				}
				else {
					cout << '-' ;
					out << '-' ;
				}		

				time++ ;
				for( int i = 0 ; i < pqueue.size() ; i++ ) {
					pqueue.at(i).waiting += 1 ;
					
				} 
				if( nowwork != -1 )
					remaindertime-- ;
			}

			while( remaindertime > 0 ) {
				if( nowwork != -1 ) {
					cout << getid(nowwork) ;
					out << getid(nowwork) ;
					
				}
				else {
					cout << '-' ;
					out << '-' ;
				}		
				time++ ;
				remaindertime-- ;
			}
			insertresult( nowwork, time, 4 ) ;
			out << endl ;
			cout << endl ;
		}
		
		void insertresult( int id, int time, int mode ) {
			for( int i = 0 ; i < processlist.size() ; i++ ) {
				if( processlist.at(i).id == id ){
					processlist.at(i).waittime[mode] = time - processlist.at(i).arrivaltime - processlist.at(i).cpuburst ;
					processlist.at(i).turntime[mode] = time - processlist.at(i).arrivaltime ;
				}
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
		name = "out_" + name ;
		out.open( name.c_str(), ios::out ) ;
		//out << "A11" << endl ;
		
		scheduler.readfile() ;
		scheduler.setfile() ;
		scheduler.makeschedule() ;
	}
	else {
		cout << "File open Error" ;
	}	
	

} // main()	

