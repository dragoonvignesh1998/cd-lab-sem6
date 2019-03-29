%{
int v=0;
int c=0;
int chars=0;
%}
%%
[aeiouAEIOU] {v++;chars++;}
[a-zA-Z] {c++;chars++;}
%%
int main()
{
yylex();
printf("Total characters: %d\n",chars);
printf("Vowels: %d\n",v);
printf("Consonants: %d\n",c);
}
int yywrap()
{
return 1;
}
