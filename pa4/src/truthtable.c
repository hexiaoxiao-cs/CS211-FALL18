#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct instrblk{
	int instr;
	int *inputs;
	int *outputs;
	instrblk *next;
}instrblk;

typedef struct strarr{
	char *name;
	int num;
	strarr *next;
}strarr;

void insert_var(char *varn,int num,strarr *curr)
{
	int numc=curr->num;
	if(curr==NULL){numc=0;}
	strarr new=(strarr*)malloc(sizeof(strarr));
	new->name=varn;
	new->num=numc+1;
	new->next=NULL;
	if(curr==NULL){curr=new;return;}
	curr->next=new;
	return;
}

char* get_name(strarr* head, int target)
{
	strarr curr=head;
	while(curr!=NULL){
		if(curr->num==target){break;}
		curr=curr->next;}
	if(curr==NULL){return NULL;}
	return curr->name;
}

int get_num(strarr* head, char* val)
{
	strarr curr=head;
	while(curr!=NULL)
	{
		if(strcmp(curr->name,val)==0){break;}
		curr=curr->next;
	}
	if(curr==NULL){return -1;}
	return curr->num;
	
}

void create_instr(instrblk *curr, int func, int *input,int *output)
{
	instrblk new= (instrblk *)malloc(sizeof(instrblk));
	new->instr=func;
	new->inputs=input;
	new->outputs=output;
	if(curr==NULL){curr=new;return;}
	curr->next=new;
	return;
}
int chkdep(instrblk *curr, int *varin)
{
	int tempvar=0;
	if(curr==NULL){return -1;}
	for(tempvar=0;tempvar<(sizeof(curr->inputs)/sizeof(int));tempvar++)
	{
		if(varin[curr->inputs[tempvar]]==0){return -1;}
	}
	return 0;
}
void instrsort(instrblk *head,int valnums)
{
	int a[valnums],tempvar;
	memset(a,-1,sizeof(a));
	instrblk curr=head,before=head,currbefore=head;
	while(curr!=null){

		//check if satisfy dependence
		if(chkdep(curr,a)==-1){currbefore=curr;curr=curr->next;continue;}
		//dependence satisfied
		for(tempvar=0;tempvar<sizeof(curr->outputs)/sizeof(int);tempvar++)
		{
			a[curr->outputs(tempvar)]=1;
		}
		currbefore->next=curr->next;
		curr->next=before->next;
		before->next=curr->next;
		before=curr;
		currbefore=curr;
		curr=curr->next;

		/*if(curr->next==NULL){//last member in the list
			curr=before->next;
			before=curr;
		}
		else{//not the last member in the list
			curr=curr->next;
		}*/
	}
	return;
}
int NOT(int input)
{
	return !input;
}
int AND(int input1,int input2)
{
	return input1&&input2;
}
int OR(int input1,int input2)
{
	return input1||input2;
}
int NAND(int input1,int input2)
{
	return !(input1&&input2);
}
int NOR(int input1,int input2)
{
	return !(input1||input2);
}
int XOR(int input1,int input2)
{

}
int* operation(instrblk *curr, int* vals)
{
	switch(curr->instr){
		case 1: //NOT
			vals[curr->inputs[0]]=NOT(vals[curr->inputs[0]]);
			break;
		case 2: //AND
	}
	if(curr->next==NULL){return vals;}
	else{return operation(curr->next,vals);
}


int main(int argc,char* argv[])
{
	strarr vars=NULL;
	instrblk a=NULL;
	//
	//initializing vals
	//
	
	return EXIT_SUCCESS;

}

