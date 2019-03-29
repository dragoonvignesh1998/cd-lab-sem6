%{
	#include<stdio.h>
	#include<string.h>
	#include<stdlib.h>
	#define YY_DECL struct token *yylex(void)
	struct token {
		char* lexeme;
		char* type;
		int r,c;
	};
	int row=1,col=1;
	struct token *tk;
	struct token* allocToken() {
		struct token* tk=(struct token*)malloc(sizeof(struct token));
		tk->lexeme=(char*)calloc(20,sizeof(char));
		tk->type=(char*)calloc(20,sizeof(char));
		return tk;
	}
	void setTokenArgs(struct token* tk,char* lexeme,char* type,int r,int c) {
		if(tk==NULL)
			return;
		strcpy(tk->lexeme,lexeme);
		strcpy(tk->type,type);
		tk->r=r;
		tk->c=c;
	}
%}
%%

"/*".(.|\n)*"*/" {
	int i=0;
	while(yytext[i]!='\0') {
		if(yytext[i]=='\n') {
			row++;
			col=1;
		}
		else
			col++;
		i++;
	}
}

"#"(.)*"\n" {row++; col=1;}

"//"(.)*"\n" {row++; col=1;} 

\n {row++; col=1;}

(\"(.)*\") {
	tk=allocToken();
	col+=strlen(yytext);
	setTokenArgs(tk,yytext,"STRING LITERAL",row,col);
	return tk;
}

[:?;,\(\)\[\]\{\}] {
	tk=allocToken();
	col++;
	setTokenArgs(tk,yytext,"SPECIAL SYMBOL",row,col);
	return tk;
}

"!"|"&&"|"||"|"|"|"&" {
	tk=allocToken();
	col+=strlen(yytext);
	setTokenArgs(tk,yytext,"LOGICAL OPERATOR",row,col);
	return tk;
}

"<"|">"|"=="|"!="|"<="|">=" {
	tk=allocToken();
	col+=strlen(yytext);
	setTokenArgs(tk,yytext,"RELATIONAL OPERATOR",row,col);
	return tk;
}

= {
	tk=allocToken();
	col+=strlen(yytext);
	setTokenArgs(tk,yytext,"ASSIGNMENT OPERATOR",row,col);
	return tk;
}

([-]?[0-9]+[.]?[0-9]*) {
	tk=allocToken();
	col+=strlen(yytext);
	setTokenArgs(tk,yytext,"num",row,col);
	return tk;
}

"main"|"NULL"|"int"|"fopen"|"FILE"|"printf"|"exit"|"getc"|"putc"|"while"|"do"|"fclose"|"return"|"break"|"if"|"else"|for {
	tk=allocToken();
	col+=strlen(yytext);
	setTokenArgs(tk,yytext,"KEYWORD",row,col);
	return tk;
}


"+"|"-"|"*"|"/"|"%"|"^"|"++"|"+="|"-="|"*="|"/=" {
	tk=allocToken();
	col+=strlen(yytext);
	setTokenArgs(tk,yytext,"ARITHMETIC OPERATOR",row,col);
	return tk;
}

"--" {
	tk=allocToken();
	col+=strlen(yytext);
	setTokenArgs(tk,yytext,"ARITHMETIC OPERATOR",row,col);
	return tk;
}

 

([a-zA-Z]+)([0-9a-zA-Z])* {
	tk=allocToken();
	col+=strlen(yytext);
	setTokenArgs(tk,yytext,"id",row,col);
	return tk;
}

"\t" {col+=4;}

. {col++;}

%%

int yywrap() {
	return 1;
}