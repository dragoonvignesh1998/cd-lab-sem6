#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int isDigit(char c)
{
	if(c=='0'||c=='1'||c=='2'||c=='3'||c=='4'||c=='5'||c=='6'||c=='7'||c=='8'||c=='9')
		return 1;
	return 0;
}
int isAlpha(char c)
{
	if(c=='a'||c=='b'||c=='c'||c=='d'||c=='e'||c=='f'||c=='g'||c=='h'||c=='i'||c=='j'||c=='k'||c=='l'||c=='m'||c=='n')//fill alphabets
		return 1;
	return 0;
}
int isKeyword(char str[])
{
    char keywords[100][100]={"while","long","int","char","return","continue","default","do","enum","goto","if","static","union","typedef","switch","struct","short","float","double","const","void","else","unsigned"}; 
    for(int i=0;i<23;i++)
    {
        if(strcmp(str,keywords[i])==0)
            return 1; 
    }
    return 0; 
}
void main()
{
	FILE *fa,*fb;
	int ca,cb;
	char arr[5];
	fa=fopen("digit.c","r");
	if(fa==NULL)
	{
		printf("Cannot open the file. \n");
		exit(0);
	}
	fb=fopen("dig1.c","w");
	ca=getc(fa);
	while(ca!=EOF)
	{
		if(ca=='/')
		{
			cb=getc(fa);
			if(cb=='/')
			{
				putc(ca,fb);
				putc(cb,fb);
				while(ca!='\n')
				{
					ca=getc(fa);
					putc(ca,fb);
				}
			}
			else if(cb=='*')
			{
				putc(ca,fb);
				putc(cb,fb);
				do{
					while(ca!='*')
						{
							ca=getc(fa);
							putc(ca,fb);
						}
					ca=getc(fa);
					putc(ca,fb);
				}while(ca!='/');
				putc(ca,fb);
			}
			else
			{
				putc(ca,fb);
				putc(cb,fb);
			}
		}
		else if(ca=='#')
		{
			while(ca!='\n')
				ca=getc(fa);
		}
		else if(ca=='m')
		{
			int i=0;
			while(i<=5)
			{
				putc(ca,fb);
				arr[i++]=ca;
				ca=getc(fa);
			}
			if(strcmp(arr,"main()")==0)
			{
				while(ca!=EOF)
				{
					putc(ca,fb);
					ca=getc(fa);
				}
			}
		}
		else
			putc(ca,fb);
		ca=getc(fa);
	}
	fclose(fa);
	fclose(fb);
	char c,buf[10];
	fa=fopen("dig1.c","r");
	ca=fgetc(fa);
	if(fa==NULL)
	{
		printf("Cannot open file");
		exit(0);
	}
	int ln=1;
	while(ca!=EOF)
	{
		int i=0;
		buf[0]='\0';
		if(ca=='\n')
			ln++;
		else if(ca=='=')
		{
			buf[i++]=ca;
			ca=fgetc(fa);
			if(ca=='=')
			{
				buf[i++]=ca;
				buf[i]='\0';
				printf("Relational operator: %s at line: %d\n",buf,ln);
			}
			else
			{
				buf[i]='\0';
				printf("Assignment operator: %s at line: %d\n",buf,ln);
			}
			if(isDigit(ca)==1)
			{
				buf[i++]=ca;
				ca=getc(fa);
				while(isDigit(ca)==1)
				{
					buf[i++]=ca;
					ca=getc(fa);
				}
				buf[i]='\0';
				buf[0]=' ';
				printf("Numeric Constant:%s at line: %d\n",buf,ln);
			    if(ca=='<'||ca=='>'||ca==';')
				{
					printf("Special symbol: %c at line: %d\n",ca,ln);
				}
			}
		}
		else if(ca=='<'||ca=='>'||ca=='!')
		{
			buf[i++]=ca;
			ca=fgetc(fa);
			if(ca=='=')
				buf[i++]=ca;
			buf[i]='\0';
			printf("Relational operator: %s at line: %d\n",buf,ln);
		}
		else if(ca=='+'||ca=='-'||ca=='*'||ca=='/'||ca=='%')
		{
			buf[i++]=ca;
			ca=fgetc(fa);
			if(ca=='=')
			{
				buf[i++]=ca;
				buf[i]='\0';
				printf("Increment/Decrement operator: %s at line: %d\n",buf,ln);
			}
			else
			{
				buf[i]='\0';
				printf("Arithmetic operator: %s at line: %d\n",buf,ln);
			}
		}
		else if(ca=='&'||ca=='|')
		{
			buf[i++]=ca;
			ca=getc(fa);
			if(ca=='&'||ca=='|'||ca=='=')
			{
				buf[i++]=ca;
				buf[i]='\0';
				printf("Logical operator: %s at line: %d\n",buf,ln);
			}
			else
			{
				buf[i]='\0';
				printf("Logical operator: %s at line: %d\n",buf,ln);	
			}
		}
		else if(ca=='<'||ca=='>'||ca==';'||ca=='#')
		{
			printf("Special symbol: %c at line: %d\n",ca,ln);
		}
		else if(ca=='('||ca=='{'||ca=='[')
			printf("Left bracket: %c at line: %d\n",ca,ln);
		else if(ca==')'||ca=='}'||ca==']')
			printf("Right bracket: %c at line: %d\n",ca,ln);
		//Fill in keywords code here and go into depression
		else if(isDigit(ca)==1)
		{
			buf[i++]=ca;
			ca=getc(fa);
			while(isDigit(ca)==1)
			{
				buf[i++]=ca;
				ca=getc(fa);
			}
			buf[i]='\0';
			printf("Numeric Constant: %s at line: %d\n",buf,ln);
		}
		/**else if(isAlpha(ca)==1)
		{
			buf[i++]=ca;
			ca=getc(fa);
			while(isAlpha(ca)==1)
			{
				buf[i++]=ca;
				ca=getc(fa);
			}
			buf[i]='\0';
			if(isKeyword(buf)==1)
				printf("Keyword: %s at line: %d\n",buf,ln);
		}**/
		ca=fgetc(fa);
	}
}