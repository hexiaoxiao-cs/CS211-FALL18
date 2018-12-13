#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
int DEBUG=0;
int log2base(int x)
{
	int tmp=0,tmp2=x;
	unsigned nbit=0;
	while(tmp2!=0){
		tmp2=tmp2>>1;
		tmp++;
	}
	nbit=(1<<tmp)&x;
	if(nbit==0){
	return tmp-1;}
	else{return -1;}
}

void print(int mr,int mw,int ch,int cm)
{
	printf("Memory reads: %d\n",mr);
	printf("Memory writes: %d\n",mw);
	printf("Cache hits: %d\n",ch);
	printf("Cache misses: %d\n",cm);
	return;
}
int checkpriority(int **priority,int sets,int assoc)
{
	//return least priority or the entry with zero
	int tmp1=0,min=0,zerofl=0,max=0,mini=100000000;
	for(tmp1=0;tmp1<assoc;tmp1++)
	{
		if(priority[sets][tmp1]==0&&zerofl!=1){min=tmp1;zerofl=1;}
		if(priority[sets][tmp1]<mini&&zerofl!=1){min=tmp1;mini=priority[sets][tmp1];}
		if(priority[sets][tmp1]>max){max=priority[sets][tmp1];}
	}
	priority[sets][min]=max+1;
	if(zerofl==0){
		for(tmp1=0;tmp1<assoc;tmp1++)
		{
			priority[sets][tmp1]--;
		}
	}
	return min;
}
void accesslru(int **priority,int sets,int assoc,int line)
{
	int tmp=0,max=0;
	for(tmp=0;tmp<assoc;tmp++)
	{
		if(priority[sets][tmp]>max){max=priority[sets][tmp];}
	}
	priority[sets][line]=max+1;
	return; 
}
int main(int argc, char *argv[])
{
	int cache_size=0,prefetch=0,replacement=0,block_size=0,assoc=0;//parameters by reading the string
	int sets=0;//parameters by calculating
	unsigned long int **tag,**valid;
	char tmp[1000];
	int **priority;
	unsigned long int addr=0;
	int memr=0,memw=0,ch=0,cm=0;
	int setb=0,blsb=0;
	unsigned long tmp1=0,tmp2=0,tmp3=0,tt=0;
	int miku=0,match=0,match_prefetch=0,wline=0;
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
	}
	if(log2base(cache_size)==-1 || log2base(block_size)==-1){printf("CRITICAL ERROR: PARAMETER NOT CORRECT!\n");return EXIT_SUCCESS;}
	cache_size=cache_size;
	block_size=block_size;
	sets=(int)cache_size/(block_size*assoc);
	
	freopen(argv[6],"r",stdin);
	//prefetch 0 no 1 one above
	//replacement 0 FIFO 1 lru
	//assoc 1 direct other 
	tag=(unsigned long int**) malloc(sets*sizeof(unsigned long int*));
	for(tmp1=0;tmp1<sets;tmp1++)
	{
		tag[tmp1]=(unsigned long int *) malloc(assoc*sizeof(unsigned long int));
		memset(tag[tmp1],0,assoc*sizeof(unsigned long int));
	}
	
	
	valid=(unsigned long int**)malloc(sets*sizeof(unsigned long int*));
	for(tmp1=0;tmp1<sets;tmp1++)
	{
		valid[tmp1]=(unsigned long int *)malloc(assoc*sizeof(unsigned long int));
		memset(valid[tmp1],0,assoc*sizeof(unsigned long int));
	}
	
	setb=log2base(sets);
	blsb=log2base(block_size);
	if(DEBUG==1){
		printf("There are %d sets, %d lines, %d block size\n",sets,assoc,block_size);
		printf("prefetch: %d, assoc: %d replacement: %d\n",prefetch,assoc,replacement);
		printf("Bit Shifter Argument: %d set shift, %d block shift",setb,blsb);
		}
	if(replacement==0){

		priority=(int **)malloc(sizeof(int*));
		priority[0]=(int *)calloc(sets,sizeof(int));
		
	}
	else{
		priority=(int **)malloc(sets*sizeof(int*));
		for(tmp1=0;tmp1<sets;tmp1++)
		{
			priority[tmp1]=(int *)calloc(assoc,sizeof(int));
			
		}
	

	}

	//Start procesing the file
	while(1)
	{
		scanf(" %s ",tmp);
		if(DEBUG==1){
		printf("%s\n",tmp);}
		if(strcmp(tmp,"#eof")==0){//print sth
			print(memr,memw,ch,cm);
			break;
		}
		scanf(" %s %lx",tmp,&addr);
		if(DEBUG==1){printf("%s %lx\n",tmp,addr);}
		tmp1=addr-(addr>>blsb)*(1<<blsb);
		addr=addr>>blsb;//delete block info

		tmp3=addr>>setb;//tag info
		tmp2=addr-tmp3*(1<<setb);//set info
		if(DEBUG==1){printf("Block Index: %lx, Set Index: %lx, Tag: %lx\n",tmp1,tmp2,tmp3);}
		match=0;
		for(miku=0;miku<assoc;miku++)
		{
			if(tag[tmp2][miku]==tmp3&&valid[tmp2][miku]==1){match=1;wline=miku;break;}
		}
		if(match==0){
			if(DEBUG==1){printf("CACHE MISS!!\n");
				if(replacement==0){
					printf("Curr Priority: %d, Curr valid: %ld, Curr tag: %lx\n",priority[0][tmp2],valid[tmp2][priority[0][tmp2]],tag[tmp2][priority[0][tmp2]]);
				}

			}
			if(replacement==0){
				match_prefetch=0;
				tag[tmp2][priority[0][tmp2]]=tmp3;
				valid[tmp2][priority[0][tmp2]]=1;
				if(priority[0][tmp2]==assoc-1){priority[0][tmp2]=0;}
				else{priority[0][tmp2]++;}
				if(prefetch==1){
					//if(((tmp1+block_size)>>blsb)>0){tmp2++;}
					if(DEBUG==1){
						printf("Entering Prefetching\n");
					}
					tmp2++;
					if((tmp2>>setb)>0){tmp2=0;
						tmp3++;
						if(DEBUG==1){
						printf("Tag Number Before Check %lx\n",tmp3);}
						
					if(DEBUG==1){printf("To Check : %lx\n",(tmp3>>(48-setb-blsb)));
					printf("Shifting %d to left\n",48-setb-blsb);}
					if((tmp3>>(48-setb-blsb))>1){tmp3=0;
					}
					}
					
					for(miku=0;miku<assoc;miku++)
					{
						if(tag[tmp2][miku]==tmp3&&valid[tmp2][miku]==1){match_prefetch=1;break;}
					}
					if(DEBUG==1){
						printf("Prefetching Set Number: %lx, Tag Number %lx, Target? %d\n",tmp2,tmp3,match_prefetch);
					}

					if(match_prefetch==0){
					tag[tmp2][priority[0][tmp2]]=tmp3;
					valid[tmp2][priority[0][tmp2]]=1;
					if(priority[0][tmp2]==assoc-1){priority[0][tmp2]=0;}
					else{priority[0][tmp2]++;}
					memr++;
					}
				}
			}
			if(replacement==1){//LRU
				match_prefetch=0;
				tt=checkpriority(priority,tmp2,assoc);//The Place For inserting
				if(DEBUG==1){
					printf("Returned Place For inserting: %lu\n",tt);
					printf("Curr Priority: %d, Curr valid: %ld, Curr tag: %lx\n",priority[tmp2][tt],valid[tmp2][tt],tag[tmp2][tt]);
				}
				tag[tmp2][tt]=tmp3;
				valid[tmp2][tt]=1;
				if(prefetch==1){
					if(DEBUG==1){
						printf("Entering Prefetching\n");
					}
					tmp2++;
					if((tmp2>>setb)>0){tmp2=0;
						tmp3++;
						if(DEBUG==1){
						printf("Tag Number Before Check %lx\n",tmp3);}
						
					if(DEBUG==1){printf("To Check : %lx\n",(tmp3>>(48-setb-blsb)));
					printf("Shifting %d to left\n",48-setb-blsb);}
					if((tmp3>>(48-setb-blsb))>1){tmp3=0;
					}
					}
					for(miku=0;miku<assoc;miku++)
					{
						if(tag[tmp2][miku]==tmp3&&valid[tmp2][miku]==1){match_prefetch=1;break;}
					}

					if(match_prefetch==0){
						tt=checkpriority(priority,tmp2,assoc);
						if(DEBUG==1){
							printf("Returned Place For inserting: %lu\n",tt);
							printf("Curr Priority: %d, Curr valid: %ld, Curr tag: %lx\n",priority[tmp2][tt],valid[tmp2][tt],tag[tmp2][tt]);
						}
						tag[tmp2][tt]=tmp3;
						valid[tmp2][tt]=1;
						memr++;
					}
				}	
			}
		}
		if(match==1&&replacement==1){accesslru(priority,tmp2,assoc,wline);}
		if(strcmp(tmp,"W")==0){//do writing
			if(match==1){ch++;memw++;}
			else{cm++;memr++;memw++;}
		}
		if(strcmp(tmp,"R")==0){//do reading
			if(match==1){ch++;}
			else{
				cm++;
				memr++;
			}
		}
		if(DEBUG==1){print(memr,memw,ch,cm);}

	}

	return EXIT_SUCCESS;
}
