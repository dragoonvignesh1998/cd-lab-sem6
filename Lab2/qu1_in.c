#include<conio.h>
int main()
{
/*program to generate various tokens*/
int a,b;
a=10,b=20; //Assignment- Inititialization
if(a>=b && b<30)
{
    for(int i=1;i<10;i++)
    {
        a=b+1;
    }
}
printf("Value is :%d",a);
printf("a is of int type");
}

