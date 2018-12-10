#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
int DEBUG=1;
int log2base(int x)
{
	int tmp=0,tmp2=x;
	unsigned nbit=0
	while(tmp2==1){
		tmp2=tmp2>>1;
		tmp++;
	}
	nbit=(1<<tmp)&x;
	if(nbit==0){
	return tmp;}
	else{return -1;}
}

void print(int mr,int mw,int ch,int cm)
{
	return;
}
void process(long int *tag, long int *valid,int sets, int bs,)
{
	char tmp[1000];
	long int addr=0;
	int memr=0,memw=0,ch=0,cm=0;
	while(1)
	{
		scanf(" %s ",tmp);
		if(strcmp(tmp,"#eof")==0){print(memr,memw,ch,cm);return;}
		scanf(" %c %x ",tmp,addr);



}

int main(int argc, char *argv[])
{
	int cache_size=0,prefetch=0,replacement=0,block_size=0,assoc=0;//parameters by reading the string
	int sets=0;//parameters by calculating
	long int *tag,*valid;
	char tmp[1000];
	long int addr=0;
	int memr=0,memw=0,ch=0,cm=0;
	int setb=0,blsb=0,tmp1=0,tmp2=0,tmp3=0;
	int miku=0,match=0;
	//reading argument
	if(argc!=7){printf("Invalid arguments\n");}
	cache_size=atoi(argv[1]);
	block_size=atoi(argv[5]);
	if(strcmp(argv[4],"lru")==0){replacement=1;}
	if(strcmp(argv[3],"p1")==0){prefetch=1;}
	if(strcmp(argv[2],"direct")==0){assoc=1;}
	else{
		if(strcmp(argv[2],"assoc")==0){assoc=(int)cache_size/block_size;}
		else{assoc=argv[2][6]-'0';if(log2base(assoc)==-1){printf("CRITICAL ERROR: ASSOC NOT POW OF 2!\n");return EXIT_SUCCESS;}
	}
	if(log2base(cache_size)==-1 || log2base(block_size)==-1){printf("CRITICAL ERROR: PARAMETER NOT CORRECT!\n");return EXIT_SUCCESS:}
	sets=(int)cache_size/(block_size*assoc);
	
	freopen(argv[6],'r',stdin);
	//prefetch 0 no 1 one above
	//replacement 0 FIFO 1 lru
	//assoc 1 direct other 
	tag=(long int*) malloc(assoc*sets*sizeof(long int));
	memset(tag,0,assoc*sets*sizeof(long int));
	valid=(long int*)malloc(assoc*sets*sizeof(long int));
	memset(valid,0,assoc*sets*sizeof(long int));
	setb=log2base(sets);
	blsb=log2base(block_size);
	
	//Start procesing the file
	while(1)
	{
		scanf(" %s ",tmp);
		if(strcmp(tmp,"#eof")==0){//print sth
		}
		scanf(" %c %x",tmp,addr);
		addr=addr>>blsb;
		tmp3=addr>>setb;
		tmp2=addr-tmp3*(1<<setb);
		if(DEBUG==1){printf("tmp1: %x, tmp2: %x, tmp3 %x\n",tmp1,tmp2,tmp3);}
		match=0;
		for(miku=0;miku<assoc;miku++)
		{
			if(tag[tmp
		}
		if(strcmp(tmp,"W")==0){//do writing
			
		}
		if(strcmp(tmp,"R")==0){//do reading
			
		}

	}

	return EXIT_SUCCESS;
}
