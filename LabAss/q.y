%{
	#include<stdio.h>
	#include<stdlib.h>
	#include<string.h>
	int yylex();
	int yyerror();
	extern FILE *yyin;
%}
%token SWITCH LB RB LCB RCB CASE NUM COLON BREAK SS ID EQ LSB RSB
%%
switchstatement: SWITCH LB ID RB switchblock;
switchblock: LCB switchlabels RCB;
switchlabels: switchlabels moreswitchlabels
			|
			;
moreswitchlabels: switchlabel moreswitchlabels
				|
				;
switchlabel: CASE NUM COLON assignstat BREAK SS;
assignstat: ID EQ NUM SS;
			| ID EQ ID LSB NUM RSB SS;
			|
			;
%%
int yyerror(char *msg)
{
	printf("Failure:\n");
	return 1;
}
int main()
{
	yyin=fopen("input.txt","r");
	do{
		if(yyparse())
		{
			printf("Failed\n");
			exit(0);
		}
	}while(!feof(yyin));
	printf("Success\n");
	return 0;
}
								

	
