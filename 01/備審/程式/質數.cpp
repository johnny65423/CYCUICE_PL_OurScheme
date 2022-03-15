#include<stdio.h> 
bool isprime ( int num1 ) {
	int a = 2 ;
	if (num1 == 1) return true ;
	while ( a < num1 ) {
		if ( num1 % a != 0 ) {
			a++;
		}
		else a = num1+ 1 ;
	}
	if ( a == num1 ) {
		return true ;
	}
	else return false ;
} //bool isprime ( int num1 )


int main(){
  int a,b ;
  scanf("%d",&a);
  if (isprime(a)){
  	printf("yes");
  }
  else printf("no") ;

}//main()
