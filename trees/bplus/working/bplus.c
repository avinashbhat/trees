/* A program to implement B+ trees
	Author - AB
	Date - June 23 2016
	References - 
*/

#include<stdio.h>
#include<stdlib.h>

struct tree
{
	int d[4];
	struct tree *ptr[5],*p;
	int leaf;
	int num;
};
typedef struct tree* node;
node root=NULL;

// A function to create a node with default
//	values
node makeNode()
{
	int i;
	node p;
	p=(node)malloc(sizeof(struct tree));
	for(i=0;i<4;i++)
		p->d[i]=0;
	for(i=0;i<5;i++)
		p->ptr[i]=NULL;
	p->num=0;
	p->leaf=1;
	return p;
}

// Returns leaf node where element has to be inserted
node find(int value)
{
	node c=root;
	int i;
	while(!c->leaf)
	{
		i=0;
		while(i<c->d[i])
		{
			i++;
			if(c->d[i]>value)
				break;
		}
		c=c->ptr[i];
	}
	return c;
}

void insertParent(node cur,int e,node sib)
{
	node temp,t1,t2;
	int i,j,f=1;

	// To check if the node to be inserted is root
	if(root->num==cur->num)
		for(i=0;i<root->num;i++)
			if(cur->d[i]!=root->d[i])
			{
				f=0;
				break;
			}
	else if(root->num!=cur->num)
		f=0;	
	//If root 
	// if root has to be split
	if(f==1 && root==cur)
	{	
		temp=makeNode();
		temp->ptr[0]=cur;
		temp->ptr[1]=sib;
		cur->p=temp;
		sib->p=temp;
		temp->d[0]=e;
		temp->leaf=0;
		(temp->num)++;
		root=temp;
	}
	
	else
	{
		temp=cur->p;
		//Check if full
		if(temp->num<4)
		{
			for(i=0;i<temp->num;i++)
			{
				if(e<temp->d[i])
					break;
			}
			for(j=temp->num;j>i;j--)
			{
				temp->ptr[j]=temp->ptr[j-1];
				temp->d[j+1]=temp->d[j];
			}
			temp->d[i]=e;
			temp->ptr[j+1]=sib;
			temp->ptr[j]=cur;
			(temp->num)++;
			sib->p=temp;
			temp->leaf=0;
		}
		else if(temp->num==4)
		{
			t1=makeNode();
			t2=makeNode();
			for(i=0;i<temp->num;i++)
			{
				t1->d[i]=temp->d[i];
				(t1->num)++;
				t1->ptr[i]=temp->ptr[i];
			}
			t1->ptr[i]=temp->ptr[i];
			
			for(i=0;i<t1->num;i++)
				if(e<t1->d[i])
					break;
			for(j=t1->num;j>i;j--)
			{
				t1->d[j]=t1->d[j-1];
				t1->ptr[j+1]=t1->ptr[j];
			}
			t1->d[i]=e;
			(t1->num)++;
			t1->ptr[i+1]=sib;
			t1->ptr[i]=cur;	
			for(i=2;i<=4;i++)
			{
				temp->ptr[i+1]=NULL;
				temp->d[i]=0;
				(temp->num)--;
			}
			(temp->num)++;
			j=t1->d[2];
			for(i=2;i<t1->num;i++)
			{
				t2->d[i-2]=t1->d[i];
				(t2->num)++;
				t2->ptr[i-2]=t1->ptr[i];
				(t2->ptr[i-2])->p=t2;
			}
			t2->ptr[i-2]=t1->ptr[i];
                        (t2->ptr[i-2])->p=t2;
			t2->leaf=0;
			insertParent(temp,j,t2);
		}
	}
}
	

node insertLeaf(node cur,int e)
{
	
	int i,k;
	for(i=cur->num-1;i>=0;i--)
		if(cur->d[i]<e)
			break;
	i++;	
	for(k=cur->num;k>i;k--)
		cur->d[k]=cur->d[k-1];
	cur->d[i]=e;
	(cur->num)++;
}


void insertion(int e)
{	
	int i;
	node cur,sib;
	if(root==NULL)
	{
		root=makeNode();
		root->d[0]=e;
		(root->num)++;
	}
	else
	{
		cur=find(e);
		
		if(cur->num<4)
			cur=insertLeaf(cur,e);
		else
		{
			sib=makeNode();
			for(i=0;i<2;i++)
			{
				sib->d[i]=cur->d[i+2];
				cur->d[i+2]=0;
				(cur->num)--;
				(sib->num)++;
			}
			sib=insertLeaf(sib,e);
			i=sib->d[0];
			insertParent(cur,i,sib);	
		}
	}
}

void print(FILE *f,node root)
{
	int i,j;
	node cur=root;
	if(cur==NULL)
		return;
	
	fprintf(f,"\"");
	for(i=0;i<cur->num;i++)
		fprintf(f,"%d ",root->d[i]);
	fprintf(f,"\"\n");
	i=0;
	while(cur->ptr[i]!=NULL && i<=cur->num)
	{
		fprintf(f,"\"");
		for(j=0;j<cur->num;j++)
			fprintf(f,"%d ",cur->d[j]);
		fprintf(f,"\"->\"");
		for(j=0;j<(cur->ptr[i])->num;j++)
			fprintf(f,"%d ",(cur->ptr[i])->d[j]);
		fprintf(f,"\"\n");
		i++;
	}
	print(f,root->ptr[0]);	
	print(f,root->ptr[1]);		
	print(f,root->ptr[2]);		
	print(f,root->ptr[3]);
	print(f,root->ptr[4]);	
}			
				
void makeFile()
{
	FILE *fptr;
	fptr=fopen("opb+.dot","w");
	fprintf(fptr,"digraph{\n");
	print(fptr,root);
	fprintf(fptr,"}");
	fclose(fptr);
}		

void main()
{
	int i,e;
	while(fscanf(stdin,"%d",&e)==1)
	{
		if(e==EOF)
			break;
		insertion(e);
	}
	makeFile();
}
