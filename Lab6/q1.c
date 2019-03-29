/*
   for (int i=0;i<n;i=i+1)
   {
   int a[5];
   int i; 
   int j,k;
   char b[20],c[100]; 
   func(); 
   i=i+1; 
   if(b[1]>10)
   {
   j=j+1; 
   }
   else
   {
   printf("hello world"); 
   }
   }$
switch(a)
{
    case 1:
        int a[10]; 
        if(a>10)
        {
            printf("hello world\n");
        }
        else {
            a[0]=999; 
        }
        break;
    case 2:
        int j; 
        for( j=0;j<4;j=j+1)
        {
            a[50]=10;
        }
    default :
        int k[1]=4;
            printf("hello world\n");
        char c='c'; 
        break; 
        
}$
   */
#include<stdio.h>
#include<stdlib.h>
#include "sym_table.h"
#include<string.h>
Token lookahead; 
FILE *fa,*fb; 
void looping_start();
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
void declarations(); 
void switch_block();
void case_blocks();
void case_block();
void default_block();
void data_type(); 
void identifier_list();
void assign_or_func_stat(); 
void func_stat(); 
void program();
void error(Token t,char str[])
{
    int r=t.row;
    int c=t.col;
    printf("error at line %d and col %d with missing %s\n",t.row,t.col,str); 
    exit(1); 
}
void func_stat() 
{
    while(strcmp(lookahead.lexeme,";")!=0)
    {
        lookahead=getNextToken(fa);
        insert_util(lookahead,fa); 
    }
}
void assign_or_func_stat()
{
    //printf("inside func and assign stat\n"); 
    if(lookahead.type==2)
    {
       // printf("inside func and assign stat\n"); 
        lookahead=getNextToken(fa);
        insert_util(lookahead,fa); 
        if(strcmp(lookahead.lexeme,"(")==0)
        {
            func_stat(); 
        }
        else if(strcmp(lookahead.lexeme,"[")==0)
        {
            lookahead=getNextToken(fa);
            insert_util(lookahead,fa); 
            if(lookahead.type==8)
            {
                lookahead=getNextToken(fa);
                insert_util(lookahead,fa); 
                if(strcmp(lookahead.lexeme,"]")==0)
                {
                    lookahead=getNextToken(fa);
                    insert_util(lookahead,fa); 
                    assign_stat(); 
                    return; 
                }
            }
        }
        else
        {
            assign_stat(); 
        }
    }
}
void dprime()
{
    if(strcmp(lookahead.lexeme,"else")==0)
    {
        lookahead=getNextToken(fa);
        insert_util(lookahead,fa); 
        if(strcmp(lookahead.lexeme,"{")==0)
        {
            lookahead=getNextToken(fa);
            insert_util(lookahead,fa); 
            statement_list();
            if(strcmp(lookahead.lexeme,"}")==0)
            {
                lookahead=getNextToken(fa);
                insert_util(lookahead,fa); 
                return; 
            }
            else error(lookahead,"}"); 
        }
        else error(lookahead,"{"); 
    }
    else return; 
}
void declarations()
{
    if(strcmp(lookahead.lexeme,"int")==0 || strcmp(lookahead.lexeme,"char")==0 )
    {
        data_type();
        identifier_list();
        if(strcmp(lookahead.lexeme,";")==0)
        {
            lookahead=getNextToken(fa); 
            insert_util(lookahead,fa); 
            declarations();
            return; 
        }
        else error(lookahead,"; "); 
    }
    else return ; 
}

void data_type()
{

    if(strcmp(lookahead.lexeme,"int")==0 || strcmp(lookahead.lexeme,"char")==0 )
    {
        lookahead=getNextToken(fa); 
        insert_util(lookahead,fa); 
        declarations();

    }
    else error(lookahead,"data type "); 
}
void looping_start()
{
    if(strcmp(lookahead.lexeme,"while")==0)
    {
        lookahead=getNextToken(fa);
        insert_util(lookahead,fa); 
        if(strcmp(lookahead.lexeme,"(")==0)
        {
            lookahead=getNextToken(fa); 
            insert_util(lookahead,fa); 
            expn();
            if(strcmp(lookahead.lexeme,")")==0)
            {
                lookahead=getNextToken(fa);
                insert_util(lookahead,fa); 
                if(strcmp(lookahead.lexeme,"{")==0)
                {
                    lookahead=getNextToken(fa); 
                    insert_util(lookahead,fa); 
                    statement_list();
                    if(strcmp(lookahead.lexeme,"}")==0)
                    {
                        lookahead=getNextToken(fa); 
                        insert_util(lookahead,fa); 
                        return; 
                    }
                    else error(lookahead,"}"); 
                }
                else error(lookahead,"{"); 
            }
            else error(lookahead,")"); 
        }
        else error(lookahead,"("); 
    }
    else if(strcmp(lookahead.lexeme,"for")==0)
    {
        lookahead=getNextToken(fa); 
        insert_util(lookahead,fa); 
        if(strcmp(lookahead.lexeme,"(")==0)
        {
            lookahead=getNextToken(fa); 
            insert_util(lookahead,fa); 
            if(strcmp(lookahead.lexeme,"int")==0 || strcmp(lookahead.lexeme,"char")==0 )
                data_type();
            assign_or_func_stat(); 
            //assign_stat();
            if(strcmp(lookahead.lexeme,";")==0)
            {
                lookahead=getNextToken(fa); 
                insert_util(lookahead,fa); 
                expn();
                if(strcmp(lookahead.lexeme,";")==0)
                {
                    lookahead=getNextToken(fa); 
                    insert_util(lookahead,fa); 
                    assign_or_func_stat(); 
                    //assign_stat();
                    //        assign_stat();
                    if(strcmp(lookahead.lexeme,")")==0)
                    {
                        lookahead=getNextToken(fa); 
                        insert_util(lookahead,fa); 
                        if(strcmp(lookahead.lexeme,"{")==0)
                        {
                            lookahead=getNextToken(fa);
                            insert_util(lookahead,fa); 
                            statement_list();
                            if(strcmp(lookahead.lexeme,"}")==0)
                            {
                                lookahead=getNextToken(fa);
                                insert_util(lookahead,fa); 
                                return; 

                            }
                            else error(lookahead,"}"); 

                        }
                        else error(lookahead,"{"); 
                    }
                    else error(lookahead,")"); 
                }
                else error(lookahead,"("); 
            }
            else error(lookahead,";"); 
        }
        else error(lookahead,"("); 
    }
    else error(lookahead,"for"); 
}

void assign_stat()
{
    /*
       if(lookahead.type==2)
       {
       lookahead=getNextToken(fa);
       insert_util(lookahead,fa);
       if(strcmp(lookahead.lexeme,"=")==0)
       {
       lookahead=getNextToken(fa);
       insert_util(lookahead,fa);
       expn(); 
       }
       else
       error(lookahead,"="); 
       }
       */
    if(strcmp(lookahead.lexeme,"=")==0)
    {
        lookahead=getNextToken(fa);
        insert_util(lookahead,fa);
        expn(); 
    }
    else
        error(lookahead,"=");
}

void decision_stat()
{
    if(strcmp(lookahead.lexeme,"if")==0)
    {

        lookahead=getNextToken(fa);
        insert_util(lookahead,fa); 
        if(strcmp(lookahead.lexeme,"(")==0)
        {
            lookahead=getNextToken(fa);
            insert_util(lookahead,fa); 
            expn();

            if(strcmp(lookahead.lexeme,")")==0)
            {
                lookahead=getNextToken(fa);
                insert_util(lookahead,fa); 
                if(strcmp(lookahead.lexeme,"{")==0)
                {
                    lookahead=getNextToken(fa);
                    insert_util(lookahead,fa); 
                    statement_list();
                    if(strcmp(lookahead.lexeme,"}")==0)
                    {
                        lookahead=getNextToken(fa);
                        insert_util(lookahead,fa); 
                        dprime(); 
                        return; 
                    }
                    else error(lookahead,"}"); 
                }
                else error(lookahead,"{"); 
            }
            else error(lookahead,")"); 
        }
        else error(lookahead,"("); 
    }
    else error(lookahead,"if"); 
}

void statement_list()
{
    //printf("%s stat_list \n",lookahead.lexeme); 
    if(lookahead.type==2 || strcmp(lookahead.lexeme,"while")==0 || strcmp(lookahead.lexeme,"for")==0 || strcmp(lookahead.lexeme,"if")==0 || strcmp(lookahead.lexeme,"int")==0 || strcmp(lookahead.lexeme,"char")==0)
    {
        statement();
        statement_list();
    }
    else return ; 
}

void statement()
{
    //printf("%s stat \n",lookahead.lexeme); 
    if(strcmp(lookahead.lexeme,"int")==0 || strcmp(lookahead.lexeme,"char")==0)
    {
        printf("%s entering declarations \n",lookahead.lexeme); 
        declarations(); 
    }
    else if(lookahead.type==2)
    {
        //assign_stat();
        assign_or_func_stat(); 
        if(strcmp(lookahead.lexeme,";")==0)
        {
            lookahead=getNextToken(fa);
            insert_util(lookahead,fa); 
            return ; 
        }
    }
    else if(strcmp(lookahead.lexeme,"if")==0)
    {
        decision_stat(); 
    }
    else if(strcmp(lookahead.lexeme,"while")==0 || strcmp(lookahead.lexeme,"for")==0)
    {
        looping_start(); 

    }
    else error(lookahead,"while or for or if"); 
}

void identifier_list()
{
    if(lookahead.type==2)
    {
        lookahead=getNextToken(fa);
        insert_util(lookahead,fa); 

        if(strcmp(lookahead.lexeme,"=")==0)
            assign_stat();

        if(strcmp(lookahead.lexeme,",")==0)
        {
            lookahead=getNextToken(fa);
            insert_util(lookahead,fa); 
            identifier_list();
        }
        else if(strcmp(lookahead.lexeme,"[")==0)
        {
            lookahead=getNextToken(fa);
            insert_util(lookahead,fa); 
            if(lookahead.type==8)
            {
                lookahead=getNextToken(fa);
                insert_util(lookahead,fa); 
                if(strcmp(lookahead.lexeme,"]")==0)
                {
                    lookahead=getNextToken(fa);
                    insert_util(lookahead,fa); 
                    if(strcmp(lookahead.lexeme,"=")==0)
                        assign_stat();
                    if(strcmp(lookahead.lexeme,",")==0)
                    {
                        lookahead=getNextToken(fa);
                        insert_util(lookahead,fa); 
                        identifier_list();
                    }
                    else return ; 
                }
            }
        }
        else return; 

    }
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
    //printf("Lexeme is: %s eprime of type: %d\n",lookahead.lexeme,lookahead.type);
    //printf("Lexeme is: %s eprime\n",lookahead.lexeme);  
    if(lookahead.type==4)
    {
        relop();
        simple_exp();
    }
    else return; 
}
void term()
{
    //lookahead.getNextToken();
    factor();
    tprime(); 

}

void seprime()
{
    //printf("Lexeme is: %s seprime of type: %d\n",lookahead.lexeme,lookahead.type); 
    //printf("Lexeme is: %s \n",lookahead.lexeme);  
    if(lookahead.type==3)
    {
        addop();
        term();
        seprime(); 
    }
    else return; 
}

void tprime()
{
    if(strcmp(lookahead.lexeme,"*")==0 || strcmp(lookahead.lexeme,"/")==0 || strcmp(lookahead.lexeme,"%")==0 ) 
    {
        mulop();
        factor();
        tprime();

    }
    else return ; 
}

void relop()
{
    if(lookahead.type==4)
    {
        lookahead=getNextToken(fa);
        insert_util(lookahead,fa); 
    }
    else
        error(lookahead,"relational operator"); 
}

void addop()
{
    if(strcmp(lookahead.lexeme,"+")==0 || strcmp(lookahead.lexeme,"-")==0 )
    {
        lookahead=getNextToken(fa);
        insert_util(lookahead,fa); 
    }
    else
        error(lookahead,"addition operators"); 
}

void factor()
{
    if(lookahead.type==2)
    {
        lookahead=getNextToken(fa); 
        insert_util(lookahead,fa); 
        if(strcmp(lookahead.lexeme,"[")==0)
        {
            lookahead=getNextToken(fa); 
            insert_util(lookahead,fa); 
            if(lookahead.type==8)
            {
                lookahead=getNextToken(fa); 
                insert_util(lookahead,fa); 
                if(strcmp(lookahead.lexeme,"]")==0)
                {
                    lookahead=getNextToken(fa); 
                    insert_util(lookahead,fa); 
                    return; 

                }
                error(lookahead,"]"); 

            }
            error(lookahead," numeric constant"); 

        }
        return; 

    }
    else if(lookahead.type==8)
    {
        lookahead=getNextToken(fa); 
        insert_util(lookahead,fa); 

    }
    else if(lookahead.type==14)
    {
        lookahead=getNextToken(fa); 
        insert_util(lookahead,fa); 

    }
    else
        error(lookahead,"identfier or numeric constant"); 
}

void mulop()
{
    if(strcmp(lookahead.lexeme,"*")==0 || strcmp(lookahead.lexeme,"/")==0 || strcmp(lookahead.lexeme,"%")==0 ) 
    {
        lookahead=getNextToken(fa); 
        insert_util(lookahead,fa); 
    }
    else
        error(lookahead,"multiplication operator"); 
}

void switch_block()
{
    if(strcmp(lookahead.lexeme,"switch")==0)
    {
        lookahead=getNextToken(fa);
        insert_util(lookahead,fa);
        if(strcmp(lookahead.lexeme,"(")==0)
        {
            lookahead=getNextToken(fa);
            insert_util(lookahead,fa);

            //if(lookahead.type==2)
            //{
            simple_exp(); 
            if(strcmp(lookahead.lexeme,")")==0)
            {
                lookahead=getNextToken(fa);
                insert_util(lookahead,fa);
                if(strcmp(lookahead.lexeme,"{")==0)
                {
                    lookahead=getNextToken(fa);
                    insert_util(lookahead,fa);
                    case_blocks();
                    if(strcmp(lookahead.lexeme,"}")==0)
                    {
                        lookahead=getNextToken(fa);
                        insert_util(lookahead,fa);
                        return ; 
                    }
                    else
                        error(lookahead,"}"); 

                }
                else
                    error(lookahead,"{"); 
            }
            else
                error(lookahead,")"); 
        }
        else
            error(lookahead,"identifier"); 
    }
    else
        error(lookahead,"switch"); 
}

void case_blocks()
{
    if(strcmp(lookahead.lexeme,"default")==0)
    {
        default_block();
    }
    else if(strcmp(lookahead.lexeme,"case")==0)
    {
        case_block(); 
        case_blocks(); 
    }
}

void case_block()
{
    if(strcmp(lookahead.lexeme,"case")==0)
    {
        lookahead=getNextToken(fa);
        insert_util(lookahead,fa);
        if(lookahead.type==8 || lookahead.type==14)
        {
            lookahead=getNextToken(fa);
            printf("%s num \n",lookahead.lexeme); 
            insert_util(lookahead,fa);
            if(strcmp(lookahead.lexeme,":")==0)
            {
                printf("%s : \n",lookahead.lexeme); 
                lookahead=getNextToken(fa);
                insert_util(lookahead,fa);
                statement_list();
                if(strcmp(lookahead.lexeme,"break")==0)
                {
                    lookahead=getNextToken(fa);
                    insert_util(lookahead,fa);
                    if(strcmp(lookahead.lexeme,";")==0)
                    {
                        lookahead=getNextToken(fa);
                        insert_util(lookahead,fa);
                        return; 
                    }
                    else
                        error(lookahead,";"); 
                }
                else return ; 
            }
            else
                error(lookahead,":"); 

        }
        else
            error(lookahead,"identifier"); 
    }
    else
        error(lookahead,"case"); 

}

void default_block()
{
    if(strcmp(lookahead.lexeme,"default")==0)
    {
        lookahead=getNextToken(fa);
        insert_util(lookahead,fa);
        if(strcmp(lookahead.lexeme,":")==0)
        {
            lookahead=getNextToken(fa);
            insert_util(lookahead,fa);
            statement_list();
            if(strcmp(lookahead.lexeme,"break")==0)
            {
                lookahead=getNextToken(fa);
                insert_util(lookahead,fa);
                if(strcmp(lookahead.lexeme,";")==0)
                {
                    lookahead=getNextToken(fa);
                    insert_util(lookahead,fa);
                    return; 
                }
                else
                    error(lookahead,";"); 
            }
            else return ; 
        }
        else
            error(lookahead,":"); 
    }
    else
        error(lookahead,"default"); 
}
void program()
{

    if(strcmp(lookahead.lexeme,"int")==0 || strcmp(lookahead.lexeme,"char")==0)
    {
        declarations(); 
    }


        if(strcmp(lookahead.lexeme,"main")==0)
        {
            lookahead=getNextToken(fa);
            insert_util(lookahead,fa);
            if(strcmp(lookahead.lexeme,"(")==0)
            {
                lookahead=getNextToken(fa);
                insert_util(lookahead,fa);
                if(strcmp(lookahead.lexeme,")")==0)
                {

                    lookahead=getNextToken(fa);
                    insert_util(lookahead,fa);
                    if(strcmp(lookahead.lexeme,"{")==0)
                    {
                        lookahead=getNextToken(fa);
                        insert_util(lookahead,fa);
                        declarations();
                        statement_list();
                        if(strcmp(lookahead.lexeme,"}")==0)
                        {
                            lookahead=getNextToken(fa);
                            insert_util(lookahead,fa);
                            return; 

                        }
                        else error(lookahead,"}"); 
                    }
                    else error(lookahead,"{"); 
                }
                else error(lookahead,")"); 

            }
            else error(lookahead,"("); 
        }

        else error(lookahead,"main"); 

}
int main()
{
    fa = fopen("q1ip.c","r");
    fb = fopen("qi2.c","w");
    discard_headers_comments(fa,fb);

    fa = fopen("qi2.c","r");
    if(fa == NULL){
        printf("Cannot open file \n");
        exit(0);
    }   
    Token t[100];
    lookahead=getNextToken(fa);   
    insert_util(lookahead,fa); 
    looping_start();
    //switch_block();
    //program(); 
    if((strcmp(lookahead.lexeme,"for")==0)||(strcmp(lookahead.lexeme,"while")==0))
        looping_start();
    if(strcmp(lookahead.lexeme,"$")==0)
        printf("Input Accepted\n");
    else
    {
        error(lookahead,"$"); 
    }
    Display(); 
    return 0; 
}
