# include <stdio.h>

int nthlastdight ( int n, int nth ) {
	int a = 1 ;
	while (nth > 1 ) {
		a = a * 10 ;
		nth = nth - 1 ;
	}
	if (n < a ) return -1 ;
	n = n / a ; 
	n = n % 10 ;
	return n ;
} 

int main(){
  int a, b ;
  scanf("%d",&a);
  scanf("%d",&b);
  printf("%d", nthlastdight(a,b)) ; 

}//main()
