%{
#include<stdio.h>
#include<stdlib.h>
int yylex();
int yyerror();
extern FILE *yyin;
%}
%token INT CHAR ID CO SS NL
%%
stmt: DC NL
    ;
DC: DT IDL SS
    ;
DT: INT
    | CHAR
    ;
IDL: ID
    | ID CO IDL
    ;
%%
int yyerror(char *msg)
{
    printf("Invalid expression\n");
    return 1;            
}
void main()
{
    printf("Enter the expression\n");
    yyin=fopen("in.txt","r");
    do
    {
    if(yyparse())
    {
    printf("\nFailure");
    exit(0);
    }
    }while(!feof(yyin));
    printf("Success");
}
