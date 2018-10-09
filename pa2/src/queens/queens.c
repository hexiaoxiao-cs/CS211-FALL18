#include<stdio.h>
#include<stdlib.h>
#include<string.h>
void getconflictmap(int board[8][8], int conflict[8][8])
{
	int temp1=0,temp2=0,temp3=0;
	for(temp1=0;temp1<8;temp1++){
		for(temp2=0;temp2<8;temp2++){
			conflict[temp1][temp2]=0;}}

	for(temp1=0;temp1<8;temp1++){
		for(temp2=0;temp2<8;temp2++)
		{
			
			if(board[temp1][temp2]!=0){
				for(temp3=1;temp3<8;temp3++)
				{
					if(temp1-temp3>=0){conflict[temp1-temp3][temp2]=1;}
					if(temp1+temp3<8){conflict[temp1+temp3][temp2]=1;}
					if(temp2-temp3>=0){conflict[temp1][temp2-temp3]=1;}
					if(temp2+temp3<8){conflict[temp1][temp2+temp3]=1;}
					if(temp1-temp3>=0&&temp2-temp3>=0){conflict[temp1-temp3][temp2-temp3]=1;}
					if(temp1+temp3<8&&temp2+temp3<8){conflict[temp1+temp3][temp2+temp3]=1;}
					if(temp1+temp3<8&&temp2-temp3>=0){conflict[temp1+temp3][temp2-temp3]=1;}
					if(temp1-temp3>=0&&temp2+temp3<8){conflict[temp1-temp3][temp2+temp3]=1;}
				}
			}
			if(board[temp1][temp2]==3){
				if(temp1
			}
		}
	}
	return;
}
int check(int conflictmap[8][8],int board[8][8]){
	int temp1,temp2;
	for(temp1=0;temp1<8;temp1++){
		for(temp2=0;temp2<8;temp2++){
			if(conflictmap[temp1][temp2]!=0&&board[temp1][temp2]!=0){
				return 1;
			}
		}
	}
	return 0;

}
int printmatrix(int board[8][8],int conflictmap[8][8])
{
	int tmp1,tmp2;
	for(tmp1=0;tmp1<8;tmp1++){
		for(tmp2=0;tmp2<8;tmp2++)
		{
			if(conflictmap[tmp1][tmp2]!=1){
				if(board[tmp1][tmp2]==2){printf("Q");continue;}
				if(board[tmp1][tmp2]==0){printf("q");continue;}
			}
			else{printf(".");}
		}
		printf("\n");
	}
	

	return 0;
}
void addqueen(int board[8][8],int conflictmap[8][8])
{
	
}
int main(int argc,char* argv[])
{
	int isWarrier=0,isaddQueen=0;
	if(argc<2){printf("INSUFFICIENT ARGUMENT\n");return EXIT_SUCCESS;}
	if(argc==2){}else{
	if(strcmp(argv[argc-2],"-w")==0 || strcmp(argv[argc-3],"-w")==0){isWarrier=1;}
	if(strcmp(argv[argc-2],"+2")==0 || strcmp(argv[argc-3],"+2")==0){isaddQueen=1;}}
	freopen(argv[argc-1],"r",stdin);
	int board[8][8],conflictbd[8][8];
	char tempchar;
	int temp1,temp2;
	int status=0;
	for(temp1=0;temp1<8;temp1++){
		for(temp2=0;temp2<8;temp2++){
			scanf(" %c ",&tempchar);
			if(tempchar=='Q'){board[temp1][temp2]=2;}
			else{if(tempchar=='W'){board[temp1][temp2]=3;}
				else{board[temp1][temp2]=0;}}
		}
	}
	getconflictmap(board,conflictbd);
	status=check(conflictbd,board);
	
	
	if(status==1){printf("Invalid\n"); return EXIT_SUCCESS;}
	else{printmatrix(board,conflictbd);}
	if(isaddQueen==1){addqueen(board,conflictbd);}	
	return EXIT_SUCCESS;
}
