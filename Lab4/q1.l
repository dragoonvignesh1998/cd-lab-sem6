%{
	#include<stdio.h>
	#include<stdlib.h>
	#include<string.h>
	#include "sym_table.c"
	#define YY_DECL Node* yylex(void)

	int lineno=1, colno=1;

	Node * n;

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
			
			int k = 0, l = 0;

			while(str[i] != ',' && str[i] != ';'){
				name[k++] = str[i];
				i++;
			}
			
			name[k] = '\0';
			strcpy(temp->dtype,dtype);
			strcpy(temp->name,name);
			temp->type = 0;
			temp->scope = 'L';
			insert(temp);

			if(str[i] == ';')
				return;

			i++;
		}
	}


	void insertFunction(char* str){
		
			Node* temp = (Node*)malloc(sizeof(Node));
			
			char rtype[10], dtype[10], func_name[10], arg_name[10];
			int i = 0, j = 0, k = 0, l = 0;

			
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
		
				if(!search(arg_name)){
					arg_temp->type = 0;
					strcpy(arg_temp->dtype,dtype);
					strcpy(arg_temp->name,arg_name);
					arg_temp->scope = 'L';
					insert(arg_temp);
				}

				while(str[i] == ',' || str[i] == ' ')
					i++;

			}

			temp->type = 1;
			temp->scope = 'N';
			insert(temp);
	}
%}


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
"auto"|"break"|"case"|"char"|"const"|"continue"|"default"|"do"|"double"|"else"|"enum"|"extern"|"float"|"or"|"goto"|"if"|"int"|"long"|"register"|"return"|"short"|"signed"|"sizeof"|"static"|"struct"|"switch"|"typedef"|"union"|"unsigned"|"void"|"volatile"|"while"] {colno = colno + strlen( yytext);
	}
("int"|"void"|"char")[ ]*[a-zA-Z_][a-zA-Z0-9_]*[ ]*\(.*\) {insertFunction(yytext);
	colno += strlen(yytext);}
("int"|"double"|"char")[ ]*[a-zA-Z_][a-zA-Z0-9_]*.*"\;" { insertIdentifier(yytext);
	colno += strlen(yytext);}
	
[A-Za-z_]+[0-9a-zA-Z_]*/\( { n = (Node*)malloc(sizeof(Node));
	strcpy(n->name,yytext);
	n->type = 0;
	strcpy(n->dtype,"--");
	n->scope = 'N';
	insert(n);
	colno += strlen(yytext);
	}


"\t" {colno+=4;}
. {colno++;}
%%

int main(int argc, char **argv)
{

	yyin=fopen("input.c","r");
	int cnt = 1;

	yylex();
	

	display();
	
	return 0;
}

int yywrap() {
	return 1;
}
