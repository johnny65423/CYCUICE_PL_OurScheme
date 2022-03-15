# include <stdio.h>

void listprimefactors (int n){
	int b = 2, a = 2 ;
	int count = 1 ;
	while (b < n ) {
		if (n % b == 0 ) {
			while ( a < b ) {
				if ( b % a != 0 ) {
					a++;
				}
				else a = b+ 1 ;
			}
			if ( a == b ) {
				if(count == 1 ) {
					printf ("%d", b ) ;
					count =-1 ; 
				}
				else {
					printf ("_%d", b ) ;
				}
				a = 2 ;
			}
			else a = 2 ;
		}		
		b += 1 ;
	}
	
}
int main(){
  int a,b ;
  scanf("%d",&a);
  listprimefactors(a) ;

}//main()
