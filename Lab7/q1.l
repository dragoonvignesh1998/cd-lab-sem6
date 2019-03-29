%{
#include<stdio.h>
#include<stdlib.h>
#include "q1.tab.h"
%}
%%
\n {return NL;}
"int" {return INT;}
"char" {return CHAR;}
[a-zA-Z]+ {printf("%s\n",yytext);return ID;}
";" {printf("yay");return SS;}
"," {printf("yay");return CO;}
%%
int yywrap()
{
return 1;
}

