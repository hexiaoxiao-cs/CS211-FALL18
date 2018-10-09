#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc,char *argv[])
{
	char *input,temp='\0';
	int a=0,counter=0,len=0,outlen=0,intlen=0;
	if(argc!=2){printf("%d ARGUMENTS MISSING!!!",2-argc);return EXIT_FAILURE;}
	input=argv[1];
	len=strlen(input);
	char *output,*qq;
	output=(char *)malloc((len*2+1)*sizeof(char));
		for(a=0;a<len;a++){
		if(input[a]>='0' && input[a]<='9'){printf("error\n");return 0;}
		if(temp=='\0'){
				temp=input[a];
				
 				counter++;
				continue;}
		else{if(temp==input[a]){counter++;continue;}
			else{
				output[outlen]=temp;
				intlen=snprintf(NULL,0,"%s%d",output,counter);
				qq=(char *)malloc((intlen+1)*sizeof(char));
				snprintf( qq, intlen + 1, "%s%d", output,counter);
				output=qq;
				intlen=snprintf(NULL,0,"%d",counter);
				counter=1;temp=input[a];outlen+=1+intlen;}}
	}
	output[outlen]=temp;
	intlen=snprintf(NULL,0,"%s%d",output,counter);
	qq=(char *)malloc((intlen+1)*sizeof(char));
	snprintf(qq,intlen+1,"%s%d",output,counter);
	output=qq;

	if(strlen(output)>strlen(input)){printf("%s",input);return 0;}
	printf("%s",output);
	
	return 0;
}
