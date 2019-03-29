#include<stdio.h>
#include<stdlib.h>
void main()
{
	FILE *fa,*fb;
	int ca,cb;
	fa=fopen("spaced.txt","r");
	if(fa==NULL)
	{
		printf("Error opening file");
		exit(0);
	}
	ca=getc(fa);
	fb=fopen("spaced2.txt","w");
	while(ca!=EOF)
	{
		if(ca==' ')
		{
			putc(ca,fb);
			cb=getc(fa);
			while(cb==' ')
				cb=getc(fa);
			//putc(ca,fb);
			putc(cb,fb);
		}
		else if(ca=='\t')
		{
			cb=' ';
			putc(cb,fb);
		}
		else
			putc(ca,fb);
		ca=getc(fa);
	}
	fclose(fa);
	fclose(fb);
}