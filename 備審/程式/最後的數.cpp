# include <stdio.h>

int lastdight ( int n ) {
	n = n % 10 ;
	return n ;
} 

int main(){
  int a ;
  scanf("%d",&a);
  printf("%d", lastdight(a)) ; 

}//main()
