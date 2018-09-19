#include<stdio.h>
#include<stdlib.h>
#define MAXTESTCASES 100
int gcd(int a,int b){
	if(b==0){return a;}
	return gcd(b,a%b);
}
int main()
{
	int a=0,b=0,ct=0;
	freopen("./tests/tests.txt","w",stdout);
	for(ct=0;ct<MAXTESTCASES;ct++)
	{
	a=rand()%100;
	b=rand()%100;
	
	printf("%d %d\n",a,b);
	printf("%d\n",gcd(a,b));
	
	}
	return 0;

}
