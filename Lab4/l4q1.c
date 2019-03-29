#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#define TableLength 30 

struct token
{
    char lexeme[1000];
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
    struct ListElement *next;
}; 

struct ListElement *TABLE[TableLength];
/**ListElement* disp1(ListElement* cur)
{
            if(strcmp(cur->t.lexeme,"num1")==0||strcmp(cur->t.lexeme,"a")==0)
                cur->size=40;
            if(strcmp(cur->t.lexeme,"num2")==0||strcmp(cur->t.lexeme,"b")==0)
                cur->size=80;
            return cur;
}**/
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
    return sum%29;
}


void Display()
{
    printf("Hash\tName\tType\tSize\tScope\tArgcount\tRtype\n"); 
    for(int i=0;i<TableLength;i++)
    {
        struct ListElement* cur = TABLE[i];
        while(cur!=NULL)
        {
           // cur=disp1(cur);
           
            printf("%d\t%s\t%s\t%d\t%c\t%d\t\t%s",HASH(cur->t.lexeme),cur->t.lexeme,cur->dtype,cur->size,cur->scope,cur->argcount,cur->rdtype);

            /*if(cur->argcount>0)
                printf("%d",cur->args[1]);
            for(int i=2;i<cur->argcount+1;i++)
                printf(",%d",cur->args[i]);*/
            printf("\n"); 
            
            cur=cur->next; 
        }
    }
}

int SEARCH(char* str)
{

    for(int i=0;i<TableLength;i++)
    {
        struct ListElement* cur = TABLE[i];
        while(cur!=NULL)
        {
            if(strcmp(cur->t.lexeme,str)==0)
            {
               return 1; 
            }
            cur=cur->next; 
        }
    }
    return 0; 
}

int size_of(char str[])
{
    if(strcmp(str,"int")==1)
        return 4;
    else if(strcmp(str,"float")==1)
        return 4;
    else if(strcmp(str,"double")==1)
        return 8;
    else if(strcmp(str,"char")==1)
        return 1;
    return 4;
}

void INSERT(struct token tk,char dtype[],int size,char scope,int argcount,int args[],char rdtype[])
{
    if(SEARCH(tk.lexeme)==1) return; 
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
    cur->next=NULL; 
    for(int i=1;i<argcount+1;i++)
    {
        cur->args[i]=args[i];
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
void go_back_one_step(FILE* fa){

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
/*void rem_brack(FILE *fa,FILE *fb)
{
    int ca,cb;
    ca=getc(fa);
    while(!feof(fa))
    {
        if(ca=='[')
        {
            cb=getc(fa);
            while(cb!=']')
            {
                cb=getc(cb);
            }
            cb=getc(fb);
            putc(cb,fb);
        }
        else
            putc(ca,fb);
        ca=getc(fa);
    }
    fclose(fa);
    fclose(fb);
}
*/     
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

    if(ca== ';' ||ca== ',' ||ca== '#')
    {
        buf[j++] = ca;
        buf[j] = '\0';
        t.type = 6;
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

    char keywords[15][20] = {"int", "if", "else", "char", "do", "while", "float", "continue", "break", "return","for", "do", "const", "typedef"};

    for (int i = 0; i < 15; ++i)
    {
        if(strcmp(buf,keywords[i]) == 0)
            return 1;
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
            col += 4;

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
    else if (ca== '(' ||ca== ')' ||ca== '{' ||ca== '}')
    {
        return get_bracket_token(ca,fa);
    }
    else if(ca== ';' ||ca== '"' ||ca== '#' ||ca== ',')
    {
        return get_special_token(ca,fa);
    }
    else if(isdigit(ca))
    {
        return get_number_token(ca,fa);
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
        t.type = 1;

    else				
        t.type = 2;

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

int main()
{
    Token t[1000]; 

    FILE *fa, *fb;
    fa = fopen("qinp.c","r");
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
    char temp_dtype[100],temp_rdtype[100]; 
    while(!feof(fa))
    {
        t[i]=getNextToken(fa); 
        if(t[i].lexeme[0]=='{') scope_flag=1; 
        if(t[i].lexeme[0]=='}') scope_flag=0; 
        if(scope_flag==1) scope='L';
        else scope='G'; 

        if(is_datatype(t[i].lexeme))
        {
            strcpy(dtype,t[i].lexeme); 
        }

        if(t[i].lexeme[0]==')')
        {
            if(scope=='G')
            INSERT(temp,"FUNC",0,scope,argcount,args,rdtype);
            else if(scope=='L')
            INSERT(temp,"FUNC",0,'\0',0,args,NULL);
            func_flag=0; 
            argcount=0; 
            continue; 
        }


        if(t[i].lexeme[0]==';') dtype[0]='\0'; 
        if(t[i].lexeme[0]==',' && func_flag==1)
        {
            argcount++; 
        }

        if(func_flag==1 && t[i].type==2)
        {
            args[argcount]=HASH(t[i].lexeme); 
          

        }

        if(t[i].type==2)
        {
            int t_flag=0; 
            ct=getc(fa);
            if(ct=='(')
            {
                func_flag=1; 
                t_flag=1; 
                strcpy(rdtype,dtype); 
                strcpy(temp.lexeme,t[i].lexeme); 
                ct=getc(fa);
                if(ct!=')')
                    argcount++;
                go_back_one_step(fa);
            }
            go_back_one_step(fa);
            if(t_flag==1)  
                continue; 
        }

        if(t[i].type==2)
        {
            INSERT(t[i],dtype,size_of(t[i].lexeme),scope,0,NULL,'\0'); 

        }
            
    } 

    Display(); 
    return 0;
}
