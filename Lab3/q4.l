%{
int s=0;
int p=0;
%}
%%
scanf {s++; fprintf(yyout,"READ");}
printf {p++; fprintf(yyout,"WRITE");}
%%
void main()
{
yyin=fopen("q4input.c","r");
yyout=fopen("q4output.c","w");
yylex();
printf("The number of scanfs is: %d and the number of printfs is: %d\n",s,p);
}
int yywrap()
{
return 1;
}
