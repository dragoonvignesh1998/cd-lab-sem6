#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
typedef struct
{
	char lexnam[100];
	int row,col;
	int type;
}Token;
int row=1,col=1;
/*void star(FILE* fp){	
	fseek(fp,-1,SEEK_CUR);
	col--;
}*/
Token forOper (char c, FILE* fp)
{
	Token t;
	char buf[10];
	int j = 0;
	if(c == '+'){

		buf[j++] = c;
		c = getc(fp);
		col++;		
		if(c == '=')			
		{
			buf[j++] = c;
			buf[j] = '\0';
			t.type = 10;	
		}
		/**else					
		{
			buf[j]='\0';	
			star(fp);
			t.type = 3;	
		}**/
	}
	else if(c == '='){
		buf[j++] = c;
		c = getc(fp);
		col++;		
		if(c == '-')			
		{
			buf[j++] = c;
			buf[j] = '\0';
			t.type = 10;	
		}
	/*	else					
		{
			buf[j]='\0';	
			star(fp);
			t.type = 3;	
		}**/
	}
	else{
		buf[0] = c;
		buf[1] = '\0';
		t.type = 3;	
	}
	strcpy(t.lexnam,buf);
	t.row = row;
	t.col = col - strlen(buf);	
	return t;
}
Token forSpec(char c, FILE* fp)
{
	Token t;
	char buf[100];
	int j = 0;	
	if(c == ';' || c == '(' || c == ')' || c == '(' || c == ')')
	{
		buf[j++] = c;
		buf[j] = '\0';
		t.type = 6;
	}	
	strcpy(t.lexnam,buf);
	t.row = row;
	t.col = col - strlen(buf);
	return t;
}
int isKeyword(char buf[]){
	char keywords[2][20] = {"scalar","foreach"};
	for (int i = 0; i < 2; ++i)
	{
		if(strcmp(buf,keywords[i]) == 0)
			return 1;
	}
	return 0;
}
int isValid(char c){
	if(isalnum(c) || c == '_')
		return 1;
	return 0;
}
Token getNextToken(FILE* fp){
	Token t;
	char c = fgetc(fp);
	col++;
	if(c == EOF){
		strcpy(t.lexnam, "EOF");
		t.row = row;
		t.col = col - 1;
		t.type = 13;
		return t;
	}
	while(c == ' ' || c == '\t' || c == '\n'){		
		if(c == '\n'){
			row++;
			col = 1;
		}
		else if(c == '\t')
			col += 4;
		c = getc(fp);
		col++;
	}
	char buf[100];
	int counter = 0;
	if(c == '+' || c=='=')
	{
		return forOper(c,fp);
	}
	else if(c == ';' || c == '(' || c == ')' || c == '{' || c == '}')
	{
		return forSpec(c,fp);
	}
	/**while(c != ' ' && c != '\n' && isValid(c) && c!=EOF){
		if(c == '\n'){
			row++;
			col = 1;
		}
		else if(c == '\t')
			col += 4;
		buf[counter++] = c;
		c = fgetc(fp);
		col++;
	}
	if(c != EOF)		
		star(fp);	
	buf[counter] = '\0';
	if(isKeyword(buf))				
		t.type = 1;
	else							
		t.type = 2;
	strcpy(t.lexnam, buf);
	t.row = row;
	t.col = col - strlen(buf);
	return t;**/
}	
int main()
{
	Token t[1000]; 
	FILE *fa,*fb,*fp;
	int ca,cb;
	fa=fopen("q_input_1.perl","r");
	if(fa==NULL)
		printf("Error");
	ca=getc(fa);
	fb=fopen("q_input_2.perl","w");
	while(ca!=EOF)
	{
		if(ca=='#')
		{
			while(ca!='\n')
				ca=getc(fa);
		}
		putc(ca,fb);
		ca=getc(fa);
	}
	fclose(fa);
	fclose(fb);
	fp =fopen("q_input_2.perl","r");
	if (fp == NULL){
		printf("Cannot open file \n");
		exit(0);
	}   
	int i = 0;
	while(!feof(fp))
	{
		t[i] = getNextToken(fp);
		printf("%s, %d, %d, ", t[i].lexnam,t[i].row,t[i].col);
		//dt(t[i].type);
		printf("\n");
		i++;
	} 
	fclose(fp);
	return 0;
}
/**
$-identifier
scalar and foreach-keywords
@_-array
;,),(,{,}- spec chars
+=,=-operators
0-number
**/