%{
	#include <stdio.h>
	#include <stdlib.h>
	int yylex();
	int yyerror();
	extern FILE *yyin;
%}
%token NL ID NUMBER IF GT GTE LT LT_EQUAL EQUALS ELSE NOT_EQUALS SS CO OPEN_FLOW CLOSE_FLOW OPEN_BRAC CLOSE_BRAC MAIN TYPE SO SC PLUS MINUS MUL DIV MOD WHILE FOR EQUAL

%%
program: MAIN OPEN_BRAC CLOSE_BRAC OPEN_FLOW declarations statement_list CLOSE_FLOW
	;
declarations: TYPE identifier_list SS declarations
	|
	;
identifier_list: ID
	| ID CO identifier_list
	| ID SO NUMBER SC CO identifier_list
	| ID SO NUMBER SC
	;
statement_list: statement statement_list
	|
	;
statement: assign_stat SS
	| decision_stat
	| looping_stat
	;
assign_stat: ID EQUAL expn
	;
expn: simple_expn eprime
	;
eprime: relop simple_expn
	|
	;
simple_expn: term seprime
	;
seprime: addop term seprime
	|
	;
term: factor tprime
	;
tprime: mulop factor tprime
	|
	;
factor: ID
	| NUMBER
	;
decision_stat: IF OPEN_BRAC expn CLOSE_BRAC OPEN_FLOW statement_list CLOSE_FLOW dprime
	;
dprime: ELSE OPEN_FLOW statement_list CLOSE_FLOW
	| 
	;
looping_stat: WHILE OPEN_BRAC expn CLOSE_BRAC OPEN_FLOW statement_list CLOSE_FLOW
	| FOR OPEN_BRAC assign_stat SS expn SS assign_stat CLOSE_BRAC OPEN_FLOW statement_list CLOSE_FLOW
	;
relop: GT
	| GTE
	| LT
	| LT_EQUAL
	| EQUALS
	| NOT_EQUALS
	;
addop: PLUS
	| MINUS
	;
mulop: MUL
	| DIV
	| MOD
	;
%%
int yyerror(char *msg){
	printf("Invalid Expression\n");
	return 1;
}
void main(){
	yyin = fopen("in.txt", "r");
	do{
		if(yyparse()){
			printf("\n Failure");
			exit(0);
		}
	}while(!feof(yyin));
	printf("success");
}

