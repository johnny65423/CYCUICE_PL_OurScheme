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
void RightTrangle (int n ) {
	int line = 0 ;
	while (line <= n ) {
		printstarline (line) ;
		printf("\n") ;
		line = line + 1 ;	
	}
}
int main(){
  int a ;
  scanf("%d",&a);
  RightTrangle(a) ; 

}//main()
