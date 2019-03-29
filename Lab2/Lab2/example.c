#include<stdio.h>
#include<string.h>
#include<stdlib.h>
int main()
{
	char c,buf[10];
	FILE *fa=fopen("digit.c","r");
	ca=fgetc(fa);
	if(fa==NULL)
	{
		printf("Cannot open file");
		exit(0);
	}
	while(ca!=EOF)
	{
		int i=0;
		buf[0]='\0';
		if(ca=='=')
		{
			buf[i++]=ca;
			ca=fgetc(fa);
			if(ca=='=')
			{
				buf[i++]=ca;
				buf[i]='\0';
				printf("\nRelational operator: %s",buf);
			}
			else
			{
				buf[i]='\0';
				printf("\nAssignment operator: %s",buf);
			}
		}
		else
		{
			if(ca=='<'||ca=='>'||ca=='!')
			{
				buf[i++]=ca;
				ca=fgetc(fa);
				if(ca=='=')
					buf[i++]=ca;
				buf[i]='\0';
				printf("\nRelational operator: %s",buf);
			}
			else
			{
				buf[i]='\0';
			}
		}
		ca=fgetc(fa);
	}
}