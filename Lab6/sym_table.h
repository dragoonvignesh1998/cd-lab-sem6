#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#define TableLength 30 

struct token
{
    char lexeme[100];
    int index;
    unsigned int row,col;
    int type; 
}; 

typedef struct token Token; 

struct ListElement{
    struct token t;
    char dtype[100]; 
    int size;
    char scope;
    int argcount;
    int args[10]; 
    char rdtype[100]; 
    int scope_counter; 
    struct ListElement *next;
}; 

struct ListElement *TABLE[TableLength];

void Initialize()
{
    for(int i=0;i<TableLength;i++)
        TABLE[i]=NULL;
}

int HASH(char* str)
{
    int sum=0;
    for(int i=0;i<strlen(str);i++)
        sum+=(str[i]*(i+1));
    //printf("hash of %s is %d\n",str,sum%29); 
    return sum%29;
}


void Display()
{
    //printf("Hash-name-type-size-scope-argcount-return type-argument\n"); 
    for(int i=0;i<TableLength;i++)
    {
        struct ListElement* cur = TABLE[i];
        while(cur!=NULL)
        {
            printf("%d-%s-%s-%d-%c -%d-%s-",HASH(cur->t.lexeme),cur->t.lexeme,cur->dtype,cur->size,cur->scope,cur->argcount,cur->rdtype);

            if(cur->argcount>0)
                printf("%d",cur->args[1]);
            for(int i=2;i<cur->argcount+1;i++)
                printf(",%d",cur->args[i]);
            printf("\n"); 

            cur=cur->next; 
        }
    }
}

int SEARCH(char* str,int scope_counter,char dtype[])
{

    for(int i=0;i<TableLength;i++)
    {
        struct ListElement* cur = TABLE[i];
        while(cur!=NULL)
        {
            if(strcmp(cur->t.lexeme,str)==0 && scope_counter==cur->scope_counter && strcmp(cur->dtype,dtype)==0)
            {
                printf("lexeme found %s \n",cur->t.lexeme);
                return 1; 

            }
            cur=cur->next; 
        }
    }
    return 0; 
}

int size_of(char str[])
{
    if(strcmp(str,"int")==0)
        return 4;
    else if(strcmp(str,"float")==0)
        return 4;
    else if(strcmp(str,"double")==0)
        return 8;
    else if(strcmp(str,"char")==0)
        return 1;
    return 4;
}

void INSERT(struct token tk,char dtype[],int size,char scope,int argcount,int args[],char rdtype[],int scope_counter)
{
    if(dtype[0]=='\0') 
    {
        printf("Variable %s \n",tk.lexeme);
        return ; 
    }
    if(scope=='\0')
    {
        printf("no scope \n");
        return; 
    }
    if(SEARCH(tk.lexeme,scope_counter,dtype)==1) return; 
    int val=HASH(tk.lexeme);
    struct ListElement* cur= (struct ListElement*)malloc(sizeof(struct ListElement));
    cur->t=tk;
    if(dtype!=NULL)
        strcpy(cur->dtype,dtype); 
    if(rdtype!=NULL)
        strcpy(cur->rdtype,rdtype); 
    cur->size=size; 
    cur->scope=scope; 
    cur->argcount=argcount; 
    cur->scope_counter=scope_counter; 
    cur->next=NULL; 
    for(int i=1;i<argcount+1;i++)
    {
        cur->args[i]=args[i];
        //        printf("arg[%d] is %d\n",i,args[i]); 

    }
    if(TABLE[val]==NULL)
        TABLE[val]=cur;
    else
    {
        struct ListElement* ele= TABLE[val];
        while(ele->next!=NULL)
            ele= ele->next; 
        ele->next =cur; 
    }
}
int row = 1, col = 1;


void go_back_one_step(FILE* fa){		//Move file pointer one step back

    fseek(fa,-1,SEEK_CUR);
    col--;
}

void discard_headers_comments(FILE* fa,FILE* fb){


    char ca, cb;
    if(fa == NULL){
        printf("Cannot open file \n");
        exit(0);
    }


    ca= getc(fa);

    while(!feof(fa)){

        if(ca== '#')
        {
            do
            {
                ca= getc(fa);
            } while (ca!= '\n');
        }

        if(ca== '"'){
            do{
                putc(ca,fb);
                ca= getc(fa);
            }while(ca!= '"');
        }

        if(ca== '/'){
            cb = getc(fa);

            if(cb == '/'){			
                while(cb != '\n'){
                    cb = getc(fa);
                }

                putc(cb,fb);
            }

            else if(cb == '*'){	

                do{
                    while(ca!= '*'){
                        ca= getc(fa);
                    }
                    ca= getc(fa);
                } while(ca!= '/');
            }
        }

        else
            putc(ca,fb);


        ca= getc(fa);
    }

    fclose(fa);
    fclose(fb);

}

Token get_arithmetic_token (char ca, FILE* fa)
{
    Token t;
    char buf[10];
    int j = 0;

    if(ca== '+'){

        buf[j++] = ca;
        ca= getc(fa);
        col++;

        if(ca== '+')	
        {
            buf[j++] = ca;
            buf[j] = '\0';
            t.type = 10;	
        }

        else					
        {
            buf[j]='\0';	
            go_back_one_step(fa);
            t.type = 3;	
        }
    }

    else if(ca== '-'){
        buf[j++] = ca;
        ca= getc(fa);
        col++;

        if(ca== '-')		
        {
            buf[j++] = ca;
            buf[j] = '\0';
            t.type = 10;	
        }

        else
        {
            buf[j]='\0';	
            go_back_one_step(fa);
            t.type = 3;	
        }
    }

    else{
        buf[0] = ca;
        buf[1] = '\0';
        t.type = 3;	
    }

    strcpy(t.lexeme,buf);
    t.row = row;
    t.col = col - strlen(buf);

    return t;
}

Token get_relational_token(char ca, FILE* fa)
{
    Token t;

    int j = 0;
    char buf[10];
    if(ca== '!')
    {
        buf[j++] = ca;
        ca= getc(fa);
        col++;

        if(ca== '=')
        {
            buf[j++] = ca;
            buf[j] = '\0';
        }
        else	
        {					
            buf[j]='\0';
            go_back_one_step(fa);

            strcpy(t.lexeme,buf);
            t.type = 11;
            t.row = row;
            t.col = col - strlen(buf);
            return t;
        }


    }

    if(ca== '=')
    {
        buf[j++] = ca;
        ca= getc(fa);
        col++;

        if(ca== '=')
        {
            buf[j++] = ca;
            buf[j] = '\0';
        }
        else	
        {					
            buf[j]='\0';
            go_back_one_step(fa);

            strcpy(t.lexeme,buf);
            t.type = 11;
            t.row = row;
            t.col = col - strlen(buf);
            return t;
        }


    }

    else if(ca== '<' ||ca== '>')
    {
        buf[j++] = ca;
        ca= getc(fa);
        col++;

        if(ca== '=')
        {
            buf[j++] = ca;
            buf[j] = '\0';
        }

        else
        {
            buf[j]='\0';	
            go_back_one_step(fa);
        }
    }

    strcpy(t.lexeme,buf);
    t.type = 4;
    t.row = row;
    t.col = col - strlen(buf);

    return t;
}

Token get_number_token(char ca,FILE* fa)
{
    Token t;
    char buf[100];
    int j = 0;

    while(isdigit(ca)){
        buf[j++] = ca;
        ca= fgetc(fa);
        col++;
    }

    go_back_one_step(fa);

    buf[j] = '\0';

    strcpy(t.lexeme,buf);
    t.type = 8;
    t.row = row;
    t.col = col - strlen(buf);
    return t;
}

Token get_special_token(char ca, FILE* fa)
{
    Token t;
    char buf[100];
    int j = 0;

    if(ca== ';' ||ca== ',' ||ca== '#' || ca ==':')
    {
        buf[j++] = ca;
        buf[j] = '\0';
        t.type = 6;
    }
    else if(ca=='\'')
    {
        do
        {
            buf[j++] = ca;
            ca=getc(fa);
            col++;

        }while(ca!= '\'');

        buf[j++] = ca;	
        buf[j] = '\0';

        t.type = 14;


    }
    else if (ca== '"')
    {

        do
        {
            buf[j++] = ca;
            ca=getc(fa);
            col++;

        }while(ca!= '"');

        buf[j++] = ca;	
        buf[j] = '\0';

        t.type = 9;
    }

    strcpy(t.lexeme,buf);
    t.row = row;
    t.col = col - strlen(buf);
    return t;
}

Token get_bracket_token(char ca, FILE* fa)
{
    Token t;
    char buf[2];

    buf[0] = ca;
    buf[1] = '\0';

    strcpy(t.lexeme,buf);
    t.type = 12;	
    t.row = row;
    t.col = col - strlen(buf);

    return t;
}


Token get_logical_token(char ca,FILE* fa)
{
    Token t;
    int j = 0;
    char buf[10];

    if(ca== '&')
    {
        buf[j++] = ca;
        ca= getc(fa);
        col++;

        if(ca== '&')
        {
            buf[j++] = ca;
            buf[j] = '\0';
            t.type = 5;
        }
        else{	
            buf[j]='\0';
            go_back_one_step(fa);
            t.type = 7;
        }	
    }

    else if (ca== '|')
    {
        buf[j++] = ca;
        ca= getc(fa);
        col++;

        if(ca== '|')
        {
            buf[j++]=ca;
            buf[j]='\0';
            t.type = 5;
        }

        else{	
            buf[j]='\0';
            go_back_one_step(fa);
            t.type = 7;
        }	
    }

    else if(ca== '!') 
    {
        buf[j++] = ca;
        ca= getc(fa);
        col++;

        if(ca== '=')
        {
            buf[j++]=ca;
            buf[j]='\0';
            t.type = 4;
        }

        else{	
            buf[j]='\0';
            go_back_one_step(fa);
            t.type = 5;
        }	
    }

    strcpy(t.lexeme,buf);
    t.row = row;
    t.col = col - strlen(buf);
    return t;

}


int is_keyword(char buf[]){

    char keywords[17][20] = {"int", "if", "else", "char", "do", "while", "float", "continue", "break", "return","for","while", "do", "const", "typedef","case","default"};

    for (int i = 0; i < 17; ++i)
    {
        if(strcmp(buf,keywords[i]) == 0)
        {
            return 1;
        }
    }

    return 0;
}

int is_valid_char(char ca){

    if(isalnum(ca ) ||ca== '_')
        return 1;

    return 0;
}


Token getNextToken(FILE* fa){

    Token t;

    char ca= fgetc(fa);
    col++;

    if(ca== EOF){
        strcpy(t.lexeme, "EOF");
        t.row = row;
        t.col = col - 1;
        t.type = 13;
        return t;
    }

    while(ca== ' ' ||ca== '\t' ||ca== '\n'){

        if(ca== '\n'){
            row++;
            col = 1;
        }

        else if(ca== '\t')
            col += 4; // or 8 depending on tab size

        ca= getc(fa);
        col++;
    }

    char buf[100];
    int counter = 0;

    if(ca== '+' ||ca== '-' ||ca== '*' ||ca== '/' ||ca== '%')
    {
        return get_arithmetic_token(ca,fa);
    }
    else if (ca== '<' ||ca== '>' ||ca== '=')
    {
        return get_relational_token(ca,fa);
    }
    else if (ca== '&' ||ca== '|' ||ca== '!')
    {
        return get_logical_token(ca,fa);
    }
    else if (ca== '(' ||ca== ')' ||ca== '{' ||ca== '}' || ca=='[' || ca==']')
    {
        return get_bracket_token(ca,fa);
    }
    else if(ca== ';' ||ca== '"' ||ca== '#' ||ca== ',' || ca==':' ||ca=='\'')
    {
        return get_special_token(ca,fa);
    }
    else if(isdigit(ca))
    {
        return get_number_token(ca,fa);
    }
    else if(ca=='$')
    {
        strcpy(t.lexeme, "$");
        t.row = row;
        t.col = col - strlen(buf);

        t.type= 13; 
        return t;

        
    }

    while(ca != ' ' &&ca!= '\n' && is_valid_char(ca) && ca!=EOF){

        if(ca == '\n'){
            row++;
            col = 1;
        }

        else if(ca== '\t')
            col += 4;

        buf[counter++] = ca;
        ca = fgetc(fa);
        col++;
    }

    if(ca!= EOF)
        go_back_one_step(fa);

    buf[counter] = '\0';

    if(ca==EOF)
    {
        strcpy(t.lexeme, "EOF");
        t.row = row;
        t.col = col - strlen(buf);

        t.type= 13; 
        return t;

    }
    else if(is_keyword(buf))
    {
        t.type = 1;
        
    }
    else				
    {
        t.type = 2;

    }

    strcpy(t.lexeme, buf);
    t.row = row;
    t.col = col - strlen(buf);

    return t;

}

void print_token_type(int type){

    switch(type){
        case 1: printf("Keyword");
                break;

        case 2: printf("Identifier");
                break;

        case 3: printf("Arithemtic");
                break;

        case 4: printf("Relational");
                break;

        case 5: printf("Logical");
                break;

        case 6: printf("Special");
                break;

        case 7: printf("Bitwise");
                break;

        case 8: printf("Numeric constant");
                break;

        case 9: printf("String literal");
                break;

        case 10: printf("Unary");
                 break;

        case 11: printf("Assignment");
                 break;

        case 12: printf("Bracket");
                 break;

        case 13: printf("EOF");
                 break;
        case 14: printf("char constant");
                 break;

        default: printf("Unknown");
    }
}

int is_datatype(char str[])
{
    char keywords[15][20] = {"int", "float","double","char"};
    for (int i = 0; i < 4; ++i)
    {
        if(strcmp(str,keywords[i]) == 0)
            return 1;
    }

    return 0;
}
int str_to_int(char str[])
{
    int sum=0;
    for(int i=0;i<strlen(str);i++)
    {
        sum=sum*10+(str[i]-'0');
    }
    return sum; 
}
//Token t[1000]; 
       Token temp; 
int i = 0;
int scope_flag=0; 
char dtype[100],rdtype[100]; 
char ct; 
int argcount=0; 
int arg_hash;
int size; 
char scope; 
int arg_flag=0; 
int func_flag=0; 
int args[100]; 
int ar_flag=0; 
int t_num=1; 
int scope_counter=0; 
char temp_dtype[100],temp_rdtype[100]; 

void insert_util(Token t,FILE* fa)
{
    //printf("lexeme %s \n",t.lexeme); 
    /*
       Token t[1000]; 

       FILE *fa, *fb;
       fa = fopen("qi.c","r");
       fb = fopen("qi2.c","w");
       discard_headers_comments(fa,fb);

       Token temp; 
       fa =fopen("qi2.c","r");

       if (fa == NULL){
       printf("Cannot open file \n");
       exit(0);
       }   

       int i = 0;
       int scope_flag=0; 
       char dtype[100],rdtype[100]; 
       char ct; 
       int argcount=0; 
       int arg_hash;
       int size; 
       char scope; 
       int arg_flag=0; 
       int func_flag=0; 
       int args[100]; 
       int ar_flag=0; 
       int t_num=1; 
       int scope_counter=0; 
       char temp_dtype[100],temp_rdtype[100]; 
       */
    //while(!feof(fa))
    //{
    //t[i]=getNextToken(fa); 
    if(t.lexeme[0]=='{')
    {
        scope_flag=1; 
        if(scope_flag!=1)scope_counter++; 
    }
    if(t.lexeme[0]=='}') scope_flag=0; 
    if(scope_flag==1) scope='L';
    else scope='G'; 

    if(is_datatype(t.lexeme))
    {
        strcpy(dtype,t.lexeme); 
    }

    if(t.lexeme[0]==')')
    {
        if(temp.type!=2)
            return; 
        if(scope=='G')
            INSERT(temp,"FUNC",0,scope,argcount,args,rdtype,scope_counter);
        else if(scope=='L')
            INSERT(temp,"FUNC",0,'\0',0,args,NULL,scope_counter);
        func_flag=0; 
        argcount=0; 
        dtype[0]='\0'; 
        //continue; 
        return ; 
    }
    if(t.lexeme[0]==']')
    {
        printf("%s-%s-%d-%d \n", temp.lexeme,dtype,t_num,size_of(dtype)); 
        INSERT(temp,dtype,size_of(dtype)*t_num,scope,argcount,args,NULL,scope_counter);
        ar_flag=0; 
        //continue; 
        return; 
    }



    if(t.type==8)
    {
        t_num=str_to_int(t.lexeme); 
    }
    if(t.lexeme[0]==';') dtype[0]='\0'; 
    if(t.lexeme[0]==',' && func_flag==1)
    {
        argcount++; 
    }

    if(func_flag==1 && t.type==2)
    {
        args[argcount]=HASH(t.lexeme); 

        //            printf("arg[%d] is %d with lexeme %s\n",argcount,args[argcount],t[i].lexeme); 
    }


    if(t.type==2)
    {
        int t_flag=0; 
        ct=getc(fa);
        if(ct=='(')
        {
            func_flag=1; 
            t_flag=1; 
           strcpy(rdtype,dtype); 
            strcpy(temp.lexeme,t.lexeme); 
            ct=getc(fa);
            if(ct!=')')
                argcount++;
            go_back_one_step(fa);
            dtype[0]='\0'; 
        }
        go_back_one_step(fa);
        if(t_flag==1)  //determines if the current t[i].lexeme is a function and continues accordingly.
            return; 
            //continue; 
    }
    if(t.type==2)
    {
        int t_flag=0; 
        ct=getc(fa);
        if(ct=='[')
        {
            ar_flag=1; 
            t_flag=1; 
            strcpy(rdtype,dtype); 
            strcpy(temp.lexeme,t.lexeme); 
            ct=getc(fa);
            //if(ct!=')')
            //   argcount++;
            go_back_one_step(fa);
        }
        go_back_one_step(fa);
        if(t_flag==1)  //determines if the current t[i].lexeme is a function and continues accordingly.
            //continue; 
            return; 
    }
    if(t.type==2 && dtype!=NULL)
    {
        if(func_flag==1) 
        {
            INSERT(t,dtype,size_of(dtype),'L',0,NULL,NULL,scope_counter); 
        }
        else
        {
            INSERT(t,dtype,size_of(dtype),scope,0,NULL,NULL,scope_counter); 
        }
    }

    //} 

    //Display(); 
    return;
}
