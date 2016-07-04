/* A program to implement B+ trees
	Author - AB
	Date - June 23 2016
	References - Database Systems and Concepts 6th Editon
				by Albert Abraham Silberschatz, 
				Henry F. Korth and S. Sudarshan
				Page Number - 485-499
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
		while(i<c->num)
		{
			if(c->d[i]>value)
				break;
			i++;
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

// A function to split the current node
// into two nodes, current and sibling
// with equal amount of data
node split(node cur,node sib)
{
	int i;
	if(cur->leaf==1)
	{
		for(i=0;i<2;i++)
		{
			sib->d[i]=cur->d[i+2];
			cur->d[i+2]=0;
			(sib->num)++;
			(cur->num)--;
		}
	}
	
	// If the node is any parent node
	// we have to split pointers equally
	
	else
	{
		sib->d[0]=cur->d[3];
		cur->d[2]=0;
		cur->d[3]=0;
		(sib->num)++;
		cur->num=cur->num-2;
		i=0;
		while(i<2)
		{
			sib->ptr[i]=cur->ptr[i+3];
			cur->ptr[i+3]=NULL;
			i++;
		}
	}
	return sib;
}


// A function to insert first element of child to parent
node insertParent(node cur,int e,node sib)
{
	node temp=cur->p,temp2,temp1;
	int i=temp->num,t;
	
	//If the parent node has space
	
	if(temp->num<4)
	{
		while(temp->d[i-1]>e)
		{
			temp->d[i]=temp->d[i-1];
			temp->ptr[i+1]=temp->ptr[i];
			i--;
		}
		temp->d[i]=e;
		(temp->num)++;
		temp->ptr[i+1]=sib;
		sib->p=temp;
	}
	
	// If parent node is full
	
	else
	{
		temp2=makeNode();
		temp2->leaf=0;
		t=temp->d[2];
		temp2=split(temp,temp2);
		
		//If node in which to be inserted is 
		// root node then new root has to be
		// created
		
		if(temp->r==1)
		{
			temp1=makeNode();
			temp->r=0;
			temp1->r=1;
			temp->p=temp1;
			temp2->p=temp1;
			temp1->leaf=0;
			temp1->ptr[0]=temp;
			temp1->ptr[1]=temp2;
			temp1->d[0]=t;
			(temp1->num)++;
			root=temp1;
		}
		else
		{
			temp->p=insertParent(temp,t,temp2);	
		}
		sib->p=temp2;
		sib->p=insertParent(sib,sib->d[0],sib);
		
	}
	return cur->p;
}
	

// A function to insert the element to the tree
node insertion(node cur,int e)
{	
	node sib,temp,r;
	
	// If node is not full
	
	if(cur->num<4)
	{	
		cur=insertLeaf(cur,e);
		return cur;
	}
	
	//If node is full then split
	
	else if(cur->num==4)
	{
		sib=makeNode();
		sib=split(cur,sib);
		if(cur->leaf==1)
		{
			if(e<cur->d[cur->num-1])
				cur=insertLeaf(cur,e);
			else
				sib=insertLeaf(sib,e);
			// If node is root
			
			if(cur->r==1)
			{
				temp=makeNode();
				cur->r=0;
				temp->r=1;
				cur->p=temp;
				sib->p=temp;
				temp->leaf=0;
				temp->ptr[0]=cur;
				temp->ptr[1]=sib;
				temp->d[0]=sib->d[0];
				(temp->num)++;
				root=temp;
				return cur;
			}
			cur->p=insertParent(cur,sib->d[0],sib);
			return cur;
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
	// Prints all data of current node to file
	
	fprintf(f,"\"");
	for(i=0;i<cur->num;i++)
		fprintf(f,"%d ",root->d[i]);
	fprintf(f,"\"\n");
	
	// Prints all the links
	
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
	
	// Recursively calls all the children 
	for(i=0;i<=4;i++)
		print(f,root->ptr[i]);		
}			

// A function to create the output file
// Calls 'print' function to print nodes
// into the output file				
void makeFile(node root)
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
	node cur;
	while(fscanf(stdin,"%d",&e)==1)
	{
		if(e==EOF)
			break;
		// For creating root node
		if(root==NULL)
		{
			root=makeNode();
			root->r=1;
			root=insertion(root,e);
		}
		else
		{
			cur=find(e);
			cur=insertion(cur,e);
		}
	}
	makeFile(root);
}
