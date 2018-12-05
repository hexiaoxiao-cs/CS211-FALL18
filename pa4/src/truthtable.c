#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ctype.h>

int DEBUG=0;
int powy(int x,int y)
{
	int tmp=0,toret=x;
	if(y==0){return 1;}
	for(tmp=1;tmp<y;tmp++)
	{
		toret*=x;
	}
	return toret;
}

typedef struct instrblk{
	int instr;
	int *inputs;
	int *outputs;
	struct instrblk *next;
} instrblk;

typedef struct strarr{
	char *name;
	int num;
	struct strarr *next;
} strarr;

int insert_var(char *varn,strarr *curr)
{
	int numc;
	strarr *haha=curr;
	if(curr==NULL){numc=0;}else{while(haha->next!=NULL){haha=haha->next;}numc=haha->num;}
	strarr *new=(strarr*)malloc(sizeof(strarr));
	new->name=varn;
	numc++;
	new->num=numc;
	new->next=NULL;
	if(curr==NULL){curr=new;return 1;}
	haha->next=new;
	return numc;
}

char* get_name(strarr* head, int target)
{
	strarr *curr=head;
	while(curr!=NULL){
		if(curr->num==target){break;}
		curr=curr->next;}
	if(curr==NULL){return NULL;}
	return curr->name;
}

int get_num(strarr* head, char* val)
{
	strarr *curr=head;
	while(curr!=NULL)
	{
		if(strcmp(curr->name,val)==0){break;}
		curr=curr->next;
	}
	if(curr==NULL){return -1;}
	return curr->num;
	
}

/*void create_instr(instrblk *curr, int func, int *input,int *output)
{
	instrblk new= (instrblk *)malloc(sizeof(instrblk));
	new->instr=func;
	new->inputs=input;
	new->outputs=output;
	if(curr==NULL){curr=new;return;}
	curr->next=new;
	return;
}*/

int chkdep(instrblk *curr, int *varin)
{
	int tempvar=0;
	int size=0;
	if(curr==NULL){return -1;}
	if(curr->instr==2){size=1;}
	if(curr->instr>=3&&curr->instr<=6){size=2;}
	if(curr->instr==7){size=-(curr->inputs[0])+1;tempvar=1;}
	if(curr->instr==8){tempvar=1;size=powy(2,-curr->inputs[0])+1-curr->inputs[0];}
	if(curr->instr==9){size=1;}
	if(DEBUG==1){printf("CHKDEP START\n");}
	for(;tempvar<size;tempvar++)
	{
		if(DEBUG==1){
		printf("Checking %d\n",tempvar);}
		if (curr->inputs[tempvar] > 0) {
			if (varin[curr->inputs[tempvar]] == -1) { 
				if(DEBUG==1){
					printf("We need: %d, do we have?:%d",curr->inputs[tempvar],varin[curr->inputs[tempvar]]);}
			return -1; }
		}else
		{ }

	}
	if(DEBUG==1){printf("CHKDEP STOPPED\n");}
	return 0;
}
void printarr(int* a,int valnums)
{
	int tmp=1;
	printf("PRINT CURR ARR\n");
	for(tmp=1;tmp<valnums;tmp++)
	{
		printf("%d:%d\n",tmp,a[tmp]);
	}
	printf("\n");
}
void instrsort(instrblk *head,int valnums)
{
	int *a,tempvar=0,size=0;
	int counter=0;
	int flag=0;
	instrblk *curr=head,*before=head,*currbefore=head;
	a=(int*)malloc(valnums*sizeof(int));
	memset(a,-1,sizeof(int)*valnums);
	if(DEBUG==1){
		printf("\nSTARTSORT\n");}
	while(curr!=NULL){
		if(curr==NULL){break;}
		if(DEBUG==1){
		printf("%d\n",counter);}
		counter++;
		if(curr->instr==0){//inputblock
			for(tempvar=1;tempvar<=-(curr->outputs[0]);tempvar++){
				a[curr->outputs[tempvar]]=1;
			}
			before=curr;
			currbefore=curr;
			curr=curr->next;
			continue;
		}
		//check if satisfy dependence
		if(chkdep(curr,a)==-1){
			if(DEBUG==1){
			printf("%d Block Not Pass, Continue to next block\n",curr->instr);
			printarr(a,valnums);}
			currbefore=curr;
			curr=curr->next;
			flag=1;
			continue;}
		//dependence satisfied
		if(DEBUG==1){
		printf("%d DEPENDENCE SATISFIED\n",curr->instr);
		
		printarr(a,valnums);
		}
		if(curr->instr==1){tempvar=0;size=1;}
		if(curr->instr==2){tempvar=0;size=1;}
		if(curr->instr>=3&&curr->instr<=6){tempvar=0;size=1;}
		if(curr->instr==7){size=powy(2,-(curr->inputs[0]));tempvar=0;}
		if(curr->instr==8){tempvar=0;size=1;}
		if(curr->instr==9){tempvar=0;size=1;}
		for(;tempvar<size;tempvar++)
		{
			if (curr->outputs[tempvar] > 0) {
				a[curr->outputs[tempvar]] = 1;
			}
		}
		if(flag==1){
			if(DEBUG==1){
				printf("We need to switch two block from: %d, to %d\n",curr->instr,before->instr);}	
			currbefore->next=curr->next;
			curr->next=before->next;
			before->next=curr;
			before=curr;
			currbefore=curr;
			curr=curr->next;
			flag=0;}
		else
		{
			if(DEBUG==1){
			printf("CONTINUE TO NEXT\n");}
			before=curr;
			currbefore=curr;
			curr=curr->next;
		}

		/*if(curr->next==NULL){//last member in the list
			curr=before->next;
			before=curr;
		}
		else{//not the last member in the list
			curr=curr->next;
		}*/
	}
	if(DEBUG==1){
		printf("EXIT SORT\n");}
	return;
}
int NOT(int input)
{
	if(DEBUG==1){
		printf("Not\n%d\n",input);}
	return !input;
}
int AND(int input1,int input2)
{
	if(DEBUG==1){
		printf("AND\n%d %d\n",input1,input2);}
	return input1&&input2;
}
int OR(int input1,int input2)
{
	if(DEBUG==1){
	printf("OR\n%d %d\n",input1,input2);}
	return input1||input2;
}
int NAND(int input1,int input2)
{
	if(DEBUG==1){
	printf("NAND\n%d %d\n",input1,input2);}
	return !(input1&&input2);
}
int NOR(int input1,int input2)
{
	if(DEBUG==1){
	printf("NOR\n%d %d\n",input1,input2);}
	return !(input1||input2);
}
int XOR(int input1,int input2)
{
	if(DEBUG==1){
	printf("XOR\n%d %d\n",input1,input2);}
	if((input1||input2)==1&&!(input1&&input2)){return 1;}
	else{return 0;}
}
int DECODER(int n, int* input1) //return the place of the digit with 1
{
	int miku,hatsune=0;
	int temp = n-2;
	if(DEBUG==1){
	printf("Decoder Block:\n");}
	for(miku=1;miku<n;miku++)
	{
		hatsune+=input1[miku]*powy(2,temp);
		if(DEBUG==1){
				printf("%d %d  %d  Curr Sum:%d\n",temp,input1[miku],powy(2,temp),hatsune);}
		temp--;
	}
	return hatsune;
}
int MUX(int n, int* input1)
//input1 is the "selector"
//input2 is to be selected
{
	int miku,hatsune=0;
	int temp = n-1;
	if(DEBUG==1){
		printf("MUX:\n");}
	for(miku= powy(2, n)+1;miku<powy(2, n)+n+1;miku++)
	{
		if(DEBUG==1){
				printf("Digit:%d,Place:%d,Datainarray:%d\n", temp, miku, input1[miku]);
				printf("power of digit: %d\n", powy(2, temp));}
		hatsune+=input1[miku]*powy(2,temp);
		temp--;
		
	}
	if(DEBUG==1){
	printf("Adder:%d    THis should give the place of the output\n", hatsune);}
	return input1[hatsune+1];
}	
void LOAD_EFFECTIVE_VALUE(int* vals, instrblk *curr, int* hatsune,int size) //hatsune is the return array, size 
{
	int miku = 0;
	
	
	for(;miku<size;miku++)
	{
		if (curr->inputs[miku] <= 0) { hatsune[miku] = -curr->inputs[miku]; continue; }
		hatsune[miku]=vals[curr->inputs[miku]];
		
	}
	return;
}
int* operation(instrblk *curr, int* vals)
{
	int *hatsune,miku;
	int temp = 0;
	switch(curr->instr){
		case 0: break;
		case 1: //NOT
			hatsune = (int*)malloc(sizeof(int));
			LOAD_EFFECTIVE_VALUE(vals, curr, hatsune, 1);
			vals[curr->outputs[0]]=NOT(hatsune[0]);
			break;
		case 2: //AND
			hatsune = (int*)malloc(2 * sizeof(int));
			LOAD_EFFECTIVE_VALUE(vals, curr, hatsune, 2);
			vals[curr->outputs[0]]=AND(hatsune[0],hatsune[1]);
			break;
		case 3: //OR
			hatsune = (int*)malloc(2 * sizeof(int));
			LOAD_EFFECTIVE_VALUE(vals, curr, hatsune, 2);
			vals[curr->outputs[0]]=OR(hatsune[0],hatsune[1]);
			break;
		case 4: //NAND
			hatsune = (int*)malloc(2 * sizeof(int));
			LOAD_EFFECTIVE_VALUE(vals, curr, hatsune, 2);
			vals[curr->outputs[0]]=NAND(hatsune[0],hatsune[1]);
			break;
		case 5: //NOR
			hatsune = (int*)malloc(2 * sizeof(int));
			LOAD_EFFECTIVE_VALUE(vals, curr, hatsune, 2);
			vals[curr->outputs[0]]=NOR(hatsune[0],hatsune[1]);
			break;
		case 6: //XOR
			hatsune = (int*)malloc(2 * sizeof(int));
			LOAD_EFFECTIVE_VALUE(vals, curr, hatsune, 2);
			vals[curr->outputs[0]]=XOR(hatsune[0],hatsune[1]);
			break;
		case 7: //DECODER
			hatsune=(int*)malloc((1-curr->inputs[0])*sizeof(int));
			LOAD_EFFECTIVE_VALUE(vals,curr,hatsune,1-curr->inputs[0]);
			temp = DECODER(1-curr->inputs[0], hatsune);
			vals[curr->outputs[temp]]=1;
			break;
		case 8://MUX
			hatsune = (int*)malloc((1 - curr->inputs[0] + powy(2, -curr->inputs[0])) * sizeof(int));
			LOAD_EFFECTIVE_VALUE(vals,curr,hatsune, 1 - curr->inputs[0] + powy(2, -curr->inputs[0]));
			miku=hatsune[0];
			vals[curr->outputs[0]]=MUX(miku,hatsune);
			break;
		case 9://PASS
			hatsune = (int*)malloc(sizeof(int));
			LOAD_EFFECTIVE_VALUE(vals,curr,hatsune, 1);
			if(DEBUG==1){printf("PASS BLOCK\nPassing from %d:%d to %d\n",curr->inputs[0],hatsune[0],curr->outputs[0]);}
			vals[curr->outputs[0]]=hatsune[0];
			break;
	}
	if(curr->next==NULL){return vals;}
	else{return operation(curr->next,vals);}
}

void printTruth(instrblk *instr,int *outputs,int valnums){
	int numinput=-instr->outputs[0],numoutput=-outputs[0],pos=0;
	int *a,*mask;
	a=(int*)malloc(valnums*sizeof(int));
	mask = (int*)malloc(valnums * sizeof(int));

	int size=powy(2,numinput),tempvar,tempvar1,OF=0;
	memset(a,0,sizeof(int)*valnums);
	memset(mask, 0, sizeof(int)*valnums);
	for (tempvar = 1; tempvar <= numinput; tempvar++) {
		mask[instr->outputs[tempvar]] = 1;
	}
	for(tempvar1=1;tempvar1<=numinput;tempvar1++){
	printf("%d ",a[instr->outputs[tempvar1]]);
	}
	operation(instr,a);
	printf("|");
	for(tempvar1=1;tempvar1<=numoutput;tempvar1++){
		printf(" %d",a[outputs[tempvar1]]);
	}
	printf("\n");
	for(tempvar=1;tempvar<size;tempvar++)
	{
		OF=0;
		a[instr->outputs[numinput]]++;
		if(a[instr->outputs[numinput]]==2){
			a[instr->outputs[numinput]]=0;
			OF=1;
			pos=numinput-1;
			while(OF!=0){
				
				a[instr->outputs[pos]]++;
				OF=0;
				if(a[instr->outputs[pos]]==2){
					a[instr->outputs[pos]]=0;
					OF=1;
					pos--;
				}
			}
		}
		//set all other values to zero
		for (tempvar1 = 0; tempvar1 < valnums; tempvar1++)
		{
			a[tempvar1] *= mask[tempvar1];

		}
		for(tempvar1=1;tempvar1<=numinput;tempvar1++){
			printf("%d ",a[instr->outputs[tempvar1]]);
		}
		operation(instr,a);
		printf("|");
		for(tempvar1=1;tempvar1<=numoutput;tempvar1++){
			printf(" %d",a[outputs[tempvar1]]);
		}
		printf("\n");
	}
	if(DEBUG==1){
	printf("%d %d",numoutput,valnums);}
	return;
}

int* scanstring(strarr **vars)
{
	char* tmp;
	char temp;
	int nowint=0,last=0,*output,tt;
	int stop=0;
	int isDigit=1;//1 is true 0 is false
	output=(int*)malloc(sizeof(int));
	tmp=(char*)malloc(sizeof(char));
	while(1)
	{		
		if(stop==1){
			//printf("\n%d\n",(int)(sizeof(output)/sizeof(int)));
			//printf("%d\n",(int)sizeof(output));
			//printf("%d\n",output[nowint-1]);
			return output;}
		if(scanf("%c",&temp)==EOF){
			stop = 1;

		}

		if(temp==':'){continue;}
		if(temp==' '||temp=='\n'||stop==1){
			if(temp=='\n'){stop=1;}
			if(last==0){if(temp=='\n'){return output;}continue;}
			else{last=0;
				if(isDigit==1){output=(int*)realloc(output,(nowint+1)*sizeof(int));
				output[nowint]=-atoi(tmp);
				last=0;
				nowint++;
				isDigit=1;
				tmp=(char*)malloc(sizeof(char));if(temp=='\n'){return output;}
				continue;}
				else{
				//printf("%s:",tmp);
				tt=get_num(*vars,tmp);
				//printf("%d\n",tt);
				if(tt==-1){
					if(*vars==NULL){
						strarr *new=(strarr*)malloc(sizeof(strarr));
						new->name=tmp;
						new->num=1;
						new->next=NULL;
						*vars=new;
						tt=1;
					}else{
					tt=insert_var(tmp,*vars);}
					}
				output=(int*)realloc(output,(nowint+1)*sizeof(int));
				//printf("nowint:%d\nintsize:%d",nowint,(int)sizeof(int));
				//printf("output:%d",(int)sizeof(output));
				output[nowint]=tt;
				nowint++;
				last=0;
				isDigit=1;
				tmp=(char*)malloc(sizeof(char));if(temp=='\n'){return output;}continue;
					}
				
			}
		}
		
		if(isdigit(temp)==0){isDigit=0;}
		tmp=(char*)realloc(tmp,(last+2)*sizeof(char));
		tmp[last]=temp;
		tmp[last+1]='\0';
		//printf("%s\n",tmp);
		if(nowint==0){}else{
		//printf("%d\n",output[nowint-1]);
		}
		last++;
	}

}

void toinout(int* orig,instrblk *instr,int where,int length){
	int ptr=0;
	instr->inputs=(int*)malloc(sizeof(int)*(where));
	instr->outputs=(int*)malloc(sizeof(int)*(length-where));
	while(ptr<length){
		if(ptr<where){//"FOR WHERE GIVE RIGHT POINT"
		
		instr->inputs[ptr]=orig[ptr];}
		else{
			instr->outputs[ptr-where]=orig[ptr];}
		ptr++;
	}
	return;
}

void print_instrblk(instrblk *instr){
	int temp;
	switch(instr->instr)
	{
		case 0: printf("Input Block\n Outputs:");for(temp=1;temp<=-(instr->outputs[0]);temp++){printf("%d ",instr->outputs[temp]);}printf("\n");break;
		case 1: printf("NOT Block\n Inputs:%d\nOutputs:%d\n\n",instr->inputs[0],instr->outputs[0]);break;
		case 2: printf("AND Block\n Inputs:%d,%d\nOutputs:%d\n\n",instr->inputs[0],instr->inputs[1],instr->outputs[0]);break;
		case 3: printf("OR Block\n Inputs:%d,%d\nOutputs:%d\n\n",instr->inputs[0],instr->inputs[1],instr->outputs[0]);break;
		case 4: printf("NAND Block\n Inputs:%d,%d\nOutputs:%d\n\n",instr->inputs[0],instr->inputs[1],instr->outputs[0]);break;
		case 5: printf("NOR Block\n Inputs:%d,%d\nOutputs:%d\n\n",instr->inputs[0],instr->inputs[1],instr->outputs[0]);break;
		case 6: printf("XOR Block\n Inputs:%d,%d\nOutputs:%d\n\n",instr->inputs[0],instr->inputs[1],instr->outputs[0]);break;
		case 7: printf("DECODER\n Numbers:%d\n", -instr->inputs[0]);
			printf("Inputs:\n");
			for (temp = 1; temp <= -instr->inputs[0]; temp++)
			{
				printf("%d ", instr->inputs[temp]);
			}
			printf("\nOutputs:\n");
			for (temp = 0; temp <powy(2,-instr->inputs[0]); temp++)
			{
				printf("%d ", instr->outputs[temp]);
			}
			printf("\n");
			break;
		case 8: printf("MUX\n Numbers:%d\n", -instr->inputs[0]);
			printf("Inputs:\n");
			for (temp = 1; temp <powy(2, -instr->inputs[0])-instr->inputs[0]+1; temp++)
			{
				printf("%d ", instr->inputs[temp]);
			}
			printf("\nOutputs:\n");
			printf("%d", instr->outputs[0]);
			printf("\n");
			break;
		case 9: printf("PASS BLOCK\n Passing %d to %d\n",instr->inputs[0],instr->outputs[0]);break;
	}
	return;
}

int* parse(strarr **vars, instrblk **instr)
{
	char ins[100];
	instrblk *tmp1,*tmp2;
	int *output;
	int *tmp3;
	//int hh;
	while(scanf(" %s ",ins)!=EOF)
	{
		tmp1=(instrblk*) malloc(sizeof(instrblk));
		if(strcmp(ins,"INPUT")==0){tmp1->instr=0;tmp1->inputs=NULL;tmp1->outputs=scanstring(vars);}
		if(strcmp(ins,"OUTPUT")==0){output=scanstring(vars);continue;}
		if(strcmp(ins,"NOT")==0){tmp1->instr=1;tmp3=scanstring(vars);toinout(tmp3,tmp1,1,2);}
		if(strcmp(ins,"AND")==0){tmp1->instr=2;tmp3=scanstring(vars);toinout(tmp3,tmp1,2,3);}
		if(strcmp(ins,"OR")==0){tmp1->instr=3;tmp3=scanstring(vars);toinout(tmp3,tmp1,2,3);}
		if(strcmp(ins,"NAND")==0){tmp1->instr=4;tmp3=scanstring(vars);toinout(tmp3,tmp1,2,3);}
		if(strcmp(ins,"NOR")==0){tmp1->instr=5;tmp3=scanstring(vars);toinout(tmp3,tmp1,2,3);}
		if(strcmp(ins,"XOR")==0){tmp1->instr=6;tmp3=scanstring(vars);toinout(tmp3,tmp1,2,3);}
		if(strcmp(ins,"DECODER")==0){tmp1->instr=7;
		tmp3=scanstring(vars);
		toinout(tmp3,tmp1,-tmp3[0]+1,1-tmp3[0]+powy(2,-tmp3[0]));}
		if(strcmp(ins,"MULTIPLEXER")==0){tmp1->instr=8;tmp3=scanstring(vars);toinout(tmp3,tmp1,powy(2,-tmp3[0])+1-tmp3[0],2-tmp3[0]+powy(2,-tmp3[0]));}
		if(strcmp(ins,"PASS")==0){tmp1->instr=9;tmp3=scanstring(vars);toinout(tmp3,tmp1,1,2);}
/* 		printf("%d\n",tmp1->instr);
		printf("Inputs:\n");
		if(tmp1->inputs==NULL){printf("No Inputs\n");}
		else{
			for(hh=0;hh<sizeof(tmp1->inputs)/sizeof(int);hh++)
			{
				printf("%d  ",tmp1->inputs[hh]);
			}
		}
		printf("\nOutputs:\n");
		if(tmp1->outputs==NULL){printf("No Outputs\n");}
		else{
			for(hh=0;hh<(sizeof(tmp1->outputs)/sizeof(int));hh++)
			{
				printf("%d  ",tmp1->outputs[hh]);
			}
		}
		printf("\n\n"); */
		if(DEBUG==1){
				print_instrblk(tmp1);}
		tmp1->next=NULL;
		if(*instr==NULL){*instr=tmp1;tmp2=tmp1;}
		else{tmp2->next=tmp1;tmp2=tmp2->next;}

	}	
	
	return output;
}



int main(int argc,char* argv[])
{
	strarr *vars=NULL,*tempstrarr;
	instrblk *a=NULL,*b=NULL;
	int cntarg=0,*output;
	//
	//initializing vals
	//
	if(argc>1){freopen(argv[1],"r",stdin);}
	output=parse(&vars,&a);
	tempstrarr=vars;
	while(tempstrarr!=NULL){cntarg++;tempstrarr=tempstrarr->next;}
	//printf("There are %d variables in the instruction",cntarg);
	instrsort(a,cntarg+1);
	if(DEBUG==1){
		printf("Execute Print INSTRBLK After Sort\n");
		b=a;
		while(b!=NULL){print_instrblk(b);b=b->next;}
		}
	printTruth(a,output,cntarg+1);
	
	return EXIT_SUCCESS;

}
