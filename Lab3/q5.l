%{
%}
%%
" and "|" or "|" if "|" but "|" though "|" although " {printf("It is a compound sentence: %s\n",yytext);}
%%
void main()
{
yylex();
}
int yywrap()
{
return 1;
}

