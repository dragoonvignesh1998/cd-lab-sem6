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
int switch_stmt();
int labeled_stmts();
int const_expn();
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

int switch_stmt(){
	if(tk && strcmp(tk->lexeme,"switch")==0) {
		tk=getNextToken();
		if(tk && strcmp(tk->lexeme,"(")==0) {
			tk=getNextToken();
			expn();
			if(tk && strcmp(tk->lexeme,")")==0) {
				tk=getNextToken();
				if(tk && strcmp(tk->lexeme,"{")==0) {
					tk=getNextToken();
					labeled_stmts();
					if(tk && strcmp(tk->lexeme,"}")==0) {
						tk=getNextToken();
						return 0;	
					}
					else {
						printf("Error in line %d col %d:Expected: }\n",(int)tk->r,(int)(tk->c-strlen(tk->lexeme)));
						exit(0);
					}
				}
				else {
					printf("Error in line %d col %d:Expected: {\n",(int)tk->r,(int)(tk->c-strlen(tk->lexeme)));
					exit(0);
				}
			}
			else {
				printf("Error in line %d col %d:Expected: )\n",(int)tk->r,(int)(tk->c-strlen(tk->lexeme)));
				exit(0);
			}
		}
		else {
			printf("Error in line %d col %d:Expected: (\n",(int)tk->r,(int)(tk->c-strlen(tk->lexeme)));
			exit(0);
		}
	}
}
int labeled_stmts(){
	if(tk && strcmp(tk->lexeme,"case")==0) {
		tk=getNextToken();
		const_expn();
		if(tk && strcmp(tk->lexeme,":")==0) {
			tk=getNextToken();
			stmt_list();
			if(tk && strcmp(tk->lexeme,"break")==0) {
				tk=getNextToken();
				if(tk && strcmp(tk->lexeme,";")==0) {
					tk=getNextToken();
					labeled_stmts();
					
				}
				else {
					printf("Error\tExpected: {\n");
					exit(0);
				}
			}
			else if(tk && (strcmp(tk->lexeme,"case")==0 || strcmp(tk->lexeme,"default")==0  || strcmp(tk->lexeme,"}")==0 )){
				labeled_stmts();

			}
			else {
				printf("Error\tExpected: )\n");
				exit(0);
			}
		}
		else {
			printf("Error\tExpected: :\n");
			exit(0);
		}
	}
	else if(tk && strcmp(tk->lexeme,"default")==0){
		tk=getNextToken();
		if(tk && strcmp(tk->lexeme,":")==0) {
			tk=getNextToken();
			stmt_list();
			if(tk && strcmp(tk->lexeme,"break")==0) {
				tk=getNextToken();
				if(tk && strcmp(tk->lexeme,";")==0) {
					tk=getNextToken();
					labeled_stmts();
					
				}
				else {
					printf("Error\n");
					exit(0);
				}
			}
			else if(tk && (strcmp(tk->lexeme,"}")==0 )){
				labeled_stmts();

			}
			else {
				printf("Error\n");
				exit(0);
			}
		}
		else {
			printf("Error in line %d col %d\n",(int)tk->r,(int)(tk->c-strlen(tk->lexeme)));
			exit(0);
		}
	}
	else if(strcmp(tk->lexeme,"}")==0 ){
		return 0;
	}
	else{
		exit(0);
	}

}
int const_expn(){
	if(tk && strcmp(tk->lexeme,"'")==0){
		tk=getNextToken();
		/*
		char c='a';
		char d='A';
		int flag=0;
		for(int i=0;i<26;i++){
			if(strncmp(tk->lexeme,&c,1)==0 || strncmp(tk->lexeme,&d,1)==0){
				flag=1;
				break;
			}
            c++;
            d++;
		}*/
		int flag=1;
		if(tk && (flag==1 || strlen(tk->lexeme)==1)) {
			tk=getNextToken();
			if(tk && strcmp(tk->lexeme,"'")==0){
				tk=getNextToken();
				return 0;
			}
			else{
				printf("Error in line %d col %d:Expected: ' \n",(int)tk->r,(int)(tk->c-strlen(tk->lexeme)));
				exit(0);
			}
		}
		else {
			printf("Error in line %d col %d:Expected: char\n",(int)tk->r,(int)(tk->c-strlen(tk->lexeme)));
			exit(0);
		}

	}
	else if(tk && isdigit(*(tk->lexeme))==1){
		tk=getNextToken();
		return 0;
	}
	else{
		printf("Error in line %d col %d:Expected: constant expression\n",(int)tk->r,(int)(tk->c-strlen(tk->lexeme)));
		exit(0);
	}
}
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
						return 0;	
					}
					else {
						printf("Error in line %d col %d:Expected: }\n",(int)tk->r,(int)(tk->c-strlen(tk->lexeme)));
						exit(0);
					}
				}
				else {
					printf("Error in line %d col %d:Expected: {\n",(int)tk->r,(int)(tk->c-strlen(tk->lexeme)));
					exit(0);
				}
			}
			else {
				printf("Error in line %d col %d:Expected: )\n",(int)tk->r,(int)(tk->c-strlen(tk->lexeme)));
				exit(0);
			}
		}
		else {
			printf("Error in line %d col %d:Expected: (\n",(int)tk->r,(int)(tk->c-strlen(tk->lexeme)));
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
								return 0;	
							}
							else {
								printf("Error in line %d col %d:Expected: }\n",(int)tk->r,(int)(tk->c-strlen(tk->lexeme)));
								exit(0);
							}
						}
						else {
							printf("Error in line %d col %d:Expected: {\n",(int)tk->r,(int)(tk->c-strlen(tk->lexeme)));
							exit(0);
						}
					}
					else {
						printf("Error in line %d col %d:Expected: )\n",(int)tk->r,(int)(tk->c-strlen(tk->lexeme)));
						exit(0);
					}
				}
				else {
					printf("Error in line %d col %d:Expected: ;\n",(int)tk->r,(int)(tk->c-strlen(tk->lexeme)));
					exit(0);
				}
			}
			else {
				printf("Error in line %d col %d:Expected: ;\n",(int)tk->r,(int)(tk->c-strlen(tk->lexeme)));
				exit(0);
			}
		}
		else {
			printf("Error in line %d col %d:Expected: (\n",(int)tk->r,(int)(tk->c-strlen(tk->lexeme)));
			exit(0);
		}
	}
	else {
		printf("Error in line %d col %d:Expected: for or while\n",(int)tk->r,(int)(tk->c-strlen(tk->lexeme)));
		exit(0);
	}
	return 0;
}

int expn(){
	simple_expn();
	eprime();
	return 0;
}

int simple_expn() {
	term();
	seprime();
	return 0;
}

int eprime() {
	if(tk && (strcmp(tk->lexeme,"==")==0 || strcmp(tk->lexeme,"!=")==0 || strcmp(tk->lexeme,"<=")==0 || strcmp(tk->lexeme,">=")==0 || strcmp(tk->lexeme,">")==0 || strcmp(tk->lexeme,"<")==0)) {
		tk=getNextToken();
		simple_expn();
	}
	else 
		return 0;
}

int seprime() {
	if((tk && (strcmp(tk->lexeme,"+")==0)) || (tk && strcmp(tk->lexeme,"-")==0)) {
		tk=getNextToken();
		term();
		seprime();
	}
	else 
		return 0;
	return 0;
}

int term() {
	if((tk && strcmp(tk->type,"id")==0) || (tk && strcmp(tk->type,"num")==0)) {
		tk=getNextToken();
		tprime();
	}
	else {
		printf("Error in line %d col %d:Expected: id or num\n",(int)tk->r,(int)(tk->c-strlen(tk->lexeme)));
		exit(0);
	}
	return 0;
}

int factor() {
	if((tk && strcmp(tk->type,"id")==0) || (tk && strcmp(tk->type,"num")==0)) {
		tk=getNextToken();
	}
	else {
		printf("Error in line %d col %d:Expected: id or num\n",(int)tk->r,(int)(tk->c-strlen(tk->lexeme)));
		exit(0);
	}
	return 0;
}

int tprime() {
	if((tk && strcmp(tk->lexeme,"*")==0) || (tk && strcmp(tk->lexeme,"/")==0) || (tk && strcmp(tk->lexeme,"%")==0)) {
		tk=getNextToken();
		factor();
		tprime();
	}
	else 
		return 0;
	return 0;
}

int relop() {
	if(tk && (strcmp(tk->lexeme,"==")==0 || strcmp(tk->lexeme,"!=")==0 || strcmp(tk->lexeme,"<=")==0 || strcmp(tk->lexeme,">=")==0 || strcmp(tk->lexeme,">")==0 || strcmp(tk->lexeme,"<")==0)) {
		tk=getNextToken();
	}
	return 0;
}

int addop() {
	if((tk && strcmp(tk->lexeme,"+")==0) || (tk && strcmp(tk->lexeme,"-")==0)) {
		tk=getNextToken();
	}
	return 0;
}

int mulop() {
	if((tk && strcmp(tk->lexeme,"*")==0) || (tk && strcmp(tk->lexeme,"/")==0) || (tk && strcmp(tk->lexeme,"%")==0)) {
		tk=getNextToken();
	}
	return 0;
}

int stmt_list() {
	if(tk && ( (strcmp(tk->lexeme,"if")==0) || (strcmp(tk->lexeme,"for")==0) || (strcmp(tk->lexeme,"while")==0) || (strcmp(tk->type,"id")==0) )) {
		stmt();
		stmt_list();
	}
	else
		return 0;
}

int stmt() {
	if(tk && (strcmp(tk->type,"id")==0)) {
		assign_stmt();
		if(tk &&(strcmp(tk->lexeme,";")==0)) {
			tk=getNextToken();
		}
		return 0;
	}
	else if(tk && (strcmp(tk->lexeme,"if")==0)) {
		dec_stmt();
	}
	else if(tk && ((strcmp(tk->lexeme,"for")==0) || (strcmp(tk->lexeme,"while")==0))) {
		lp_stmt();
	}
	else 
		return 0;
}

int assign_stmt() {
	if(tk && (strcmp(tk->type,"id")==0)) {
		tk=getNextToken();
		if(tk && (strcmp(tk->lexeme,"=")==0)) {
			tk=getNextToken();
			expn();
		}
		else if(tk && ((strcmp(tk->lexeme,"++")==0) || (strcmp(tk->lexeme,"--")==0))) {
			tk=getNextToken();
			return 0;
		}
		else {
			printf("Error in line %d col %d:Expected: =\n",(int)tk->r,(int)(tk->c-strlen(tk->lexeme)));
			exit(0);
		}
	}
	else {
		printf("Error in line %d col %d:Expected: id\n",(int)tk->r,(int)(tk->c-strlen(tk->lexeme)));
		exit(0);
	}
	return 0;
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
						printf("Error in line %d col %d:Expected: }\n",(int)tk->r,(int)(tk->c-strlen(tk->lexeme)));
						exit(0);
					}
				}
				else {
					printf("Error in line %d col %d:Expected: {\n",(int)tk->r,(int)(tk->c-strlen(tk->lexeme)));
					exit(0);
				}
			}
			else {
				printf("Error in line %d col %d:Expected: )\n",(int)tk->r,(int)(tk->c-strlen(tk->lexeme)));
				exit(0);
			}
		}
		else {
			printf("Error in line %d col %d:Expected: (\n",(int)tk->r,(int)(tk->c-strlen(tk->lexeme)));
			exit(0);
		}
	}
	else {
		printf("Error in line %d col %d:Expected: if\n",(int)tk->r,(int)(tk->c-strlen(tk->lexeme)));
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
				return 0;
			}
			else {
				printf("Error in line %d col %d:Expected: }\n",(int)tk->r,(int)(tk->c-strlen(tk->lexeme)));
				exit(0);
			}
		}
		else {
			printf("Error in line %d col %d:Expected: {\n",(int)tk->r,(int)(tk->c-strlen(tk->lexeme)));
			exit(0);
		}
	}
	else
		return 0;
}
int main() {
	yyin = fopen("ip1.c", "r");
	tk = getNextToken();
	switch_stmt();
	printf("Successfully parsed\n" );
	return 0;
}
