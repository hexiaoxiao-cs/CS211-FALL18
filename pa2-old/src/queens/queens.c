#include<stdio.h>
#include<stdlib.h>
#include<string.h>
void getconflictmap(int board[8][8], int conflict[8][8])
{
	int temp1=0,temp2=0,temp3=0;
	//int deb1=0,deb2=0;
	for(temp1=0;temp1<8;temp1++){
		for(temp2=0;temp2<8;temp2++){
			conflict[temp1][temp2]=0;}}

	for(temp1=0;temp1<8;temp1++){
		for(temp2=0;temp2<8;temp2++)
		{
			//for(deb1=0;deb1<8;deb1++){
			//for(deb2=0;deb2<8;deb2++){
			//printf("%d",conflict[deb1][deb2]);}
			//printf("\n");
			//}
			//printf("-----------------------\n");	
			if(board[temp1][temp2]!=0){
			//for(deb1=0;deb1<8;deb1++){
			//for(deb2=0;deb2<8;deb2++){
			//printf("%d",conflict[deb1][deb2]);}
			//printf("\n");
			//}
			//printf("----------------\n");

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
				if(temp1-2>=0){
					if(temp2-1>=0){conflict[temp1-2][temp2-1]=1;}
					//else{conflict[temp1-2][8+temp2-1]=1;}
					if(temp2+1<8){conflict[temp1-2][temp2+1]=1;}
					//else{conflict[temp1-2][temp2+1-8]=1;}
				}
			//	else{
			//		if(temp2-1>=0){conflict[temp1-2+8][temp2-1]=1;}
					//else{conflict[temp1-2+8][8+temp2-1]=1;}
			//		if(temp2+1<8){conflict[temp1-2+8][temp2+1]=1;}
					//else{conflict[temp1-2+8][temp2+1-8]=1;}
			//	}
				if(temp1+2<8){
					if(temp2-1>=0){conflict[temp1+2][temp2-1]=1;}
					//else{conflict[temp1+2][8+temp2-1]=1;}
					if(temp2+1<8){conflict[temp1+2][temp2+1]=1;}
					//else{conflict[temp1+2][temp2+1-8]=1;}
				}
			//	else{
			//		if(temp2-1>=0){conflict[temp1+2-8][temp2-1]=1;}
					//else{conflict[temp1+2-8][8+temp2-1]=1;}
			//		if(temp2+1<8){conflict[temp1+2-8][temp2+1]=1;}
					//else{conflict[temp1+2-8][temp2+1-8]=1;}
			//	}
				if(temp1-1>=0){
					if(temp2-2>=0){conflict[temp1-1][temp2-2]=1;}
					//else{conflict[temp1-1][8+temp2-2]=1;}
					if(temp2+2<8){conflict[temp1-1][temp2+2]=1;}
					//else{conflict[temp1-1][temp2+2-8]=1;}
				}
			//	else{
			//		if(temp2-2>=0){conflict[temp1-1+8][temp2-2]=1;}
					//else{conflict[temp1-1+8][8+temp2-2]=1;}
			//		if(temp2+2<8){conflict[temp1-1+8][temp2+2]=1;}
					//else{conflict[temp1-1+8][temp2+2-8]=1;}
			//	}
				if(temp1+1<8){
					if(temp2-2>=0){conflict[temp1+1][temp2-2]=1;}
					//else{conflict[temp1+1][8+temp2-2]=1;}
					if(temp2+2<8){conflict[temp1+1][temp2+2]=1;}
					//else{conflict[temp1+1][temp2+2-8]=1;}
				}
			//	else{
			//		if(temp2-2>=0){conflict[temp1+1-8][temp2-2]=1;}
					//else{conflict[temp1+1-8][8+temp2-2]=1;}
			//		if(temp2+2<8){conflict[temp1+1-8][temp2+2]=1;}
					//else{conflict[temp1+1-8][temp2+2-8]=1;}
			//	}



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
int printmatrix_withwarrier(int board[8][8],int conflictmap[8][8])
{
	int tmp1,tmp2;
	int tmpb[8][8],tmpc[8][8];
	for(tmp1=0;tmp1<8;tmp1++){
		for(tmp2=0;tmp2<8;tmp2++)
		{
			if(conflictmap[tmp1][tmp2]!=1){
				if(board[tmp1][tmp2]==2){printf("Q");continue;}
				if(board[tmp1][tmp2]==3){printf("W");continue;}
				if(board[tmp1][tmp2]==0){
					memcpy(tmpb,board,8*8*sizeof(int));
					tmpb[tmp1][tmp2]=3;
					getconflictmap(tmpb,tmpc);
					if(check(tmpc,tmpb)==1){printf("q");continue;}
					else{printf("w");continue;}
				}
			}
			else{printf(".");}
		}
		printf("\n");
	}
	

	return 0;
}
int addqueen(int board[8][8],int conflictmap[8][8],int depth)
{
	int t1=0,t2=0,flag=0,tmpf=0;
	int tempconf[8][8],tempboard[8][8];
	if(depth>2){return 0;}
	for(t1=0;t1<8;t1++)
	{
		for(t2=0;t2<8;t2++)
		{
			if(conflictmap[t1][t2]!=1&&board[t1][t2]==0)
			{
				memcpy(tempboard,board,8*8*sizeof(int));
				tempboard[t1][t2]=2;
				getconflictmap(tempboard,tempconf);
				//if(check(tempconf,tempboard)==1){return flag;}
				//printf("LOOP\n");
				tmpf=addqueen(tempboard,tempconf,depth+1);
				if(tmpf+1>flag){flag=tmpf+1;}
			}
		}
	}

	return flag;
			
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
	int howmanyqueens=0;
	for(temp1=0;temp1<8;temp1++){
		for(temp2=0;temp2<8;temp2++){
			scanf(" %c ",&tempchar);
			if(tempchar=='Q'){board[temp1][temp2]=2;}
			else{if(tempchar=='W'){board[temp1][temp2]=3;}
				else{if(tempchar=='.'){board[temp1][temp2]=0;}
					else{printf("WRONGFILE!!!");return EXIT_SUCCESS;}}}
		}
	}
	getconflictmap(board,conflictbd);
	status=check(conflictbd,board);
	
	
	if(status==1){printf("Invalid\n"); return EXIT_SUCCESS;}
	else{if(isWarrier==1){printmatrix_withwarrier(board,conflictbd);}else{printmatrix(board,conflictbd);}}
	if(isaddQueen==1){howmanyqueens=addqueen(board,conflictbd,0);
	//printf("%d",howmanyqueens);
	if(howmanyqueens==0){printf("Zero\n");}
	if(howmanyqueens==1){printf("One\n");}
	if(howmanyqueens>=2){printf("Two or more\n");}}
	
	
	return EXIT_SUCCESS;
}
