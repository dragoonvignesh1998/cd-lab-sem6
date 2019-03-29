%{
int pn=0;
int nn=0;
int pf=0;
int nf=0;
%}
%%
\+?[0-9]+ pn++;
\-[0-9]+ nn++;
\+?[0-9]+\.?[0-9]+ pf++;
\-[0-9]+\.?[0-9]+ nf++;
%%
void main()
{
yylex();
printf("Positive Integer: %d \n",pn);
printf("Negative Integer: %d \n",nn);
printf("Positive Floating: %d \n",pf);
printf("Negative Floating: %d \n",nf);
}
int yywrap()
{
return 1;
}
