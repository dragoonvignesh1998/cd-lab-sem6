//Week2
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
int row = 1, col = 1;
void star(FILE* fp){	
	fseek(fp,-1,SEEK_CUR);
	col--;
}
void rpac(){
	FILE *fp, *fb;
	char c, cb;
	fp = fopen("qu1_in.c","r");
	if(fp == NULL){
		printf("Cannot open file \n");
		exit(0);
	}
	fb = fopen("qu1_in_2.c","w");
	c = getc(fp);
	while(!feof(fp)){		
		if(c == '#')
		{
			do
			{
				c = getc(fp);
			} while (c != '\n');
		}
		if(c == '"'){
			do{
				putc(c,fb);
				c = getc(fp);
			}while(c != '"');
		}
		if(c == '/'){
			cb = getc(fp);
			if(cb == '/'){			
				while(cb != '\n'){
					cb = getc(fp);
				}
				putc(cb,fb);
			}
			else if(cb == '*'){
				do{
					while(c != '*'){
						c = getc(fp);
					}
					c = getc(fp);
				} while(c != '/');
			}
		}
		else
			putc(c,fb);			
		c = getc(fp);
	}
	fclose(fp);
	fclose(fb);
}
Token forArith (char c, FILE* fp)
{
	Token t;
	char buf[10];
	int j = 0;
	if(c == '+'){

		buf[j++] = c;
		c = getc(fp);
		col++;		
		if(c == '+')			
		{
			buf[j++] = c;
			buf[j] = '\0';
			t.type = 10;	
		}
		else					
		{
			buf[j]='\0';	
			star(fp);
			t.type = 3;	
		}
	}
	else if(c == '-'){
		buf[j++] = c;
		c = getc(fp);
		col++;		
		if(c == '-')			
		{
			buf[j++] = c;
			buf[j] = '\0';
			t.type = 10;	
		}
		else
		{
			buf[j]='\0';	
			star(fp);
			t.type = 3;	
		}
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
Token forRel(char c, FILE* fp)
{
	Token t;
	int j = 0;
	char buf[10];
	if(c == '=')
	{
		buf[j++] = c;
		c = getc(fp);
		col++;		
		if(c == '=')		 
		{
			buf[j++] = c;
			buf[j] = '\0';
		}
		else					
		{						
			buf[j]='\0';
			star(fp);
			strcpy(t.lexnam,buf);
			t.type = 11;
			t.row = row;
			t.col = col - strlen(buf);
			return t;
		}		
	}	
	else if(c == '<' || c == '>')
	{
		buf[j++] = c;
		c = getc(fp);
		col++;		
		if(c == '=')
		{
			buf[j++] = c;
			buf[j] = '\0';
		}
		else
		{
			buf[j]='\0';	
			star(fp);
		}
	}	
	strcpy(t.lexnam,buf);
	t.type = 4;
	t.row = row;
	t.col = col - strlen(buf);	
	return t;
}
Token forNum(char c,FILE* fp)
{
	Token t;
	char buf[100];
	int j = 0;	
	while(isdigit(c)){
		buf[j++] = c;
		c = fgetc(fp);
		col++;
	}
	star(fp);	
	buf[j] = '\0';	
	strcpy(t.lexnam,buf);
	t.type = 8;
	t.row = row;
	t.col = col - strlen(buf);
	return t;
}
Token forSpec(char c, FILE* fp)
{
	Token t;
	char buf[100];
	int j = 0;	
	if(c == ';' || c == ',' || c == '#')
	{
		buf[j++] = c;
		buf[j] = '\0';
		t.type = 6;
	}	
	else if (c == '"')
	{
		do
		{
			buf[j++] = c;
			c=getc(fp);
			col++;
		}while(c != '"');		
		buf[j++] = c;		
		buf[j] = '\0';
		
		t.type = 9;
	}	
	strcpy(t.lexnam,buf);
	t.row = row;
	t.col = col - strlen(buf);
	return t;
}
Token forBrac(char c, FILE* fp)
{
	Token t;
	char buf[2];	
	buf[0] = c;
	buf[1] = '\0';	
	strcpy(t.lexnam,buf);
	t.type = 12;	
	t.row = row;
	t.col = col - strlen(buf);
	return t;
}
Token forLog(char c,FILE* fp)
{
	Token t;
	int j = 0;
	char buf[10];	
	if(c == '&')
	{
		buf[j++] = c;
		c = getc(fp);
		col++;
		if(c == '&')		
		{
			buf[j++] = c;
			buf[j] = '\0';
			t.type = 5;
		}
		else{				
			buf[j]='\0';
			star(fp);
			t.type = 7;
		}	
	}
	else if (c == '|')
	{
		buf[j++] = c;
		c = getc(fp);
		col++;
		if(c == '|')		
		{
			buf[j++]=c;
			buf[j]='\0';
			t.type = 5;
		}
		else{				
			buf[j]='\0';
			star(fp);
			t.type = 7;
		}	
	}
	else if(c == '!')                
	{
		buf[j++] = c;
		c = getc(fp);
		col++;
		if(c == '=')		
		{
			buf[j++]=c;
			buf[j]='\0';
			t.type = 4;
		}
		else{				
			buf[j]='\0';
			star(fp);
			t.type = 5;
		}	
	}
	strcpy(t.lexnam,buf);
	t.row = row;
	t.col = col - strlen(buf);
	return t;
}
int isKeyword(char buf[]){
	char keywords[15][20] = {"int", "if", "else", "char", "do", "while", "float", "continue", "break", "return","for"
								"while", "do", "const", "typedef"};
	for (int i = 0; i < 15; ++i)
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
	if(c == '+' || c == '-' || c == '*' || c == '/' || c == '%')
	{
		return forArith(c,fp);
	}
	else if (c == '<' || c == '>' || c == '=') 
	{
		return forRel(c,fp);
	}
	else if (c == '&' || c == '|' || c == '!')
	{
		return forLog(c,fp);
	}
	else if (c == '(' || c == ')' || c == '{' || c == '}')
	{
		return forBrac(c,fp);
	}
	else if(c == ';' || c == '"' || c == '#' || c == ',')
	{
		return forSpec(c,fp);
	}
	else if(isdigit(c))
	{
		return forNum(c,fp);
	}
	while(c != ' ' && c != '\n' && isValid(c) && c!=EOF){
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
	return t;
}
void dt(int type){
	switch(type){
		case 1: printf("Keyword");
				break;
		case 2: printf("Identifier");
				break;
		case 3: printf("Arithemtic");
				break;
		case 4: printf("Relational");
				break;
		case 5: printf("Logical");
				break;
		case 6: printf("Special");
				break;
		case 7: printf("Bitwise");
				break;
		case 8: printf("Numeric constant");
				break;
		case 9: printf("String literal");
				break;
		case 10: printf("Unary");
				 break;
		case 11: printf("Assignment");
				 break;
		case 12: printf("Bracket");
				 break;
		case 13: printf("EOF");
				 break;
		default: printf("Unknown");
	}
}
/**int main()
{
	Token t[1000]; 
	rpac();
	FILE *fp =fopen("qu1_in_2.c","r");
	if (fp == NULL){
		printf("Cannot open file \n");
		exit(0);
	}   
	int i = 0;
	while(!feof(fp))
	{
		t[i] = getNextToken(fp);
		printf("%s, %d, %d, ", t[i].lexnam,t[i].row,t[i].col);
		dt(t[i].type);
		printf("\n");
		i++;
	} 
	return 0;
}**/
/**
$-identifier
scalar and foreach-keywords
@_-array
;,),(,{,}- spec chars
+=,=-operators
0-number
**/
