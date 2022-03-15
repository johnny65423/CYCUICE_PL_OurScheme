#include<stdio.h>
#include<iostream>
#include<queue>
using namespace std ;

struct number{
	int id ;
	int p ;
};


struct checkPPRR{
	bool operator() (const number& x, const number& y){

			return x.p > y.p ;

	}
};

number getnumber( int id, int p ) {
	number temp ;
	temp.id = id ;
	temp.p = p ;
	
	return temp ;
	
}

int main(int argc, char** argv) {
	priority_queue<number, vector<number>, checkPPRR > pqueue;
	pqueue.push( getnumber( 4, 10 ) ) ;
	pqueue.push( getnumber( 8, 12 ) ) ;
	pqueue.push( getnumber( 10, 13 ) ) ;
	pqueue.push( getnumber( 5, 13 ) ) ;
	pqueue.push( getnumber( 13, 14 ) ) ;
	pqueue.push( getnumber( 7, 16 ) ) ;
	
	pqueue.push( getnumber( 2, 13  ) ) ;
	while(!pqueue.empty()) {
	  cout << pqueue.top().id << " " << pqueue.top().p << endl ;
	  pqueue.pop() ;
		
				
		
	}
}

