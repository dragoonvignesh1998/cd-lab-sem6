#include<stdio.h>
#include<stdlib.h>
#include "sym_table.h"
#include<string.h>
Token lookahead; 
FILE *fa,*fb; 
void looping_statement();
void expn();
void assign_stat();
void statement_list();
void statement(); 
void simple_exp(); 
void factor();
void tprime();
void seprime(); 
void term(); 
void eprime(); 
void relop();
void addop();
void mulop(); 
void error(Token t)
{
    printf("Error\n"); 
    printf("Line: %d Column: %d\n",t.row,t.col);
    exit(1); 
}
void looping_statement()
{
    if(strcmp(lookahead.lexeme,"while")==0)
    {
        lookahead=getNextToken(fa);
        insert_into(lookahead,fa); 
        if(strcmp(lookahead.lexeme,"(")==0)
        {
            lookahead=getNextToken(fa); 
            insert_into(lookahead,fa); 
            expn();
            if(strcmp(lookahead.lexeme,")")==0)
            {
                lookahead=getNextToken(fa);
                insert_into(lookahead,fa); 
                if(strcmp(lookahead.lexeme,"{")==0)
                {
                    lookahead=getNextToken(fa); 
                    insert_into(lookahead,fa); 
                    statement_list();
                    if(strcmp(lookahead.lexeme,"}")==0)
                    {
                        lookahead=getNextToken(fa); 
                        insert_into(lookahead,fa); 
                        return; 
                    }
                    else
                        error(lookahead); 
                }
                else
                    error(lookahead); 
            }
            else
                error(lookahead); 
        }
        else
            error(lookahead); 
    }
    else if(strcmp(lookahead.lexeme,"for")==0)
    {
        lookahead=getNextToken(fa); 
        insert_into(lookahead,fa); 
        if(strcmp(lookahead.lexeme,"(")==0)
        {
            lookahead=getNextToken(fa); 
            insert_into(lookahead,fa); 
            assign_stat();
            if(strcmp(lookahead.lexeme,";")==0)
            {
                lookahead=getNextToken(fa); 
                insert_into(lookahead,fa); 
                expn();
                if(strcmp(lookahead.lexeme,";")==0)
                {
                    lookahead=getNextToken(fa); 
                    insert_into(lookahead,fa); 
                    assign_stat();
                    if(strcmp(lookahead.lexeme,")")==0)
                    {
                        lookahead=getNextToken(fa); 
                        insert_into(lookahead,fa); 
                        if(strcmp(lookahead.lexeme,"{")==0)
                        {
                            lookahead=getNextToken(fa);
                            insert_into(lookahead,fa); 
                            statement_list();
                            if(strcmp(lookahead.lexeme,"}")==0)
                            {
                                lookahead=getNextToken(fa);
                                insert_into(lookahead,fa); 
                                return; 
                            }
                            else
                                error(lookahead); 

                        }
                        else
                            error(lookahead); 
                    }
                    else
                        error(lookahead); 
                }
                else
                    error(lookahead); 
            }
            else
                error(lookahead); 
        }
        else
            error(lookahead); 
    }
    else
        error(lookahead); 
}

void assign_stat()
{
    if(lookahead.type==2)
    {
        lookahead=getNextToken(fa);
        insert_into(lookahead,fa);
        if(strcmp(lookahead.lexeme,"=")==0)
        {
            lookahead=getNextToken(fa);
            insert_into(lookahead,fa);
            expn(); 
        }
        else
            error(lookahead); 
    }
}

void statement_list()
{
    if(lookahead.type==2 || strcmp(lookahead.lexeme,"while")==0 || strcmp(lookahead.lexeme,"while")==0)
    {
        statement();
        statement_list();
    }
    else
        return ; 
}

void statement()
{
    if(lookahead.type==2)
    {
        assign_stat();
        if(strcmp(lookahead.lexeme,";")==0)
        {
            lookahead=getNextToken(fa);
            insert_into(lookahead,fa); 
            return ; 
        }
    }
    else if(strcmp(lookahead.lexeme,"while")==0 || strcmp(lookahead.lexeme,"for")==0)
    {
        looping_statement(); 

    }
    else
        error(lookahead); 
}

void expn()
{
    simple_exp(); 
    eprime();
}
void simple_exp()
{

    term();
    seprime();
}
void eprime()
{
    printf("%s eprime %d type\n",lookahead.lexeme,lookahead.type); 
    if(lookahead.type==4)
    {
        relop();
        simple_exp();
    }
    else
        return; 
}
void term()
{
    factor();
    tprime(); 

}
void seprime()
{
    printf("%s seprime %d type\n",lookahead.lexeme,lookahead.type); 
    if(lookahead.type==3)
    {
        addop();
        term();
        seprime(); 
    }
    else
        return; 
}
void tprime()
{
    if(strcmp(lookahead.lexeme,"*")==0 || strcmp(lookahead.lexeme,"/")==0 || strcmp(lookahead.lexeme,"%")==0 ) 
    {
        mulop();
        factor();
        tprime();

    }
    else
        return ; 
}
void relop()
{
    if(lookahead.type==4)
    {
        lookahead=getNextToken(fa);
        insert_into(lookahead,fa); 
    }
    else
        error(lookahead); 
}
void addop()
{
    if(strcmp(lookahead.lexeme,"+")==0 || strcmp(lookahead.lexeme,"-")==0 )
    {
        lookahead=getNextToken(fa);
        insert_into(lookahead,fa); 
    }
    else
        error(lookahead); 
}
void factor()
{
    if(lookahead.type==2 || lookahead.type==8)
    {
        lookahead=getNextToken(fa); 
        insert_into(lookahead,fa); 
    }
    else
        error(lookahead); 
}
void mulop()
{
    if(strcmp(lookahead.lexeme,"*")==0 || strcmp(lookahead.lexeme,"/")==0 || strcmp(lookahead.lexeme,"%")==0 ) 
    {
        lookahead=getNextToken(fa); 
        insert_into(lookahead,fa); 
    }
    else
        error(lookahead); 
}
int main()
{
    fa = fopen("q2i.c","r");
    fb = fopen("qi2.c","w");
    discard_headers_comments(fa,fb);

    fa = fopen("qi2.c","r");
    if(fa == NULL){
        printf("Cannot open file \n");
        exit(0);
    }   
    Token t[100];
    lookahead=getNextToken(fa);   
    insert_into(lookahead,fa); 
    looping_statement(); 
    if(strcmp(lookahead.lexeme,"$")==0)
        printf("Successful\n");
    else
    {
        error(lookahead); 
    }
    Display(); 
    return 0; 
}
