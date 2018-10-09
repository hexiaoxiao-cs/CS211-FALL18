#include<stdio.h>
#include<stdlib.h>
#include<math.h>
typedef struct llnode{
	double num;
	struct llnode *next;
}llnode;

int main(int argc,char* argv[])
{
	struct llnode *head=NULL;
	struct llnode *temp=NULL;
	struct llnode *curr=NULL;
	double currnum=0;
	double mean=0;
	double standdev=0;
	double temp2=0;
	int count=0;
	while(scanf("%lf",&currnum)==1)
	{
		if(head==NULL){
			head=(llnode *) malloc(sizeof(llnode));
			head->num=currnum;
			head->next=NULL;
			curr=head;
			}
		else{
			temp=(llnode*)malloc(sizeof(llnode));
			temp->num=currnum;
			temp->next=NULL;
			curr->next=temp;
			curr=temp;
		}
		currnum=0;
		count++;
	}
	curr=head;
	while(curr!=NULL)
	{
		mean+=curr->num;
		curr=curr->next;
	}
	mean=mean/count;
	curr=head;
	printf("mean: %.0f\n",mean);
	while(curr!=NULL)
	{
		temp2+=pow(curr->num-mean,2);
		curr=curr->next;
	}
	temp2=temp2/count;
	standdev=sqrt(temp2);
	printf("stddev: %.0f\n",standdev);
	return EXIT_SUCCESS;

}
