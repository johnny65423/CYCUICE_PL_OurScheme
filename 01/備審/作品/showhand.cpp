
#include<string.h> 
#include<iostream>

using namespace std ;

enum Suit { SPADES = 832, HEARTS = 574, DIAMONDS = 456, CLUBS = 192 };
enum Face { ACE = 3947, DEUCE = 4537, THREE = 6696, 
            FOUR = 7729, FIVE = 8374, SIX = 9295, 
			SEVEN = 11491, EIGHT = 13638, NINE = 14554, 
			TEN = 15332, JACK = 18987, QUEEN = 20215, KING = 213643 };

struct Card {
	Suit suit;
	Face face;
};

typedef Card Hand[5] ;


bool InputHand( Hand hand ) ;

void PrintCard( Card card ) ;

bool BiggerThan( Card card1, Card card2 ) ;

void FindLargest( Hand hand, Card &largest ) ;

int Findlarge( Hand hand, Card & largest, Card &largesttp ) ;

bool IsFlush( Hand hand, Card &largest ) ;

bool IsStraightFlush( Hand hand, Card &largest ) ;

bool HasOnePair( Hand hand, bool excludeCard, Card excludedCard, Card &largest ) ;

bool IsFourOfAKind( Hand hand, Card &largest ) ;
     
bool HasThreeOfAKind( Hand hand, Card &largest ) ;

bool IsTwoPairs( Hand hand, Card &largest1, Card &largest2 ) ;

bool IsStraight( Hand hand, Card &largest ) ;

bool IsFullHouse( Hand hand, Card &largest ) ;

void Play( Hand hand1, Hand hand2 ) ;

void Printname( int i, int player ) ;

void Printfinal( int p1, int p2, bool win, Card largest1, Card largest2, Card largest1tp, Card largest2tp ) ;


int main() {
	Hand Hand1, Hand2 ;
	Card largest, largesttp ;
	int i ;
	
	if ( InputHand( Hand1 ) )
		if ( InputHand( Hand2 ) )
			Play( Hand1, Hand2 ) ;
			
		else cout << "input error hand2\n" ;
		
	else cout << "input error hand1\n" ;

	/*
	InputHand( Hand1 ) ;
	i = Findlarge( Hand1, largest, largesttp ) ;
	PrintCard( largest ) ;
	Printname( i, 1 ) ;	
	*/
} // main

bool InputHand( Hand hand ) { 
#define J = 11; 
#define Q = 12; 
#define K = 13; 
#define A = 1;
	int j = 0; 
	char test[5]; 
	char a ; 
	char b[4] ; 
	while ( j < 5 ) {
		cin >> a ; 
		if ( a == 'S' ) hand[j].suit = SPADES ; 
		else if ( a == 'H' ) hand[j].suit = HEARTS ; 
		else if ( a == 'D' ) hand[j].suit = DIAMONDS ; 
		else if ( a == 'C' ) hand[j].suit = CLUBS ; 
		cin >> b ; 
		if ( (strcmp( b, "A" ) == 0 ) ) hand[j].face = ACE ; 
		else if ( ( strcmp( b, "2" ) == 0 ) ) hand[j].face = DEUCE ; 
		else if ( ( strcmp( b, "3" ) == 0 ) ) hand[j].face = THREE ; 
		else if ( ( strcmp( b, "4" ) == 0 ) ) hand[j].face = FOUR ; 
		else if ( ( strcmp( b, "5" ) == 0 ) ) hand[j]. face = FIVE ; 
		else if ( ( strcmp( b, "6" ) == 0 ) ) hand[j].face = SIX ; 
		else if ( ( strcmp( b, "7" ) == 0 ) ) hand[j].face = SEVEN ; 
		else if ( ( strcmp( b, "8" ) == 0 ) ) hand[j].face = EIGHT ; 
		else if ( ( strcmp( b, "9" ) == 0 ) ) hand[j].face = NINE; 
		else if ( ( strcmp( b, "10" ) == 0 ) ) hand[j].face = TEN ; 
		else if ( ( strcmp( b, "J" ) == 0 ) ) hand[j].face = JACK ; 
		else if ( ( strcmp( b, "Q" ) == 0 ) ) hand[j].face = QUEEN ; 
		else if ( ( strcmp( b, "K" ) == 0 ) ) hand[j].face = KING ;

		j++;
	} // while
	
	cin >> test ;
	
	Card temp ;
	int i = 0; 
	j = 1; 
	
	while ( i < 4 ) { 
		while ( j < 5 ) { 
			if ( BiggerThan ( hand[i], hand[j] ) ) {
				temp = hand[i];
				hand[i] = hand[j] ;
				hand[j] = temp;

			} // if
		
			j++; 
		} // while
	
		i++; 
		j = i +1;
	} // while
	
	if ( strcmp( test, "-1" ) != 0 ) return false ; 
	else return true ;

} // InputHand()

void FindLargest( Hand hand, Card & largest ) { 
	int i = 0; 
	largest = hand[0] ;  
	int a = 0 ; 
	int b = 0 ; 
	int c = 0 ;
	int d = 0 ; 
	int e = 0 ; 
	int goal = 0; 
	while ( i < 5 ) {
		if ( hand[i].face == ACE) a = 1; 
		if ( hand[i].face == DEUCE ) b = 1 ; 
		if ( hand[i].face == THREE) c = 1; 
		if ( hand[i].face == FOUR ) d = 1 ; 
		if ( hand[i].face == FIVE ) {
			goal = i;
			e = 1; } // if
			i++; 
	} // while
	
	i = 1; 
	while (i< 5 ) { 
		if ( !BiggerThan( largest, hand[i] ) ) {
			largest. face = hand[i].face;
			largest. suit = hand[i].suit; 
		} // if
		
		i++; 
	} // while
	
	if ( a + b + c + d + e == 5 ) {
		largest = hand[goal] ; 
	} // if
	
} // FindLargest()

void PrintCard ( Card card ) { 
	if ( card.face == ACE ) cout << "Ace" ; 
	else if ( card.face == DEUCE ) cout << "Deuce" ; 
	else if ( card.face == THREE ) cout << "Three" ; 
	else if ( card.face == FOUR ) cout << "Four" ; 
	else if ( card.face == FIVE ) cout << "Five" ; 
	else if ( card.face == SIX ) cout << "Six" ; 
	else if ( card.face == SEVEN ) cout << "Seven" ; 
	else if ( card.face == EIGHT ) cout << "Eight" ; 
	else if ( card.face == NINE ) cout << "Nine" ; 
	else if ( card.face == TEN ) cout << "Ten" ; 
	else if ( card.face == JACK ) cout << "Jack" ; 
	else if ( card.face == QUEEN ) cout << "Queen" ; 
	else if ( card.face == KING ) cout << "King" ;
	
	cout << " of " ;
	
	if ( card.suit == SPADES ) cout << "Spades" ; 
	else if ( card.suit == HEARTS ) cout << "Hearts" ; 
	else if ( card.suit == DIAMONDS ) cout << "Diamonds" ; 
	else if ( card.suit == CLUBS ) cout << "Clubs" ;
} // PrintCard()

bool BiggerThan( Card card1, Card card2 ) {
	bool big = false;
	if ( card1. face == ACE && card2. face != ACE ) big = true; 
	else if ( card1. face != ACE && card2. face == ACE ) big = false ; 
	else if ( card1. face > card2. face ) {
		big = true; 
	} // if 
	else if ( card1. face < card2. face ) {
		big = false; } // if 
	else {
		if ( card1. suit > card2. suit ) big = true;
		else big = false ; 
	} // else
	
	if ( big ) return true ; 
	else return false ;
} // BiggerThan()

int Findlarge( Hand hand, Card & largest, Card & largesttp ) {
	bool excludeACard = false ; 
	Card excludedCard ; 
	
	if ( IsStraightFlush( hand, largest ) ) return 8 ; 
	else if ( IsFourOfAKind( hand, largest )) return 7; 
	else if ( IsFullHouse( hand, largest )) return 6 ; 
	else if ( IsFlush( hand, largest ) ) return 5;
	else if ( IsStraight( hand, largest ) ) {
		FindLargest( hand, largest );
		return 4 ; 
	} // else if 
	else if ( HasThreeOfAKind( hand, largest )) return 3; 
	else if ( IsTwoPairs( hand, largesttp, largest )) return 2 ; 
	else if ( HasOnePair( hand, excludeACard, excludedCard, largest )) return 1 ; 
	else {
		FindLargest( hand, largest ) ; 
		return 0;
	} // else 
} // Findlarge ()

bool IsFlush( Hand hand, Card & largest ) {
	int i = 0; int goal ; 
	bool check = true; 
	
	if ( hand [0]. suit == SPADES ) goal = hand[0].suit ; 
	if ( hand [0]. suit == HEARTS ) goal = hand[0] . suit ; 
	if ( hand [0]. suit == DIAMONDS ) goal = hand[0]. suit ; 
	if ( hand [0] . suit == CLUBS ) goal = hand [0] . suit ; 
	while ( i < 5 ) {
		if ( hand[i].suit != goal ) check = false;
		i++; 
	} // while
	if ( check == true ) { 
		FindLargest( hand, largest );
		return true ; 
	} // if 
	else {
		return false; 
	} // else 
} // IsFlush()

bool IsStraight( Hand hand, Card & largest ) {
	int a [5]; 
	int b; 
	int temp; 
	int i = 0; 
	while (i < 5 ) {
		if ( hand[i]. face == ACE ) a[i] = 14 ; 
		else if ( hand[i]. face == DEUCE ) a[i] = 2 ; 
		else if ( hand[i]. face == THREE) a[i] = 3 ; 
		else if (hand[i]. face == FOUR ) a[i] = 4 ; 
		else if ( hand[i]. face == FIVE ) a[i] = 5 ;
		else if ( hand[i]. face == SIX ) a[i] = 6 ; 
		else if ( hand[i]. face == SEVEN ) a[i] = 7 ;
		else if ( hand[i]. face == EIGHT ) a[i] = 8 ; 
		else if ( hand[i]. face == NINE ) a[i] = 9 ; 
		else if ( hand[i]. face == TEN ) a[i] = 10 ; 
		else if ( hand[i]. face == JACK ) a[i] = 11 ; 
		else if ( hand[i]. face == QUEEN ) a[i] = 12 ; 
		else if (hand[i]. face == KING ) a[i] = 13 ;
		
		i++; 
	} // while
	
	i = 0 ;
	

	int ace = 0 ; 
	int two = 0 ; 
	int three = 0, four = 0, five = 0; 
	i = 0; 
	while (i < 5 ) {
		if ( hand[i]. face == ACE ) ace = 1 ; 
		if ( hand[i].face == DEUCE ) two = 1 ; 
		if ( hand[i].face == THREE ) three = 1 ; 
		if ( hand[i]. face == FOUR ) four = 1 ; 
		if ( hand[i].face == FIVE ) five = 1 ; 		
		i++; 
	} // while

	FindLargest( hand, largest ) ;
	if ( a[0] + 1 == a[1] && a [1] + 1 == a[2] && a [2] + 1 == a[3] && a[3] + 1 == a[4] ) {
		return true; 
	} // if 
	else if ( ace == 1 && two == 1 && three == 1 && four == 1 && five == 1 ) return true ;  
	else return false;
	
} // IsStraight()

bool IsStraightFlush( Hand hand, Card & largest ) {  
	if ( IsFlush ( hand, largest ) && IsStraight( hand, largest ) ) {
		return true; 
	} // if 
	else {
		return false; 
	} // else
	
} // IsStraightFlush()

bool IsFourOfAKind( Hand hand, Card & largest ) { 
	int i = 0; 

	
	if ( hand[0].face == hand[2].face && hand[1].face == hand[3].face && 
		 hand[1].face == hand[2].face && hand[3].face != hand[4].face ) {
		largest = hand[3] ;
		return true ; 
	} // if 
	else if ( hand[0].face != hand[1].face && hand[1].face == hand[3].face &&
			  hand[2].face == hand[3].face && hand[2].face == hand[4].face ) {
		largest = hand[4] ;
		return true ; 
	} // else if 
	else return false;
} // IsFourOfAKind()

bool HasThreeOfAKind( Hand hand, Card & largest ) { 
	int i = 0; 

	if ( hand[0].face == hand[1].face && hand[1].face == hand[2].face ) { 
		if (hand[1].face != hand[3].face && hand[4].face != hand[1].face ) {
			largest = hand[2] ; 
			return true; 
		} // if
		else return false;
	} // if 
	else if (hand[0].face != hand[1].face && hand[1].face == hand[2].face) { 
		if ( hand[2].face == hand[3]. face && hand[3].face != hand[4].face) {
			largest = hand[3] ; 
			return true ; 
		} // if
		else return false; 
	} // else if 
	else if (hand[0].face != hand[3].face && hand[1].face != hand[3].face) { 
		if (hand[2].face == hand[3].face && hand[3].face == hand[4].face) {
			largest = hand[4] ; 
			return true; 
		} // if
		else return false; 
	} // else if 
	else return false;
	
} // HasThreeOfAKind()

bool HasOnePair( Hand hand, bool excludeACard, Card excludedCard, Card & largest ) {
	int first = 0, second = 0; 
	bool find = false; 
	bool find2 = false; 
	
	if (hand[0].face == hand[1]. face && hand[1]. face != hand[2].face) {
		find = true ; 
		if ( first == 0 ) first = 1; 
		else {
			second = 1;
			find2 = true; 
		} // else 
	} // if
	
	if ( hand[1].face == hand[2].face && hand[3].face != hand[2].face && hand[0].face != hand[1].face) {
		find = true; 
		if ( first == 0 ) first = 2; 
		else {
			second = 2;
			find2 = true ; 
		} // else 
	} // if
	
	if (hand[2].face == hand[3].face && hand[1].face != hand[2].face && hand[3].face != hand[4].face) {
		find = true; 
		if ( first == 0 ) first = 3; 
		else {
			second = 3;
			find2 = true; 
		} // else 
	} // if
	
	if ( hand[3].face == hand[4].face && hand[2].face != hand[3].face) {
		find = true ; 
		if ( first == 0 ) first = 4; 
		else {
			second = 4;
			find2 = true; 
		} // else 
	} // if

	if ( excludeACard == false && find == true) { 
		largest = hand[first] ; 
		return true; 
	} // if 
	else if ( excludeACard == true && find2 == true) {
		largest = hand[second] ; 
		return true; 
	} // else if 
	else return false;

} // HasOnePair()

bool IsTwoPairs ( Hand hand, Card & largest1, Card & largest2 ) {
	bool excludeACard = false; 
	Card excludedCard ; 
	bool first, second ;
	 
	first = HasOnePair( hand, excludeACard, excludedCard, largest1 ) ; 
	excludeACard = true;
	second = HasOnePair( hand, excludeACard, excludedCard, largest2 ) ; 
	if ( first && second ) return true; 
	else return false; 
} // IsTwo Pairs ()

bool IsFullHouse( Hand hand, Card & largest ) { 
	Card excludedCard ; 
	bool excludeACard = false ; 
	if ( HasOnePair( hand, excludeACard, excludedCard, largest ) == true ) {
		if ( HasThreeOfAKind( hand, largest ) == true){
			return true;
		} // if
		else return false; 
		
	} // if 
	else return false;
} // IsFullHouse()

void Play ( Hand hand1, Hand hand2 ) {
	Card largest1, largest2 ; 
	Card largest1tp, largest2tp ; 
	int p1, p2 ; 
	p1 = Findlarge ( hand1, largest1, largest1tp ) ; 
	p2 = Findlarge ( hand2, largest2, largest2tp ) ; 
	
	if ( p1 > p2 ) {
		Printfinal ( p1, p2, true, largest1, largest2, largest1tp, largest2tp ) ; 
	} // if 
	else if ( p2 > p1 ) {
		Printfinal( p1, p2, false, largest1, largest2, largest1tp, largest2tp ) ; 
	} // else if 
	else { 
		if ( BiggerThan( largest1, largest2 ) ) {
			Printfinal( p1, p2, true, largest1, largest2, largest1tp, largest2tp ) ; 
		} // if 
		else {
			Printfinal( p1, p2, false, largest1, largest2, largest1tp, largest2tp ) ; 
		} // else 
		
	} // else
	
} // Play()

void Printname( int i, int player ) {
	if ( i == 0 ) {
		if ( player == 1 ) cout << "Zilch" ;
		else cout << "zilch" ; 
	} // if
	if ( i == 1) {
		if ( player == 1 ) cout << "One pair" ;
		else cout <"one pair" ; 
	} // if
	if ( i == 2 ) {
		if ( player == 1 ) cout << "Two pairs" ;
		else cout << "two pairs" ; 
	} // if
	if ( i == 3 ) {
		if ( player == 1 ) cout << "Three of a kind" ;
		else cout << "three of a kind" ; 
	} // if
	if ( i == 4){
		if ( player == 1 ) cout << "Straight" ;
		else cout << "straight" ; 
	} // if
	if ( i == 5 ) {
		if ( player == 1 ) cout << "Flush" ;
		else cout << "flush" ; 
	} // if
	if ( i == 6 ) {
		if ( player == 1 ) cout << "Full house" ; 
		else cout << "full house" ; 
	} // if
	if ( i == 7 ) {
		if ( player == 1 ) cout << "Four of a kind" ;
		else cout << "four of a kind" ; 
	} // if
	if ( i == 8 ) {
		if ( player == 1 ) cout << "Straight flush" ;
		else cout << "straight flush" ; 
	} // if
} // Printname ()

void Printfinal( int pl, int p2, bool win, Card largest1, 
				 Card largest2, Card largest1tp, Card largest2tp ) { 

		Printname ( pl, 1 ) ; 
		cout << " ("; 
		PrintCard( largest1 ) ; 
		if ( pl == 2) {
			cout << " and " ;
			PrintCard( largest1tp ) ; 
		} // if
		
		if ( win ) cout << ") wins over " ; 
		else cout << ") loses to " ; 
		
		Printname( p2, 2 ); 
		cout << " ("; 
		PrintCard( largest2 ) ; 
		if ( p2 == 2 ) {
			cout << " and " ;
			PrintCard( largest2tp ); 
		} // if
		
		cout << ")." ; 
	
} // Printfinal()




