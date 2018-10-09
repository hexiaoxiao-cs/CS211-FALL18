#include<stdio.h>
#include<stdlib.h>
typedef struct node{
	int num;
	struct node *left,*right;
}node;
node* insert(node *root,int num){
	struct node *before,*curr,*rt,*temp;
	rt=root;
	curr=root;
	while(curr!=NULL)
	{
		if(curr->num==num){printf("duplicate\n");return root;}
		if(curr->num<num){before=curr;curr=curr->right;}
		else{before=curr;curr=curr->left;}
	}
	if(rt==NULL){
		rt=(node *)malloc(sizeof(node));
		rt->num=num;
		rt->right=NULL;
		rt->left=NULL;
		printf("inserted\n");
		return rt;
	}
	if(before->num<num){
		temp=(node*)malloc(sizeof(node));
		temp->num=num;
		printf("inserted\n");
		temp->left=NULL;
		temp->right=NULL;
		before->right=temp;
	}
	else{
		temp=(node*)malloc(sizeof(node));
		temp->num=num;
		printf("inserted\n");
		temp->left=NULL;
		temp->right=NULL;
		before->left=temp;
	}
	return rt;
}
node* delete(node *root,int num){
	struct node *rt=NULL,*curr=NULL,*temp=NULL,*temp2=NULL,*before=NULL;
	int toc=0;
	rt=root;
	curr=rt;
	if(rt==NULL){printf("absent\n");return NULL;}
	while(curr!=NULL){
		if(curr->num==num){break;}
		if(curr->num>num){before=curr;curr=curr->left;}
		else{before=curr;curr=curr->right;}	
	}
	if(curr==NULL){printf("absent\n");return rt;}
	temp=curr;
	if(curr->left==NULL){
		if(curr->right==NULL){
			if(before!=NULL){
			if(before->num>num){before->left=NULL;}
			else{
			before->right=NULL;}
			free(curr);
			printf("deleted\n");
			return rt;}
			else{printf("deleted\n");free(rt);return NULL;}
			}	
		else{
			temp=curr->right;
			curr->right=curr->right->right;
			curr->num=temp->num;
			printf("deleted\n");
			free(temp);
			return rt;
		}
	}
	before=temp;

	temp2=temp->left;	
	while(temp2->right!=NULL){
		before=temp2;
		temp2=temp2->right;
	}
	toc=temp2->num;
	if(before==temp){before->left=NULL;}else{
	before->right=NULL;}
	free(temp2);
	printf("deleted\n");
	curr->num=toc;
	return rt;
}
void search(node *root,int num)
{
	struct node *rt=root;
	while(rt!=NULL)
	{
		if(rt->num==num){printf("present\n");return;}
		if(rt->num>num){rt=rt->left;}
		else{rt=rt->right;}
	}
	printf("absent\n");
	return;
}
void printnode(node *root)
{
	if(root==NULL)return;
	printf("(");
	if(root->left!=NULL){printnode(root->left);
	}
	printf("%d",root->num);
	if(root->right!=NULL){printnode(root->right);}
	printf(")");
	return;
}
int main(int argc,char* argv[])
{
	char a;
	int curr;
	struct node *root=NULL;
	while(scanf(" %c",&a)==1){
		if(a=='i'){
			scanf(" %d",&curr);
			root=insert(root,curr);
			continue;
		}
		if(a=='s'){
			scanf(" %d",&curr);
			search(root,curr);
			continue;}
		if(a=='d'){
			scanf(" %d",&curr);
			root=delete(root,curr);
			continue;}
		if(a=='p'){printnode(root);printf("\n");continue;}
		else{break;}
	}
	return EXIT_SUCCESS;
}
