# include <stdio.h>

void printstarline ( int n ) {
	int a = 0 ;
	int first = 1 ;
	while (a<n){
		if (first == 1) {
			printf ("*") ;
			first = 0 ;
		}
		else printf (" *") ;
		a +=1 ;
	}
}
void arrow (int n ) {
	int line = 1 ;
	int x = 0, y = 1 ;
	while (line <= n ) {
		printstarline (line) ;
		if (line <= n / 2 ){
			x = 1*(n-2*line) + (n - 2*line+1) ;
			while (y <= x) {
			printf (" ") ;
			y = y + 1 ;
			}
			printf ("*") ;
		}
		printf("\n") ;
		line = line + 1 ;
		y = 1 ;	
	}
}
int main(){
  int a ;
  scanf("%d",&a);
  arrow(a) ; 

}//main()
