# include <stdio.h>

int secondlastdight ( int n ) {
	if (n < 10 ) return -1 ;
	n = n / 10 ; 
	n = n % 10 ;
	return n ;
} 

int main(){
  int a ;
  scanf("%d",&a);
  printf("%d", secondlastdight(a)) ; 

}//main()
