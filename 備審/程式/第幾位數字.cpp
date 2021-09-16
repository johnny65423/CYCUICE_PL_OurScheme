# include <stdio.h>

int main(){
	int n = 0 ;
	int a = 0 ;
	int num = 1 ;
	scanf( "%d", &n );
	scanf( "%d", &a );
	while( a > 1 ){
		num = num * 10 ;
		a = a - 1 ;
	}
	if (num > n ) {
		printf( "none" ) ;
	}
	else {
		n = n / num ;
		n = n % 10 ;
		printf( "%d",n ) ;
	}
}//main()
