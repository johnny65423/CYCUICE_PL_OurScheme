# include <stdio.h>



bool isfactorprime ( int pf, int num1 ) {
	int a = 2 ;
	if  ( num1 == 0 ) {
		return false ;
	} // if  ( num1 == 0 )
	if (num1 % pf == 0 ) {
		while ( a < pf ) {
			if ( pf % a != 0 ) {
				a++;
			}
			else a = pf+ 1 ;
		}
		if ( a == pf ) {
			return true ;
		}
	else return false ;
	}
	else return false ;
	
} //bool ifprime ( int num1 )

int main(){
  int a,b ;
  scanf("%d",&a);
  scanf("%d",&b);
  if (isfactorprime(a,b)){
  	printf("yes");
  }
  else printf("no") ;

}//main()
