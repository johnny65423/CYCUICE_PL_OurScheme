# include <stdio.h>

int nthcharbhind (char & ch, int n) {
	ch = ch + n ;
	return ch ;
}
int main(){
  int n ;
  char ch ;
  scanf("%c",&ch);
  scanf("%d",&n);
  nthcharbhind ( ch, n ) ;
  printf( "%c", ch ) ;

}//main()
