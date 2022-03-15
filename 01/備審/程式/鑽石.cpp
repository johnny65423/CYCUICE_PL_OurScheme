# include <stdio.h>
void printdiamond (char ch, int n ) {
	int line = 0, numofline ;
	int before , a = 0 ;
	int after , b = 0 ;
	numofline = n + 2 ;
	while (line < numofline ) {
		before = (n+1)/2 - line ;
		if (before < 0 ) before = before * -1 ;
		while (a< before) {
			printf(" ") ;
			a+=1 ;
		}
		printf("%c", ch ) ;
		after = n - 2*before ;
		while (b< after) {
			printf(" ") ;
			b+=1 ;
		}
		if (line != 0 && line != n + 1 ) {
			printf("%c", ch ) ;
		}
		line +=1;
		a = 0 ;
		b = 0 ;
		printf("\n") ;	
	}
}
int main(){
  int a ;
  char ch ;
  scanf("%c",&ch);
  scanf("%d",&a);
  printdiamond(ch, a) ; 
}//main()
