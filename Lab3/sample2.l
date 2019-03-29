%{
#include<stdio.h>
#include<stdlib.h>

//int sum=a+b;
int sum2=4+5;
int co=0;
%}
%%
[0-9]+ {co++;}
%%
int main()
{
yylex();
//printf("The sum: %d\n",sum);
printf("The sum: %d\n",sum2);
printf("%d\n",co);
}
int yywrap()
{
return 1;
}
