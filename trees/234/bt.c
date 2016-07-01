/*A program to print 2-3-4 trees.
	Algorithm taken from Introduction to Algorithms by T.Cormen
	B Trees - Page Number 491-498
	and Data structures and Algorithms by G A V Pai
	Page Number 263-277
*/ 

#include<stdio.h>
#include<stdlib.h>

struct tree
{
	int d[3],leaf,num;
	struct tree* ptr[4];
};
typedef struct tree * node;

/* A function to create a new blank node with
	no elements 
*/
node makeNode()
{
	node p=(node)malloc(sizeof(struct tree));
	p->d[0]=p->d[1]=p->d[2]='\0';
	p->ptr[0]=p->ptr[1]=p->ptr[2]=p->ptr[3]=NULL;
	p->leaf=p->num=0; 
	return p;
}

/* A function to check if the tree is 2-3-4
*/
void checkTree(node root)
{
	int i,flag=0;
	if(root->ptr[0]!=NULL)
	{
		i=root->ptr[0]->num;
		if(root->d[0]<root->ptr[0]->d[i-1])
			flag=1;
		checkTree(root->ptr[0]);
	}
	if(root->ptr[1]!=NULL)
	{
		i=root->ptr[1]->num;
		if(root->d[1]<root->ptr[1]->d[i-1] && 
			root->d[0]>root->ptr[1]->d[i-1])
			flag=1;
		checkTree(root->ptr[1]);
	}
	if(root->ptr[2]!=NULL)
	{
		i=root->ptr[0]->num;
		if(root->d[2]<root->ptr[2]->d[i-1] &&
			root->d[1]>root->ptr[2]->d[i-1])
			flag=1;
		checkTree(root->ptr[2]);
	}
	if(root->ptr[3]!=NULL)
	{
		i=root->ptr[0]->num;
		if(root->d[2]>root->ptr[3]->d[i-1])
			flag=1;
		checkTree(root->ptr[3]);
	}	
	if(flag==1)
	{
		printf("Not 2-3-4\n");
		exit(0);
	}
}

/* A function to split a full node
*/
node split(node root,int i,node child)
{
	node temp;
	int j;
	temp=makeNode();
	temp->leaf=child->leaf;
	temp->d[0]=child->d[2];
	(temp->num)++;
	
	// To move pointers to new node
	if(!(child->leaf))
	{
		temp->ptr[0]=child->ptr[2];
		temp->ptr[1]=child->ptr[3];
		child->ptr[2]=child->ptr[3]=NULL;
	}
	
	child->num=1;
	
	// To move pointers of root node
	for(j=((root->num)+1);j>i+1;j--)
		root->ptr[j]=root->ptr[j-1];
	root->ptr[i+1]=temp;
	
	// To move elements of root node
	for(j=(root->num);j>i;j--)
		root->d[j]=root->d[j-1];
	root->d[i]=child->d[1];
	
	(root->num)++;
	child->d[1]=child->d[2]='\0';
	return root;
}


/* A function to insert in a not completely
	filled node.
*/
node insertNFull(node root,int e)
{
	int i;
	i=root->num;
	//if leaf, find the location of the element and insert
	if(root->leaf)
	{	while(i>0 && root->d[i-1]>e)
		{	root->d[i]=root->d[i-1];
			i--;
		}
		root->d[i]=e;
		(root->num)++;
	}
	//if not leaf, go to the branch where element is to be 
	//inserted and check if full, if full split else insert
	else
	{
		while(i>0 && root->d[i-1]>e)
			i--;
		if((root->ptr[i])->num==3)
		{	root=split(root,i,root->ptr[i]);
			if(e>root->d[i])
				i++;
		}
		root->ptr[i]=insertNFull(root->ptr[i],e);
	}
	return root;
}
			
/* Function to insert an element into 2-3-4 tree. 
*/
node insertion(node root,int e)
{
	node temp; 

	if(root==NULL)
	{
		// For first node
		root=makeNode();
		root->leaf=1;
		root->d[0]=e;
		(root->num)++;
	} else if(root->num==3) {
		// If the root node is full
		temp=makeNode();
		temp->ptr[0]=root;
		temp=split(temp,0,root);
		temp=insertNFull(temp,e);
		return temp;
	} else {
		//if the root is not full
		insertNFull(root,e);
	}

	return root;
}

/*A function to print the inorder traversal of the tree.
*/	
void printin(node root)
{
	if(root==NULL)
		return;
	printin(root->ptr[0]);
	printf("%d\n",root->d[0]);
	printin(root->ptr[1]);
	printf("%d\n",root->d[1]);
	printin(root->ptr[2]);
	printf("%d\n",root->d[2]);
	printin(root->ptr[3]);
}	

/* A fuction to recursively call the nodes and print in file.
*/
void print(node root,FILE *f)
{

	if(root==NULL)
		// When the branch terminates
		return;

	if(root->leaf)
	{
		// To print the node if there are only three elements
		fprintf(f,"\"%d   %d   %d\"\n",root->d[0],root->d[1],
		root->d[2]);
		return;
	}

	
	print(root->ptr[0],f);
	if(root->ptr[0]!=NULL)
		fprintf(f,"\"%d   %d   %d\"->\"%d   %d   %d\"\n",
		root->d[0],root->d[1],root->d[2],(root->ptr[0])->d[0],
		(root->ptr[0])->d[1],(root->ptr[0])->d[2]);
	print(root->ptr[1],f);
	if(root->ptr[1]!=NULL)
		fprintf(f,"\"%d   %d   %d\"->\"%d   %d   %d\"\n",
		root->d[0],root->d[1],root->d[2],(root->ptr[1])->d[0],
		(root->ptr[1])->d[1],(root->ptr[1])->d[2]);
	print(root->ptr[2],f);
	if(root->ptr[2]!=NULL)
		fprintf(f,"\"%d   %d   %d\"->\"%d   %d   %d\"\n",
		root->d[0],root->d[1],root->d[2],(root->ptr[2])->d[0],
		(root->ptr[2])->d[1],(root->ptr[2])->d[2]);
	print(root->ptr[3],f);
	if(root->ptr[3]!=NULL)
		fprintf(f,"\"%d   %d   %d\"->\"%d   %d   %d\"\n",
		root->d[0],root->d[1],root->d[2],(root->ptr[3])->d[0],
		(root->ptr[3])->d[1],(root->ptr[3])->d[2]);
	return;
}

/* A function to print the tree in dot file
	calls the 'print' function above
*/
void makeFile(node root)
{
	FILE *f;
	f=fopen("opbt.dot","w");
	if(f==NULL)
		exit(0);
	fprintf(f,"digraph{\n");
	print(root,f);
	fprintf(f,"}");
	fclose(f);
}

/*main function to run these functions
*/
void main()
{
	int ele;
	node root=NULL;
	while(fscanf(stdin,"%d",&ele)==1)
	{
		if(ele==EOF)
			break;
		root=insertion(root,ele);
	}
	checkTree(root);
//	printin(root);
	makeFile(root);
}
