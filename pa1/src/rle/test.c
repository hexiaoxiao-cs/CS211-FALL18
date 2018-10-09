#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int main()
{
	char *a,*b;
	char *z;
	int c =0;
	a="asdkjfhaksjdh";
	b=(char *)malloc((strlen(a)+2)*sizeof(char));
	for(c=0;c<strlen(a);c++){
	b[c]=a[c];
	}
	z=(char*)malloc(2*sizeof(char));
	z[0]='a';
	b[c]='\0';
	printf("%d",(int)(strlen(z)));
	return 0;
}
