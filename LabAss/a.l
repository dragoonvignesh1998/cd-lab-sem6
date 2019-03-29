%{
    #include<stdio.h>
    #include<string.h>
    #include<stdlib.h>
    #include "q.tab.h"
%}
%%
"\n" {}
"switch" {printf("%s\n",yytext); return SWITCH;}
"(" {printf("(\n"); return LB;}
")" {printf(")\n"); return RB;}
"{" {printf("{\n"); return LCB;}
"}" {printf("}\n"); return RCB;}
"case" {printf("%s\n",yytext); return CASE;}
[0-9]+ {printf("%s\n",yytext); return NUM;}
":" {printf(":\n"); return COLON;}
"break" {printf("%s\n",yytext); return BREAK;}
";" {printf(";\n"); return SS;}
[a-zA-Z]+[_a-zA-Z0-9]? {printf("%s\n",yytext); return ID;}
"=" {printf("=\n"); return EQ;}
"[" {printf("[\n"); return LSB;}
"]" {printf("]\n"); return RSB;}
" " {}
%%
int yywrap()
{
    return 1;
}

