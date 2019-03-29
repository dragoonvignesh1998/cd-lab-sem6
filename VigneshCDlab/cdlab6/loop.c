#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lex.yy.c"

struct token *tk;
struct token* getNextToken() {
	struct token* tk;
	if(!(tk = yylex()))
		tk = NULL;
	return tk;
}
int expn();
int simple_expn();
int eprime();
int seprime();
int term();
int factor();
int tprime();
int relop();
int addop();
int mulop();
int stmt_list();
int stmt();
int assign_stmt();
int dec_stmt();
int dprime();



int lp_stmt(){
	if(tk && strcmp(tk->lexeme,"while")==0) {
		tk=getNextToken();
		if(tk && strcmp(tk->lexeme,"(")==0) {
			tk=getNextToken();
			expn();
			if(tk && strcmp(tk->lexeme,")")==0) {
				tk=getNextToken();
				if(tk && strcmp(tk->lexeme,"{")==0) {
					tk=getNextToken();
					stmt_list();
					if(tk && strcmp(tk->lexeme,"}")==0) {
						tk=getNextToken();
						return;	
					}
					else {
						printf("Error\tExpected: }\n");
						exit(0);
					}
				}
				else {
					printf("Error\t{\n");
					exit(0);
				}
			}
			else {
				printf("Error\tExpected: )\n");
				exit(0);
			}
		}
		else {
			printf("Error\tExpected: (\n");
			exit(0);
		}
	}
	else if(tk && strcmp(tk->lexeme,"for")==0) {
		tk=getNextToken();
		if(tk && strcmp(tk->lexeme,"(")==0) {
			tk=getNextToken();
			assign_stmt();
			if(tk && strcmp(tk->lexeme,";")==0) {
				tk=getNextToken();
				expn();
				if(tk && strcmp(tk->lexeme,";")==0) {
					tk=getNextToken();
					assign_stmt();
					if(tk && strcmp(tk->lexeme,")")==0) {
						tk=getNextToken();
						if(tk && strcmp(tk->lexeme,"{")==0) {
							tk=getNextToken();
							stmt_list();
							if(tk && strcmp(tk->lexeme,"}")==0) {
								tk=getNextToken();
								return;	
							}
							else {
								printf("Error\tExpected: }\n");
								exit(0);
							}
						}
						else {
							printf("Error in line %d col %d:Expected: {\n",tk->r,(tk->c-strlen(tk->lexeme)));
							exit(0);
						}
					}
					else {
						printf("Error in line %d col %d:Expected: )\n",tk->r,(tk->c-strlen(tk->lexeme)));
						exit(0);
					}
				}
				else {
					printf("Error in line %d col %d:Expected: ;\n",tk->r,(tk->c-strlen(tk->lexeme)));
					exit(0);
				}
			}
			else {
				printf("Error\tExpected: ;\n");
				exit(0);
			}
		}
		else {
			printf("Error\tExpected: (\n");
			exit(0);
		}
	}
	else {
		printf("Error\tExpected: for or while\n");
		exit(0);
	}
	return;
}

int expn(){
	simple_expn();
	eprime();
	return;
}

int simple_expn() {
	term();
	seprime();
	return;
}

int eprime() {
	if(tk && (strcmp(tk->lexeme,"==")==0 || strcmp(tk->lexeme,"!=")==0 || strcmp(tk->lexeme,"<=")==0 || strcmp(tk->lexeme,">=")==0 || strcmp(tk->lexeme,">")==0 || strcmp(tk->lexeme,"<")==0)) {
		tk=getNextToken();
		simple_expn();
	}
	else 
		return;
}

int seprime() {
	if((tk && (strcmp(tk->lexeme,"+")==0)) || (tk && strcmp(tk->lexeme,"-")==0)) {
		tk=getNextToken();
		term();
		seprime();
	}
	else 
		return;
	return;
}

int term() {
	if((tk && strcmp(tk->type,"id")==0) || (tk && strcmp(tk->type,"num")==0)) {
		tk=getNextToken();
		tprime();
	}
	else {
		printf("Error in line %d col %d:Expected: id or num\n",tk->r,(tk->c-strlen(tk->lexeme)));
		exit(0);
	}
	return;
}

int factor() {
	if((tk && strcmp(tk->type,"id")==0) || (tk && strcmp(tk->type,"num")==0)) {
		tk=getNextToken();
	}
	else {
		printf("Error in line %d col %d:Expected: id or num\n",tk->r,(tk->c-strlen(tk->lexeme)));
		exit(0);
	}
	return;
}

int tprime() {
	if((tk && strcmp(tk->lexeme,"*")==0) || (tk && strcmp(tk->lexeme,"/")==0) || (tk && strcmp(tk->lexeme,"%")==0)) {
		tk=getNextToken();
		factor();
		tprime();
	}
	else 
		return;
	return;
}

int relop() {
	if(tk && (strcmp(tk->lexeme,"==")==0 || strcmp(tk->lexeme,"!=")==0 || strcmp(tk->lexeme,"<=")==0 || strcmp(tk->lexeme,">=")==0 || strcmp(tk->lexeme,">")==0 || strcmp(tk->lexeme,"<")==0)) {
		tk=getNextToken();
	}
	return;
}

int addop() {
	if((tk && strcmp(tk->lexeme,"+")==0) || (tk && strcmp(tk->lexeme,"-")==0)) {
		tk=getNextToken();
	}
	return;
}

int mulop() {
	if((tk && strcmp(tk->lexeme,"*")==0) || (tk && strcmp(tk->lexeme,"/")==0) || (tk && strcmp(tk->lexeme,"%")==0)) {
		tk=getNextToken();
	}
	return;
}

int stmt_list() {
	if(tk && ( (strcmp(tk->lexeme,"if")==0) || (strcmp(tk->lexeme,"for")==0) || (strcmp(tk->lexeme,"while")==0) || (strcmp(tk->type,"id")==0) )) {
		stmt();
		stmt_list();
	}
	else
		return;
}

int stmt() {
	if(tk && (strcmp(tk->type,"id")==0)) {
		assign_stmt();
		if(tk &&(strcmp(tk->lexeme,";")==0)) {
			tk=getNextToken();
		}
		return;
	}
	else if(tk && (strcmp(tk->lexeme,"if")==0)) {
		dec_stmt();
	}
	else if(tk && ((strcmp(tk->lexeme,"for")==0) || (strcmp(tk->lexeme,"while")==0))) {
		lp_stmt();
	}
	else 
		return;
}

int assign_stmt() {
	if(tk && (strcmp(tk->type,"id")==0)) {
		tk=getNextToken();
		if(tk && (strcmp(tk->lexeme,"=")==0)) {
			tk=getNextToken();
			expn();
		}
		else {
			printf("Error in line %d col %d:Expected: =\n",tk->r,(tk->c-strlen(tk->lexeme)));
			exit(0);
		}
	}
	else {
		printf("Error in line %d col %d:Expected: id\n",tk->r,(tk->c-strlen(tk->lexeme)));
		exit(0);
	}
	return;
}

int dec_stmt() {
	if(tk && strcmp(tk->lexeme,"if")==0) {
		tk=getNextToken();
		if(tk && strcmp(tk->lexeme,"(")==0) {
			tk=getNextToken();
			expn();
			if(tk && strcmp(tk->lexeme,")")==0) {
				tk=getNextToken();
				if(tk && strcmp(tk->lexeme,"{")==0) {
					tk=getNextToken();
					stmt_list();
					if(tk && strcmp(tk->lexeme,"}")==0) {
						tk=getNextToken();
						dprime();
					}
					else {
						printf("Error in line %d col %d:Expected: }\n",tk->r,(tk->c-strlen(tk->lexeme)));
						exit(0);
					}
				}
				else {
					printf("Error in line %d col %d:Expected: {\n",tk->r,(tk->c-strlen(tk->lexeme)));
					exit(0);
				}
			}
			else {
				printf("Error in line %d col %d:Expected: )\n",tk->r,(tk->c-strlen(tk->lexeme)));
				exit(0);
			}
		}
		else {
			printf("Error in line %d col %d:Expected: (\n",tk->r,(tk->c-strlen(tk->lexeme)));
			exit(0);
		}
	}
	else {
		printf("Error in line %d col %d:Expected: if\n",tk->r,(tk->c-strlen(tk->lexeme)));
		exit(0);
	}
}

int dprime() {
	if(tk && (strcmp(tk->lexeme,"else")==0)) {
		tk=getNextToken();
		if(tk && strcmp(tk->lexeme,"{")==0) {
			tk=getNextToken();
			stmt_list();
			if(tk && strcmp(tk->lexeme,"}")==0) {
				tk=getNextToken();
				return;
			}
			else {
				printf("Error in line %d col %d:Expected: }\n",tk->r,(tk->c-strlen(tk->lexeme)));
				exit(0);
			}
		}
		else {
			printf("Error in line %d col %d:Expected: {\n",tk->r,(tk->c-strlen(tk->lexeme)));
			exit(0);
		}
	}
	else
		return;
}
int main() {
	yyin = fopen("input.c", "r");
	tk = getNextToken();
	lp_stmt();
	printf("Successfully parsed\n" );
}




