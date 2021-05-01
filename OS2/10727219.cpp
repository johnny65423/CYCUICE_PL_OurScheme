#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <vector> 
using namespace std ;

static fstream file ;

struct Process{
	int id ;
	int cpuburst ;
	int arrivaltime ;
	int priority ;
	
};

class Scheduler {
	public:
		void readfile(){
		  string info ;
			file >> method ;
			file >> timeslice ;
			cout << method << " " << timeslice << endl ;
			getline( file, info ) ;
			getline( file, info ) ;

			while( !file.eof() ) {
				readprocess() ;
			}	

			for ( int i = 0 ; i < processlist.size() ; i++ ) {
				cout << processlist.at(i).id << "\t" ;
				cout << processlist.at(i).cpuburst << "\t" ;
				cout << processlist.at(i).arrivaltime << "\t" ;
				cout << processlist.at(i).priority << endl ;
			}
				
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
} ;

int main() {
	Scheduler scheduler = Scheduler() ;
	string filename ;
	int method ;
	int timeslice ;
	cout << "½Ð¿é¤JÀÉ¦W¡G" ;
	cin >> filename;
	cout << filename ;
	string name = filename + ".txt" ;
	
	file.open( name.c_str(), ios::in ) ;
	if(file) {
	  cout << "open" ;
		scheduler.readfile() ;
	}
	else {
		cout << "File open Error" ;
	}	
	
	/*
	switch ( method ) {
		case 1 :
			FCFS() ;
		case 2 :
			RR();
		case 3 :
			SRTF();
		case 4 :
			PPRR();
		case 5 :
			HRRN();
		case 6 :
			allmethod() ;
		
	
	} // switch
	*/
} // main()	

