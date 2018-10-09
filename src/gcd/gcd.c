#include <stdio.h>
#include <stdlib.h>
int gcd(int x,int y)
{

	if(x==0){return y;}
	return gcd(y%x,x); 
}

int main(int argc, char *argv[])
{
	int number1,number2,result;
	if(argc!=3){printf("%d ARGUMENT(S) MISSING",3-argc);return EXIT_FAILURE;}
	number1=atoi(argv[1]);
	number2=atoi(argv[2]);
	/*printf("%d %d\n",number1,number2);*/
	result=gcd(number1,number2);
	printf("%d\n",result);
	return 0;
}
