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
	int r;
};
typedef struct tree* node;
node root=NULL;

// A function to create and return
//	 node with default values	
node makeNode()
{
	int i;
	node temp;
	temp=(node)malloc(sizeof(struct tree));
	for(i=0;i<4;i++)
		temp->d[i]=0;
	for(i=0;i<5;i++)
		temp->ptr[i]=NULL;
	temp->num=0;
	temp->leaf=1;
	temp->p=temp;
	temp->r=0;
	return temp;
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

// A function to insert in the leaf node
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
	return cur;
}

// A function to insert first element of child to parent
void insertParent(node cur,int e,node sib)
{
	node temp,t1,t2;
	int i,j;
	temp=cur->p;
	// if root has to be split
	if(temp->r==1 && temp->num==4 && temp==cur)
	{	
		temp=makeNode();
		temp->ptr[0]=cur;
		temp->ptr[1]=sib;
		cur->p=temp;
		sib->p=temp;
		temp->d[0]=e; 
		temp->leaf=0;
		temp->r=1;
		(temp->num)++;
		root=temp;
		cur->r=0; 
		cur->num=cur->num-2;
	}
	// If root is not full or any other node
	else
	{
		cur->num=cur->num-2;
		//If not full insert first element of child
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
		// If the node is full then  split
	 	else if(temp->num==4)
		{
			//t2 is the sibling
			t2=makeNode();
			for(i=0;i<1;i++) 
			{
				t2->d[i]=temp->d[i+3];
				t2->ptr[i]=temp->ptr[i+3];
				temp->ptr[i+3]=NULL;
				temp->d[i+3]=0;
				(t2->num)++;
			}
			t2->ptr[i]=temp->ptr[i+3];
			temp->ptr[i+3]=NULL;
			t2->ptr[i+1]=sib;
			for(i=t2->num-1;i>=0;i--)
				if(t2->d[i]<e)
					break;
			i++;	
			for(j=t2->num;j>i;j--)
			{
				t2->d[j]=temp->d[j-1];
				t2->ptr[j+1]=t2->ptr[j];	
			}
			t2->ptr[j+1]=NULL;
			t2->d[j]=e;
			(t2->num)++;
			i=temp->d[2];
			temp->d[2]=0;
			insertParent(temp,i,t2);
		}
	} 
}
	


// A function to insert the element to the tree
void insertion(int e)
{	
	int i;
	node cur,sib;
	// If tree is empty
	if(root==NULL)
	{
		root=makeNode();
		root->d[0]=e;
		(root->num)++;
		root->p=root;
		root->r=1;
	}
	else
	{
		cur=find(e);
		//If node is not full
		if(cur->num<4)
			cur=insertLeaf(cur,e);
		//If node is full then split
		else
		{
			sib=makeNode();
			for(i=0;i<2;i++)
			{
				sib->d[i]=cur->d[i+2];
				cur->d[i+2]=0;
//				(cur->num)--;
				(sib->num)++;
			}
			sib=insertLeaf(sib,e);
			i=sib->d[0];
			insertParent(cur,i,sib);	
		}
	}
}

// A function to print the nodes
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
	for(i=0;i<=4;i++)
		print(f,root->ptr[i]);		
}			

// A function to create the output file
// Calls 'print' function to print nodes
// into the output file				
void makeFile()
{
	FILE *fptr;
	fptr=fopen("opb+.dot","w");
	fprintf(fptr,"digraph{\n");
	print(fptr,root);
	fprintf(fptr,"}");
	fclose(fptr);
}		

// Main function
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
