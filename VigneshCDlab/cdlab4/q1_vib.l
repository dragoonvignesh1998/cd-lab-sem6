%{
		#include<stdio.h>
	#include<stdlib.h>
	#include<string.h>
	#include "sym_table.c"
	#define YY_DECL Node* yylex(void)

	int lineno=1, colno=1;

	Node * n;

	int ind = 0;
	int cur_func; 

	int flag = 0;

	char sizeOfType(char dtype[]){

		if(strcmp(dtype,"int") == 0 || strcmp(dtype,"float") == 0 || strcmp(dtype,"long") == 0)
			return '4';
		else if(strcmp(dtype,"double") == 0)
			return '8';
		else if (strcmp(dtype,"char") == 0)
			return '1';
	}

	void insertIdentifier(char* str){
		int i = 0, j = 0;
		char dtype[10], name[10];

		while(str[i] != ' '){
			dtype[j++] = str[i];
			i++;
		}
		
		dtype[j] = '\0';
		i++;

		while(str[i] != ';'){
			
			Node* temp = (Node*)malloc(sizeof(Node));
			temp->index = ind++;
			
			int k = 0, l = 0;

			while(str[i] != ',' && str[i] != ';'){
				name[k++] = str[i];
				i++;
			}
			
			name[k] = '\0';
			strcpy(temp->dtype,dtype);
			strcpy(temp->name,name);
			temp->type = 0;
			if(flag == 1){
				temp->scope[0] = 'L';
				strcat(temp->scope,indToString(cur_func));
			}
			else
				temp->scope[0] = 'G';
			
			temp->size = sizeOfType(dtype);
			insert(temp);

			if(str[i] == ';')
				return;

			i++;
		}
	}


	void insertFunction(char* str){
		
			Node* temp = (Node*)malloc(sizeof(Node));
			cur_func = ind;
			temp->index = ind++;

			int num = 0;
			
			char rtype[10], dtype[10], func_name[10], arg_name[10];
			int i = 0, j = 0, k = 0, l = 0, m = 0;
			
			while(str[i] != ' '){
				rtype[j++] = str[i];
				i++;
			
			}
			i++;

			rtype[j] = '\0';
			strcpy(temp->rtype,rtype);
			
			while(str[i] != '('){
				func_name[k++] = str[i];
				i++;
			}

			func_name[k] = '\0';
			strcpy(temp->name,func_name);
			

			i++;

			while(str[i] != ')'){
				
				Node* arg_temp = (Node*)malloc(sizeof(Node));
				arg_temp->index = ind++;
				num++;

				j = 0, l = 0;
				
				while(str[i] != ' '){
					dtype[j++] = str[i];
					i++;
				}

				dtype[j] = '\0';

				i++;
				while(str[i] != ',' && str[i] != ')'){
					arg_name[l++] = str[i];
					i++;
				}

				arg_name[l++] = '\0';


				arg_temp->type = 0;
				strcpy(arg_temp->dtype,dtype);
				strcpy(arg_temp->name,arg_name);
				arg_temp->scope[0] = 'L';
				strcat(arg_temp->scope,indToString(cur_func));
				arg_temp->size = sizeOfType(dtype);
				insert(arg_temp);
				

				while(str[i] == ',' || str[i] == ' ')
					i++;

				temp->argsList[m++] = arg_temp->index;

			}

			temp->type = 1;
			temp->scope[0] = 'G';
			temp->num = num;
			temp->size = '-';
			insert(temp);
	}
%}

dtype "int"|"void"|"char"|"double"|"float"|"long"|"short"
function {dtype}[ ]*[a-zA-Z_][a-zA-Z0-9_]*[ ]*\(.*\)
id {dtype}[ ]*[a-zA-Z_][a-zA-Z0-9_]*.*"\;"
keyword "auto"|"break"|"case"|"char"|"const"|"continue"|"default"|"do"|"double"|"else"|"enum"|"extern"|"float"|"for"|"or"|"goto"|"if"|"int"|"long"|"register"|"return"|"short"|"signed"|"sizeof"|"static"|"struct"|"switch"|"typedef"|"union"|"unsigned"|"void"|"volatile"|"while" 
%%
"/*".*"*/" {	
		int i = 0;
		while (yytext[i]!='\0') 
		{
			if(yytext[i]=='\n')
			{
				lineno++;
				colno=1;
			}
			else
				colno++;
			i++;
		}
	}
"#".*"\n" { lineno++; colno=1;}
"//".*"\n" { lineno++; colno=1;}
(\'(.)\') {colno+=strlen(yytext);}
\".*\" {colno += strlen(yytext);}
\n 	{lineno++; colno=1;}
{keyword} {colno = colno + strlen( yytext);
	}
{function} {insertFunction(yytext);
	colno += strlen(yytext);}
{id} { insertIdentifier(yytext);
	colno += strlen(yytext);}
\{	{flag = 1;}
\}  {flag = 0;}
	
[A-Za-z_]+[0-9a-zA-Z_]*/\( { n = (Node*)malloc(sizeof(Node));
	strcpy(n->name,yytext);
	n->type = 0;
	strcpy(n->dtype,"--");
	n->scope[0] = 'L';
	strcat(n->scope,indToString(cur_func));
	n->index = ind++;
	insert(n);
	n->size = '-';
	colno += strlen(yytext);
	}


"\t" {colno+=4;}
. {colno++;}
%%

int main(int argc, char **argv)
{

	yyin=fopen("qinput.c","r");
	int cnt = 1;

	yylex();
	

	display(); 
	
	return 0;
}

int yywrap() {
	return 1;
}
