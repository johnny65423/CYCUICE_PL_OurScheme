# include <stdio.h>

int numofdightin ( char ch, int n ) {
	int a = 0,x;
	while (n > 10) {
		x = n % 10 ;
		if ( x + '0' == ch ) {
			a = a + 1 ;
		}
		n = n / 10 ;
	}
	if (x + '0' == ch ) {
		a = a + 1 ;
	}
	return a ;
} 

int main(){
  char ch ;
  int a ;
  scanf("%c",&ch);
  scanf("%d",&a);
  printf("%d", numofdightin(ch, a)) ; 
}//main()
