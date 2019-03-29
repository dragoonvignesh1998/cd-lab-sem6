%{
#include<stdio.h>
#include<stdlib.h>
int yylex();
int yyerror();
extern FILE *yyin;
%}
%token NL FOR WHILE IF ELSE EQ ADDOP MULOP RELOP ID NUMBER SL LB RB LCB RCB
%%
stmt: decstmt NL
	;
dec_stmt: IF LB expn RB LCB stmt_list RCB dprime
	;
expn: simple_expn eprime
	;
simple_expn: term seprime
	;
term: factor tprime
	;
factor: ID
	| NUMBER
	;
tprime: MULOP factor tprime
	|
	;
seprime: ADDOP term seprime
	|
	;
stmt_list: stmt stmt_list
	|
	;
stmt: assign_stmt SC
	| dec_stmt
	| looping_stmt
	;
assign_stmt: ID EQ expn
	;							
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
