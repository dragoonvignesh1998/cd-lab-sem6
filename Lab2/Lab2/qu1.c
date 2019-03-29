#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct
{
	char lexemename[100];
	int row,col;
	int type;
}Token;

int row = 1, col = 1;


void retract(FILE* fp){
	
	fseek(fp,-1,SEEK_CUR);
	col--;
}

void remove_prepro_and_comments(){

	FILE *fp, *fb;

	char c, cb;

	fp = fopen("q1_in.c","r");

	if(fp == NULL){
		printf("Cannot open file \n");
		exit(0);
	}

	fb = fopen("q1_in_2.c","w");

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

			if(cb == '/'){			//Single line comment (Pun ++)
				while(cb != '\n'){
					cb = getc(fp);
				}

				putc(cb,fb);
			}

			else if(cb == '*'){		//Multi-line comment

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

Token handleArithmetic (char c, FILE* fp)
{
	Token t;
	char buf[10];
	int j = 0;

	if(c == '+'){

		buf[j++] = c;
		c = getc(fp);
		col++;
		
		if(c == '+')			// Unary
		{
			buf[j++] = c;
			buf[j] = '\0';
			t.type = 10;	
		}
		
		else					
		{
			buf[j]='\0';	
			retract(fp);
			t.type = 3;	
		}
	}

	else if(c == '-'){
		buf[j++] = c;
		c = getc(fp);
		col++;
		
		if(c == '-')			// Unary
		{
			buf[j++] = c;
			buf[j] = '\0';
			t.type = 10;	
		}
		
		else
		{
			buf[j]='\0';	
			retract(fp);
			t.type = 3;	
		}
	}

	else{
		buf[0] = c;
		buf[1] = '\0';
		t.type = 3;	
	}

	strcpy(t.lexemename,buf);
	t.row = row;
	t.col = col - strlen(buf);
	
	return t;
}

Token handleRelational(char c, FILE* fp)
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

		else					//Assignment operator
		{					
			
			buf[j]='\0';
			retract(fp);

			strcpy(t.lexemename,buf);
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
			retract(fp);
		}
	}
	
	strcpy(t.lexemename,buf);
	t.type = 4;
	t.row = row;
	t.col = col - strlen(buf);
	
	return t;
}

Token handleNumericConstant(char c,FILE* fp)
{
	Token t;
	char buf[100];
	int j = 0;
	
	while(isdigit(c)){
		buf[j++] = c;
		c = fgetc(fp);
		col++;
	}

	retract(fp);
	
	buf[j] = '\0';
	
	strcpy(t.lexemename,buf);
	t.type = 8;
	t.row = row;
	t.col = col - strlen(buf);
	return t;
}

Token handleSpecial(char c, FILE* fp)
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
		
		buf[j++] = c;		//add closing "
		buf[j] = '\0';
		
		t.type = 9;
	}
	
	strcpy(t.lexemename,buf);
	t.row = row;
	t.col = col - strlen(buf);
	return t;
}

Token handleBracket(char c, FILE* fp)
{
	Token t;
	char buf[2];
	
	buf[0] = c;
	buf[1] = '\0';
	
	strcpy(t.lexemename,buf);
	t.type = 12;	
	t.row = row;
	t.col = col - strlen(buf);

	return t;
}


Token handleLogical(char c,FILE* fp)
{
	Token t;
	int j = 0;
	char buf[10];
	
	if(c == '&')
	{
		buf[j++] = c;
		c = getc(fp);
		col++;
		
		if(c == '&')		//Logical
		{
			buf[j++] = c;
			buf[j] = '\0';
			t.type = 5;
		}
		else{				//Bitwise
			buf[j]='\0';
			retract(fp);
			t.type = 7;
		}	
	}
	
	else if (c == '|')
	{
		buf[j++] = c;
		c = getc(fp);
		col++;
		
		if(c == '|')		//Logical
		{
			buf[j++]=c;
			buf[j]='\0';
			t.type = 5;
		}
		
		else{				//Bitwise
			buf[j]='\0';
			retract(fp);
			t.type = 7;
		}	
	}
	
	else if(c == '!')                //handling the ! and != case
	{
		buf[j++] = c;
		c = getc(fp);
		col++;
		
		if(c == '=')		// Relational
		{
			buf[j++]=c;
			buf[j]='\0';
			t.type = 4;
		}
		
		else{				//Logical
			buf[j]='\0';
			retract(fp);
			t.type = 5;
		}	
	}

	strcpy(t.lexemename,buf);
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
		strcpy(t.lexemename, "EOF");
		t.row = row;
		t.col = col - 1;
		t.type = 13;
		return t;
	}

	while(c == ' ' || c == '\t' || c == '\n'){		//skip whitespace
		
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
		return handleArithmetic(c,fp);
	}
	else if (c == '<' || c == '>' || c == '=') //direct the != case from handleLogical to handleRelational
	{
		return handleRelational(c,fp);
	}
	else if (c == '&' || c == '|' || c == '!')
	{
		return handleLogical(c,fp);
	}
	else if (c == '(' || c == ')' || c == '{' || c == '}')
	{
		return handleBracket(c,fp);
	}
	else if(c == ';' || c == '"' || c == '#' || c == ',')
	{
		return handleSpecial(c,fp);
	}
	else if(isdigit(c))
	{
		return handleNumericConstant(c,fp);
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

	if(c != EOF)		//Very important. If c == EOF and we retract then infinite loop
		retract(fp);	// if c!= EOF then we retract because we read an extra character

	buf[counter] = '\0';

	if(isKeyword(buf))				//keyword
		t.type = 1;

	else							//identifier
		t.type = 2;

	strcpy(t.lexemename, buf);
	t.row = row;
	t.col = col - strlen(buf);

	return t;
	
}

void display_type(int type){

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


int main()
{
	Token t[1000]; 

	remove_prepro_and_comments();
	
	FILE *fp =fopen("q1_in_2.c","r");
	
	if (fp == NULL){
		printf("Cannot open file \n");
		exit(0);
	}   

	int i = 0;

	while(!feof(fp))
	{
		t[i] = getNextToken(fp);
		printf("< %s, %d, %d, ", t[i].lexemename,t[i].row,t[i].col);
		display_type(t[i].type);
		printf(" >\n");
		i++;
	} 

	return 0;
}