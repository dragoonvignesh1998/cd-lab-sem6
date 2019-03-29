%{
#include <stdio.h>
#include <stdlib.h>
#include "a.tab.h"
%}
%%
\n {}
[0-9]+ {printf("%s\n", yytext); return NUMBER;}
\[ {printf("[\n"); return SO;}
\] {printf("]\n"); return SC;}
"-" {printf("-\n"); return MINUS;}
"+" {printf("+\n"); return PLUS;}
"*" {printf("*\n"); return MUL;}
"/" {printf("/\n"); return DIV;}
"%" {printf("%\n"); return MOD;}
";" {printf(";\n"); return SS;}
"," {printf(",\n"); return CO;}
"=" {printf("=\n"); return EQUAL;}
">" {printf(">\n"); return GT;}
">=" {printf(">=\n"); return GTE;}
"<" {printf("<\n"); return LT;}
"<=" {printf("<=\n"); return LTE;}
"==" {printf("==\n"); return EQ;}
"!=" {printf("!=\n"); return NE;}
"(" {printf("(\n"); return OPEN_BRAC;}
")" {printf(")\n"); return CLOSE_BRAC;}
"{" {printf("{\n"); return OPEN_FLOW;}
"}" {printf("}\n"); return CLOSE_FLOW;}
"int" {printf("%s\n", yytext); return TYPE;}
"char" {printf("%s\n", yytext); return TYPE;}
"main" {printf("%s\n", yytext); return MAIN;}
"while" {printf("%s\n", yytext); return WHILE;}
"for" {printf("%s\n", yytext); return FOR;}
"if" {printf("if\n"); return IF;}
"else" {printf("else\n"); return ELSE;}
[a-zA-Z]+ {printf("%s\n", yytext); return ID;}
%%
int yywrap(){
	return 1;
}

