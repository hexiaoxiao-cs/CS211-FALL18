#include<stdio.h>
#include<stdlib.h>
int check(int **board,int x, int y,int height,int width)
{
	int x_up=0,x_down=0,y_right=0,y_left=0;
	int counter=0;
	if(x==0){x_up=height-1;x_down=1;}else{
	if(x==height-1){x_down=0;x_up=x-1;}else
	{x_up=x-1;x_down=x+1;}
	}

	if(y==0){y_left=width-1;y_right=1;}else{
	if(y==width-1){y_right=0;y_left=y-1;}else{
		y_right=y+1;y_left=y-1;}
	}
	if(board[x_up][y_left]==1){counter++;}
	if(board[x_up][y]==1){counter++;}
	if(board[x_up][y_right]==1){counter++;}
	if(board[x][y_left]==1){counter++;}
	if(board[x][y_right]==1){counter++;}
	if(board[x_down][y_left]==1){counter++;}
	if(board[x_down][y]==1){counter++;}
	if(board[x_down][y_right]==1){counter++;}
	if(board[x][y]==1&&(counter!=3&&counter!=2)){return 0;}
	if(board[x][y]==0&&counter==3){return 1;}
	return board[x][y];
}
void printboard(int **board, int height,int width){
	int temp1,temp2;
	for(temp1=0;temp1<height;temp1++){
		for(temp2=0;temp2<width;temp2++)
		{
			if(board[temp1][temp2]==1){printf("*");}
			else{printf(".");
			}
		}
		printf("\n");
	}

	return;
}
int main(int argc, char* argv[])
{
	int times,**board,***tempboard;
	char tempchar;
	int height,width,temp,temp2,counter;
	if(argc!=3){printf("ARGUMENT MISSING!!!\n");return EXIT_SUCCESS;}
	freopen(argv[2],"r",stdin);
	times=atoi(argv[1]);
	scanf("%d %d ",&height,&width);
	board=(int**)malloc(height*sizeof(int*));
	tempboard=(int***)malloc(times*sizeof(int**));
	
	for(temp=0;temp<height;temp++)
	{
		
		board[temp]=(int*)malloc(width*sizeof(int*));

		for(temp2=0;temp2<width;temp2++)
		{
			scanf(" %c ",&tempchar);
			if(tempchar=='.'){board[temp][temp2]=0;}
			else{if(tempchar=='*'){board[temp][temp2]=1;}
				else{printf("INPUT ERROR\n");return EXIT_SUCCESS;}
			}
		}
	}
	counter=0;
	while(counter<times){
		tempboard[counter]=(int**)malloc(height*sizeof(int*));

		for(temp=0;temp<height;temp++)
		{
			tempboard[counter][temp]=(int*)malloc(width*sizeof(int));
			for(temp2=0;temp2<width;temp2++){
				tempboard[counter][temp][temp2]=check(board,temp,temp2,height,width);
	}
		}
		board=tempboard[counter];
		counter++;}
	printboard(board,height,width);
	return EXIT_SUCCESS;
}
