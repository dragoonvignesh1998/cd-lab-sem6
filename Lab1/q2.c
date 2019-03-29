#include<stdio.h>
#include<stdlib.h>
#include<string.h>
void main()
{
	FILE *fa,*fb;
	int ca,cb;
	char arr[5];
	fa=fopen("q2inp.c","r");
	if(fa==NULL)
	{
		printf("Cannot open the file. \n");
		exit(0);
	}
	fb=fopen("q2out.c","w");
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
}
