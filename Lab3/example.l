%{
	int l=0;
%}
%%
\n { l++; }
%%
int main()
{
	yylex();
	printf("lines %d",l);
}
int yywrap()
{
	return 1;
}
