#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(int argc,char *argv[])
{
	int a = 0,len;
	if(argc!=2){printf("%d ARGUMENTS MISSING\n",2-argc);return EXIT_FAILURE;}
	len=strlen(argv[1]);
	for(a=0;a<len;a++){
		if((argv[1][a]>='A' && argv[1][a]<='M')||(argv[1][a]>='a' && argv[1][a]<='m')){
			argv[1][a]+=13;
		}else{
		if((argv[1][a]>='N' && argv[1][a]<='Z')||(argv[1][a]>='n' && argv[1][a]<='z')){
			argv[1][a]-=13;
		}
		}
	}
	printf("%s\n",argv[1]);
	return 0;

}
