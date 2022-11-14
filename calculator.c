#include<stdio.h>
int add(int a,int b)
{
	return a+b;
}
/*int substraction(inr p ,int q)
{
	return p-q;
}*/
int mult(int a,int b)
{
	return a*b;
}
int divi(int p,int q)
{
	return p/q;
}

int main()
{int a,b;
	printf("enter two numbers");
	scanf("%d %d",&a,&b);
	printf("CALCULATOR\n");
printf("%d\n",add(a,b));
printf("%d\n",mult(a,b));
printf("%d\n",divi(a,b));

	return 0;
}

