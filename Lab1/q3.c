#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int is_delim(char c)
{
    char delims[11] ={'(',')',' ','#','{','}','[',']','<','>','\n'}; 

    for(int i=0;i<11;i++)
    {
        if(c==delims[i])
            return 1; 
    }
    return 0; 
}

int is_keyword(char str[])
{
    char keywords[100][100]={"while","long","int","char","return","continue","default","do","enum","goto","if","static","union","typedef","switch","struct","short","float","double","const","void","else","unsigned"}; 
    for(int i=0;i<23;i++)
    {
        if(strcmp(str,keywords[i])==0)
            return 1; 
    }
    return 0; 
}
/*
   void output_to_file(FILE *f,char str[],int l_no,int c_no)
   {
   for(int i=0;i<strlen(str);i++)
   {
   putc(str[i],f); 
   }

   putc(' ',f); 
   fprintf(f,"l_no %d c_no %d",l_no,c_no); 
   putc('\n',f); 
   }
   */

int main()
{
    FILE *fa,*fb;
    int ca,cb;
    int line_count=0,col_count=0; 
    fa=fopen("q3i.c","r");
    if(fa==NULL)
    {
        printf("error cant open file\n");
        exit(0); 
    }
    fb=fopen("q3o.txt","w");

    ca=getc(fa);
    char buf[100];

    while(ca!=EOF)
    {
        if(ca=='/')
        {
            cb=getc(fa); 
            if(cb=='/')
            {
                while(ca!='\n') ca=getc(fa); 
                line_count++;
                col_count=0; 
            }
            else if(cb=='*')
            {
                do {
                    while(ca!='*')
                    {
                        ca=getc(fa);
                        if(ca=='\n')
                        {
                            line_count++;
                            col_count=0; 
                        }
                    }
                    ca=getc(fa);

                }while(ca!='/');

            }

        }
        else if(is_delim(ca)==0)
        {
            if(ca=='"')
            {
//                printf("inside quotes %c\n",ca); 
                ca=getc(fa); 
                while(ca!='"') 
                    ca=getc(fa);

            }
            int i=0; 
            while(is_delim(ca)==0)
            {
                buf[i++]=ca;
                ca=getc(fa);
                col_count++;      
                if(ca=='\n')
                {
                    line_count++;
                    col_count=0; 
                }
            }
            if (is_keyword(buf)==1)
            {
                //printf("found keyword %s at l_n %d and c_no %d \n ",buf,line_count,col_count-strlen(buf)); 
                //output_to_file(fb,buf,line_count,col_count);
                for(int j=0;j<=strlen(buf);j++)
                    if(buf[j]>=97 && buf[j]<=122)
                        buf[j]=buf[j]-32; 
                fprintf(fb,"%s %d %d \n",buf,line_count+1,col_count-strlen(buf)); 

            }

            memset(buf,0,100); 

        }

        ca=getc(fa);
        col_count++;      
        if(ca=='\n')
        {
            line_count++;
            col_count=0; 
        }    
    }
}
