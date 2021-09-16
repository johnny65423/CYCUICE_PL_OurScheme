# include <stdio.h>
void printstarline ( int n ) {
	int a = 0 ;
	int first = 1 ;
	while (a<n){
		if (first == 1) {
			printf ("*") ;
			first = 0 ;
		}
		else printf ("_*") ;
		a +=1 ;
	}
}
int main(){
	int n ;
	scanf("%d", &n);
	printstarline ( n ) ;
}//main()
