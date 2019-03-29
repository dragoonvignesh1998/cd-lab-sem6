#include <stdio.h>
#include <stdlib.h>
#include "qu1.c"
#include <string.h>
#include <ctype.h>
typedef struct Token lookahead;
FILE *fp;
int i=0;
void proc_t();
void proc_e()
{
	lookahead=getNextToken(fp);
	if(strcmp(lookahead.lexemename,"NUMBER")==0)
	{
		lookahead=getNextToken(fp);
		proc_t();
	}
	else
	{
		printf("\nError")''
	}
	if (strcmp(lookahead.lexemename,"EOL")==0)
	{
		printf("\nSuccessful");
	}
	else
		printf("\nError");
}
void proc_t()
{
	lookahead=getNextToken(fp);
	if(strcmp(lookahead.lexemename,"MUL")==0)
	{
		lookahead=getNextToken(fp);
		proc_t();
	}
	else
		printf("Error");
}
int main()
{
	lookahead=getNextToken(fp);
	proc_e();
	return 0;
}
