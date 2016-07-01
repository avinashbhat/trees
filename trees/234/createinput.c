#include<stdio.h>
#include<stdlib.h>
#include<time.h>
void main()
{
	FILE *op;
	srand((unsigned)time(NULL));
	int num=50,max=1000,i;
//	scanf("%d%d",&num,&max);
	op=fopen("inputcases.txt","w");
	for(i=1;i<=num;i++)
		fprintf(op,"%d\t",rand()%max+1);
	fclose(op);
}
	
	
