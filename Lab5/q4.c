#include<stdio.h>
#include<stdlib.h>
#include"qu1.h"
#include<string.h>
Token lookahead; 

//int i=0; 

FILE *fa,*fb; 
//char fa[100];

void l();
void list();
void ls();
void ls_dash();

void l()
{

    if(lookahead.type==8|| lookahead.type==2)
    {
        lookahead=getNextToken(fa); 
        return; 
    }
    else if(strcmp(lookahead.lexemename,"(")==0)
    {
        list();
    }
    else 
    {
        printf("Not Accepted\n");
        exit(1); 
    }
}
void list()
{
    if(strcmp(lookahead.lexemename,"(")==0)
    {
        lookahead=getNextToken(fa); 
        ls();
        if(strcmp(lookahead.lexemename,")")==0)
        {
            lookahead=getNextToken(fa); 
        }
    else 
    {
        printf("Not Accepted\n");
        exit(1); 
    }
    }
    else 
    {
        printf("Not Accepted\n");
        exit(1); 
    }
}
void ls()
{
    l();
    ls_dash();
}
void ls_dash()
{
    if(lookahead.type==8|| lookahead.type==2 || strcmp(lookahead.lexemename,"(")==0)
    {
       l();
       ls_dash();

    }
    else
    {
        return; 
    }
}
int main()
{
    //fa = fopen("q4i.c","r");
    //fb = fopen("qi2.c","w");
   // rpac(fa,fb);

    fa = fopen("q4i.c","r");
    if(fa == NULL){
        printf("Cannot open file \n");
        exit(0);
    }  
   /** printf("Enter the string: \n"); 
    gets(fa);**/
    lookahead=getNextToken(fa); 
    l(); 
    if(strcmp(lookahead.lexemename,"$")==0)
        printf("Accepted\n");
    else
    {
        printf("Not Accepted\n"); 
        exit(1); 
    }

    return 0; 

}
