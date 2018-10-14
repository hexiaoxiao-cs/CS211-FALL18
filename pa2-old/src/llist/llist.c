#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct llnode{
	int num;
	struct llnode *next;
}llnode;
llnode* insert(llnode *p, int x)
{
	struct llnode *currhead,*beforehead=NULL,*temp;
	currhead=p;
	if(currhead==NULL)
	{
		currhead=(llnode*)malloc(sizeof(llnode));
		currhead->num=x;
		currhead->next=NULL;
		return currhead;
	}
	while(currhead!=NULL&&currhead->num<=x)
	{
		if(currhead->num==x){return p;}
		beforehead=currhead;
		currhead=currhead->next;
	}
	if(beforehead==NULL){
		beforehead=(llnode*)malloc(sizeof(llnode));
		beforehead->num=x;
		beforehead->next=currhead;
		return beforehead;
	}
	temp=(llnode*)malloc(sizeof(llnode));
	temp->num=x;
	temp->next=currhead;
	beforehead->next=temp;
	return p;
}
int checklength(llnode *p)
{
	int a=0;
	struct llnode *head=p;
	while (head!=NULL)
	{
		a++;
		head=head->next;
	}
	return a;
}
llnode* delete(llnode *p, int target)
{
	struct llnode *currhead,*beforehead;
	currhead=p;
	while(currhead!=NULL&&currhead->num!=target)
	{
		beforehead=currhead;
		currhead=currhead->next;
	}
	if(beforehead==NULL&&currhead!=NULL){beforehead=p;p=p->next;free(beforehead);return p;}
	if(currhead==NULL){return p;}
	beforehead->next=currhead->next;
	free(currhead);
	return p;
}
void printllist(llnode *p,int counter)
{
	struct llnode *curr;
	printf("%d :",counter);
	curr=p;
	while(curr!=NULL){
		printf(" %d",curr->num);
		
		curr=curr->next;
	}
	printf("\n");
	return;
}



int main(int argc,char* argv[])
{
	struct llnode* head=NULL;
	int counter=0,num=0;
	char input;
	
	while(scanf("%c %d",&input,&num)!=EOF){
		if(input=='i'){head=insert(head,num);counter++;printllist(head,checklength(head));}
		if(input=='d'){head=delete(head,num);counter--;printllist(head,checklength(head));}
	}
	return EXIT_SUCCESS;
}
