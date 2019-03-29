/**#include <stdio.h>
#include<string.h>
#include<stdlib.h>

#define ERROR(p) {printf("\n Error %d",p);exit(0);}
#define eq(x) strcmp(l,x)

char l[100];
char input[100][30];
int in=0;

void getNextToken()
{
    strcpy(l,input[in++]);
}

void statement_list()
{
    getNextToken();
    if(!eq("statement;"))
    {
        statement_list();
    }
}
void case_stmt()
{
   // getNextToken();
    if(!eq("case"))
    {
        getNextToken();
        if(!eq("num:"))
        {

            statement_list();
            if(!eq("break"))
            {
                getNextToken();
                case_stmt();
            }else if(!eq("}"))
            {
                return;
            }
            else if(!eq("case"))
            {
                case_stmt();
            }
            else {
                ERROR(8)
            }
        }else ERROR(7)
    }
}

void S()
{
    getNextToken();
    if(!eq("switch"))
    {
        getNextToken();
        if(!eq("("))
        {
            getNextToken();
            if(!eq("expr"))
            {
                getNextToken();
                if(!eq(")"))
                {
                    getNextToken();
                    if(!eq("{"))
                        {
                            getNextToken();
                            case_stmt();
                            if(eq("}"))
                            {
                                printf("Errror76");
                                exit(0);
                            }


                        } else ERROR(5)

                }else ERROR(4)
            }else ERROR(3)
        }else ERROR(2)
    }else ERROR(1)

    getNextToken();
    if(!eq("$"))
        printf("Success\n");
    else
        printf("error");

}


int main()
{
    char arr[100];
    int i=0;
    while(strcmp(arr,"$")!=0)
    {
        scanf("%s",arr);
        strcpy(input[i++],arr);
    }
    S();
    return 0;
}**/
/////
/////
/////
#include <stdio.h>
#include<string.h>
#include<stdlib.h>

#define ERROR(p) {printf("\n Error %d",p);exit(0);}
#define eq(x) strcmp(l,x)

char l[100];
char input[100][30];
int in=0;

void getNextToken()
{
    strcpy(l,input[in++]);
}

void statement_list()
{
    getNextToken();
    if(!eq("statement;"))
    {
        statement_list();
    }
}

void assgn_stat()
{
    getNextToken();
    if(!eq("id"))
    {
        getNextToken();
        if(!eq("="))
        {
        getNextToken();
        if(!eq("expn"))
        {
        getNextToken();
        return;
        }else ERROR(12)
        }else ERROR(11)
    }else ERROR(10)
}

void S()
{
    getNextToken();
    if(!eq("while"))
    {
        getNextToken();
        if(!eq("("))
        {
            getNextToken();
            if(!eq("expn"))
            {
                getNextToken();
                if(!eq(")"))
                {
                    getNextToken();
                    if(!eq("{"))
                        {
                            statement_list();
                            if(eq("}"))
                                ERROR(6)
                        } else ERROR(5)

                }else ERROR(4)
            }else ERROR(3)
        }else ERROR(2)
    }

    else if(!eq("for"))
    {
    getNextToken();

        if(!eq("("))
        {
            assgn_stat();
            if(!eq(";"))
            {
                getNextToken();
                if(!eq("expn"))
                {
                    getNextToken();
                    if(!eq(";"))
                        {
                            assgn_stat();
                            if(!eq(")")){
                                getNextToken();
                                if(!eq("{")){
                                statement_list();
                                if(eq("}"))
                                    ERROR(9)
                            }else ERROR(8)
                            }else ERROR(7)
                        } else ERROR(6)

                }else ERROR(5)
            }else ERROR(4)
        }else ERROR(3)
    }
    else ERROR(1);




    getNextToken();
    if(!eq("$"))
        printf("Success\n");
    else
        printf("error");

}


int main()
{
    char arr[100];
    int i=0;
    while(strcmp(arr,"$")!=0)
    {
        scanf("%s",arr);
        strcpy(input[i++],arr);
    }
    S();
    return 0;
}


