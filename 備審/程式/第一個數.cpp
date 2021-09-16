# include <stdio.h>

int lastdight ( int n ) {
	while (n > 10) {
		n = n / 10 ;
	}
	return n ;
} 

int main(){
  int a ;
  scanf("%d",&a);
  printf("%d", lastdight(a)) ; 

}//main()
