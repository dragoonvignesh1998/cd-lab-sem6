%{
	int chars=0;
	int words=0;
	int lines=0;
	int blanks=0;
%}
%%
[a-zA-Z]+ {words++; chars+=strlen(yytext);}
\n {chars++;lines++;}
. {chars++;}
%%
void main(int argc,char **argv)
{
yylex();
printf("%d %d %d %d\n",lines,words,chars,blanks);
}
int yywrap()
{
return 1;
}
