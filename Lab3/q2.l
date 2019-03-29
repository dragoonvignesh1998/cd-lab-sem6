%{
int words=0;
int chars=0;
int blanks=0;
int lines=0;
%}
%%
[a-zA-Z]+ {words++;chars+=strlen(yytext);}
\n {chars++;lines++;}			
" " {blanks++;chars++;}
. {chars++;}
%%
void main()
{
yylex();
printf("%d %d %d %d \n",words,chars,blanks,lines);
}
int yywrap()
{
return 1;
}

