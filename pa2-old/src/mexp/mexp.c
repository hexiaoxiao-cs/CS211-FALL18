#include<stdio.h>
#include<stdlib.h>

int** toexp(int m,int**matrix1, int**matrix2)
{
	int tmp1,tmp2,tmp3,sum=0;
	int **ansmat=(int **)malloc(m*sizeof(int *));
	for(tmp1=0;tmp1<m;tmp1++)
	{
		ansmat[tmp1]=(int*)malloc(m*sizeof(int));
	}
	for(tmp1=0;tmp1<m;tmp1++){
		for(tmp2=0;tmp2<m;tmp2++){
			for(tmp3=0;tmp3<m;tmp3++){
				sum+=matrix1[tmp1][tmp3]*matrix2[tmp3][tmp2];
				//	printf("%d * %d = %d\n",matrix1[tmp1][tmp3],matrix2[tmp3][tmp2],sum);
			}
			ansmat[tmp1][tmp2]=sum;
			sum=0;
		}
	}
	return ansmat;
}

int main(int argc, char* argv[])
{
	freopen(argv[1],"r",stdin);
	int m,tmp1=0,tmp2=0,exp;
	scanf("%d\n",&m);
	int **matrix=(int**)malloc(m*sizeof(int*));
	int **ans;
	//printf("%s\n",argv[1]);
	//freopen(argv[1],"r",stdin);
	for(tmp1=0;tmp1<m;tmp1++)
	{
		matrix[tmp1]=(int*)malloc(m*sizeof(int));
	}

	for(tmp1 = 0 ; tmp1<m; tmp1++){
		for(tmp2=0;tmp2<m;tmp2++){
			scanf("%d ",&matrix[tmp1][tmp2]);
		}
		scanf("\n");
	}
	scanf("%d",&exp);
	ans=matrix;
	if(exp==0){
		for(tmp1=0;tmp1<m;tmp1++)
		{
			if(tmp1==0){
			printf("1");}
			else{printf("0");}
			for(tmp2=1;tmp2<m;tmp2++)
			{
				if(tmp1==tmp2){printf(" 1");}
				else{printf(" 0");}
			}
			printf("\n");
		}
		return EXIT_SUCCESS;
	}
	for(tmp1=1;tmp1<exp;tmp1++)
	{
		ans=toexp(m,ans,matrix);
	}
	for(tmp1=0;tmp1<m;tmp1++)
	{
		printf("%d",ans[tmp1][0]);
		for(tmp2=1;tmp2<m;tmp2++)
		{
			printf(" %d",ans[tmp1][tmp2]);
		}
		printf("\n");
	}

	return EXIT_SUCCESS;
}

