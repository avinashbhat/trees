#include<stdio.h>
#include<stdlib.h>
#include<time.h>
void main()
{
	FILE *op;
	srand((unsigned)time(NULL));
	int num=50,max=100,i;
	op=fopen("inputx.txt","w");
	for(i=1;i<=num;i++)
		fprintf(op,"%d\t",rand()%max+1);
	fclose(op);
}
	
	
