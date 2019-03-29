%{
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define YY_DECL struct token *yylex(void)
enum tokenType {EOFILE=-1,LESS_THAN,LESS_THAN_OR_EQUAL,GREATER_THAN,GREATER_THAN_OR_EQUAL,EQUAL,NOT_EQUAL,KEYWORD,IDENTIFIER,SPECIAL_CHARACTER,UNARY_ARITHMETIC,BINARY_ARITHMETIC,LOGICAL_OPERATOR};
struct token
{
    char *lexeme;
    int index;
    unsigned int rowno,colno;
    enum tokenType type;
};
int lineno=1,colno=1;
struct token *tk;
struct token *allocToken()
{
    struct token *tk;
    tk=(struct token *)malloc(sizeof(struct token));
    tk->lexeme=(char *)malloc(sizeof(char)*3);
    tk->index=-1;
    tk->type=EOFILE;
    return tk;
}
void setTokenArgs(struct token *tk,char *lexeme,int index,int rowno,int colno,enum tokenType type)
{
    if(tk==NULL)
        return;
    strcpy(tk->lexeme,lexeme);
    tk->index=index;
    tk->rowno=rowno;
    tk->colno=colno;
    tk->type=type;
}
%}
%%
("/*".*"*/") {                                                                                                                                                     
    int i=0;
    while(yytext[i]!='\0')
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
"//".*"\n" {lineno++; colno=1;}
(\"(.)*\") {colno+=strlen(yytext);}
(\'(.)\') {colno+=strlen(yytext);}
\n {lineno++;colno=1;}
if|else|then|switch|case|break|for|while|do {tk=allocToken(); setTokenArgs(tk,yytext,-1,lineno,colno,KEYWORD); colno+=strlen(yytext); return tk;}
[a-zA-Z]([a-zA-Z_]|[0-9])* {tk=allocToken(); setTokenArgs(tk,yytext,-1,lineno,colno,IDENTIFIER); colno+=strlen(yytext); return tk;}
";"|","|"#" {tk=allocToken(); setTokenArgs(tk,yytext,-1,lineno,colno,SPECIAL_CHARACTER); colno++; return tk;}
"++"|"--"|"+="|"-=;" {tk=allocToken(); setTokenArgs(tk,yytext,-1,lineno,colno,UNARY_ARITHMETIC); colno+=2; return tk;}
"+"|"-"|"*"|"/" {tk=allocToken(); setTokenArgs(tk,yytext,-1,lineno,colno,BINARY_ARITHMETIC); colno++; return tk;}
"<="|">="
"<" {tk=allocToken(); setTokenArgs(tk,yytext,-1,lineno,colno,LESS_THAN); colno++; return tk;}
"<=" {tk=allocToken(); setTokenArgs(tk,yytext,-1,lineno,colno,LESS_THAN_OR_EQUAL); colno+=2; return tk;}
">" {tk=allocToken(); setTokenArgs(tk,yytext,-1,lineno,colno,GREATER_THAN); colno++; return tk;}
">=" {tk=allocToken(); setTokenArgs(tk,yytext,-1,lineno,colno,GREATER_THAN_OR_EQUAL); colno+=2; return tk;}
"==" {tk=allocToken(); setTokenArgs(tk,yytext,-1,lineno,colno,EQUAL); colno+=2; return tk;}
"!=" {tk=allocToken(); setTokenArgs(tk,yytext,-1,lineno,colno,NOT_EQUAL); colno+=2; return tk;}
"\t" {colno+=8;}
. {colno++;}
%%
main(argc,argv)
int argc;
char **argv;
{
    if(argc<2)
    {
        printf("This program requires the name of a C file\n");
        exit(0);
    }
    yyin=fopen(argv[1],"r");
    int cnt=0;
    while((tk==yylex()))
    {
        printf("%d %d %d %s\n",cnt,tk->rowno,tk->colno,tk->lexeme);
        cnt++;
    }
    return 0;
}
int yywrap()
{
return 1;
}


