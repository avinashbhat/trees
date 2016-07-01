/* A program to insert element into an AVL tree.

Reference : 
Data Structures and Algorithms by G A V Pai
http://www.geeksforgeeks.org/avl-tree-set-1-insertion/

Author : AB
Date : Jun 10 2016
*/

#include<stdio.h>
#include<stdlib.h>

struct tree
{
	int data;
	struct tree* l;
	struct tree* r;
	int h;
};
typedef struct tree * node;

/* A function to return height of the tree
*/
int height(node k)
{
	if(k==NULL)
		return 0;
	return (k->h);
}

/* A function to return max element
*/
int max(int a,int b)
{
	if (a>b)
		return a;
	return b;
}

node makeNode(int e)
{
	node p=(node)malloc(sizeof(struct tree));
	p->data=e;
	p->l=NULL;
	p->r=NULL;
	p->h=1;
	return p;
}

/* A function to rotate a left skewed tree to right
*/
node rightRotate(node root)
{
	node cur=NULL;
	cur=root->l;
	root->l=cur->r;
	cur->r=root;
	root->h=max(height(root->l),height(root->r))+1;
	cur->h=max(height(cur->l),height(cur->r))+1;
	return cur;
}

/* A function to rotate a right skewed tree to left
*/
node leftRotate(node root)
{
	node cur=NULL;
	cur=root->r;
	root->r=cur->l;
	cur->l=root;
	root->h=max(height(root->l),height(root->r))+1;
	cur->h=max(height(cur->l),height(cur->r))+1;
	return cur;
}

/* A function to return the difference between number of
	left and right children
*/
int balance(node root)
{
	if(root==NULL)
		return 0;
	return (height(root->l)-height(root->r));
}

/* A function to insert elements into the tree
*/
node insertion(node root,int e)
{
	int b;
	if(root==NULL)
		return  makeNode(e);
	if(e<root->data)
	root->l=insertion(root->l,e);
	else if(e>=root->data)
	root->r=insertion(root->r,e);
	root->h=max(height(root->l),height(root->r))+1;
	b=balance(root);
	
	// If the tree is skewed
	if(b>1 || b<-1)
	{
		if(b>1 && e<(root->l)->data)
			return rightRotate(root);
		if(b>1 && e>=(root->l)->data)
		{
			root->l=leftRotate(root->l);
			return rightRotate(root);
		}
		if(b<-1 && e>=(root->r)->data)
			return leftRotate(root);
		if(b<-1 && e<(root->r)->data);
		{
			root->r=rightRotate(root->r);
			return leftRotate(root);
		}
	}	
	return root;
}

/*funtion to print the tree to check output
*/
void printin(node root)
{
	if(root==NULL)
		return;
	printin(root->l);
	printf("%d\t",root->data);
	printin(root->r);
}

/*funtion to test whether the tree is avl or not
*/
int checkAvl(node root)
{
	int b,flag=0;
	if(root==NULL)
		return 0;
	b=balance(root);
	if(b<-1 || b>1)
		return 1;
	flag=checkAvl(root->l);
	flag=checkAvl(root->r);
	return flag;
}
	
/*funtion to print the file in dot format. inorder tree format 
printing. in accordance with funtion 'makeFile'
*/
void print(node root,FILE *f)
{
	if(root==NULL)
		return;
	print(root->l,f);
	if(root->l!=NULL)
		fprintf(f,"%d -> %d\n",root->data,(root->l)->data);
	if(root->r!=NULL)
		fprintf(f,"%d -> %d\n",root->data,(root->r)->data);
	print(root->r,f);
}

/*function to print the program in a dot file. gives the appropriate
syntax making use of 'print' function
*/
void makeFile(node root)
{
	FILE *f;
	f=fopen("opavl.dot","w");
	if(f==NULL)
		exit(0);
	fprintf(f,"digraph{\n");
	print(root,f);
	fprintf(f,"}");
	fclose(f);
}

/*main function to run these functions.
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
	if(checkAvl(root))
	{
		printf("Not AVL\n");
		exit(0);
	}
	makeFile(root);
//	printin(root);
}
