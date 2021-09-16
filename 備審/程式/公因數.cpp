#include<stdio.h> 
bool factorofboth ( int num, int a, int b ) {
	if (num == 0 || a == 0 || b == 0) {
		return false ;
	}
	else {
		if (a%num == 0 && b%num == 0){
			return true ;
		}
		else return false ;
	} 
	
} //bool isprime ( int num1 )


int main(){
  int n ;
  int x,y ;
  scanf("%d",&n);
  scanf("%d",&x);
  scanf("%d",&y);
  if (factorofboth(n, x, y)){
  	printf("yes");
  }
  else printf("no") ;

}//main()
