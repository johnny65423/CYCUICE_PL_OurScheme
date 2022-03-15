#include<string.h>
#include<stdio.h>
#include<iostream>
using namespace std ;

struct IntNode {
	int id ;
	int count ;
	IntNode * next ;
};

typedef IntNode * IntPtr ;

void Write( IntPtr head ) {
	IntPtr temp = NULL ;
	temp = head ;
	
	while ( temp != NULL ) {
		cout << temp->id << "-" << temp->count ;
		cout << endl ;
		temp = temp->next ;
	}
	
}

void OrderInsert( IntPtr & head, int id ) {
	IntPtr temp = NULL;
	IntPtr insert = NULL;
	bool find = false ;
	temp = head ;

	while( temp != NULL && !find ) {
		
		if ( id == temp->id ) {
			find = true ;
			temp->count++ ;
		}
		else {
			temp = temp->next ;
		}
		
	}
	
	if ( !find ) {
		
		temp = head ;
		
		if( head == NULL ) {
			head = new IntNode ;
			head->id = id ;
			head->count = 1 ;
			head->next = NULL ;
			
		}	
		else {
			if ( head->id > id ) {
				insert = new IntNode ;
				insert->id = id ;
				insert->count = 1 ;
				insert->next = head ;
				head = insert ;
			}
			while ( temp != NULL ){
				if( temp->id < id ) {
					if( temp->next == NULL ) {
						temp->next = new IntNode ;
						temp = temp->next ; 
						temp->id = id ;
						temp->count = 1 ;
						temp->next = NULL ;
					}
					else if ( temp->next->id > id ) {
						insert = new IntNode ;
						insert->id = id ;
						insert->count = 1 ;
						insert->next = temp->next ;
						temp->next = insert ;
						
					}
					
				}
				
				
				temp = temp->next ;
			}
		
		}
		
	}
	
	
}

int main() {
	int id = -1 ;
	IntPtr intList = NULL ;
	IntPtr temp = NULL ;
	cin >> id ;
	while ( id != -1 ) {
		OrderInsert( intList, id ) ;
		cin >> id ;
		
	}
	
	Write( intList ) ;

	temp = intList ;
	temp->next = NULL ;
	if ( intList->next == NULL ) cout << 1 ;
}

