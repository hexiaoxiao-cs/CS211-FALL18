#include<stdio.h>
#include<stdlib.h>
#define MAXTESTCASES 100
int gcd(int a,int b){
	if(b==0){return a;}
	return gcd(b,a%b);
}
void rle()
{
	int MAXLENGTH=100;
	int a=0;
	int b[85];
	int c=0,target;
	int temp=0;
	int stuff=0;
	int str[100];
	int place=0;
	int occurance[100];
	for(a=33;a<=47;a++)
	{
		b[c]=a;
		c++;
	}
	for(a=58;a<=126;a++)
	{
		b[c]=a;
		c++;
	}
	for(a=0;a<MAXTESTCASES;a++)
	{
		c=0;
		place=0;
		while(c<MAXLENGTH)
		{
				
			target=rand()%MAXLENGTH;
			stuff=rand()%84;
			for(temp=0;temp<target;temp++)
			{
				printf("%c",b[stuff]);
			}
			str[place]=stuff;
			occurance[place]=target;
			place++;
			c+=target;
		}
		printf("\n");
		if(place*2<c){
		for(c=0;c<place;c++)
			{
			printf("%c%d",b[str[c]],occurance[c]);
			}}
		else{}
		printf("\n");
		
		}
	
	
	return;
}
int main()
{
	
	freopen("./tests/tests.txt","w",stdout);
	/*for(ct=0;ct<MAXTESTCASES;ct++)
	{
	a=rand()%100;
	b=rand()%100;
	
	printf("%d %d\n",a,b);
	printf("%d\n",gcd(a,b));
	
	}*/
	rle();
	return 0;

}
