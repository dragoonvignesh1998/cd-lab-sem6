%{
#include<stdio.h>
#include<stdlib.h>
int l=0;
%}
%%
\n l++;
%%
int main()
{
yylex();
printf("The lines= %d\n",l);
}
int yywrap()
{
return 1;
}
