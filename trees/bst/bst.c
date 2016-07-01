#include<stdio.h>
#include<stdlib.h>

struct tree
{
	int data;
	struct tree* left;
	struct tree* right;
};
typedef struct tree* node;

node createNode(int n)
{
    node p;
    p=(node)malloc(sizeof(struct tree));
    p->data=n;
    p->left=NULL;
    p->right=NULL;
    return p;
}

void checkBinary(node r)
{
	int flag=0;
	if(r->left!=NULL)
	{
		if((r->data)<((r->left)->data))
        	{
			flag=1;
			return;
		}
		checkBinary(r->left);
    	}
	if(r->right!=NULL)
	{
		if((r->data)>=((r->right)->data))
	        {
        	    flag=1;
          	  return;
	        }
        checkBinary(r->right);
	}
	if(flag==1)
	{
		printf("Not Binary Tree\n");
		exit(0);
	}
}

node createTree(node r,int e)
{
	if(r==NULL)
		return createNode(e);
	else if(e<r->data)
		r->left=createTree((r->left),e);
	else if(e>r->data)
		r->right=createTree((r->right),e);
	return r;
}

void printinorder(node r)
{
	if(r==NULL)
		return;
	printinorder(r->left);
	printf("%d\t",r->data);
	printinorder(r->right);
}

void print(node n,FILE *f)
{
	if(n==NULL)
		return;
	if(n->left!=NULL)
		fprintf(f,"%d -> %d\n",n->data,(n->left)->data);
	if(n->right!=NULL)
		fprintf(f,"%d -> %d\n",n->data,(n->right)->data);
	print(n->left,f);
	print(n->right,f);
}

void makefile(node n)
{
        FILE *fptr;
        fptr=fopen("opbst.dot","w");
        if(fptr==NULL)
                exit(0);
        fprintf(fptr,"digraph{\n");
        print(n,fptr);
        fprintf(fptr,"}");
        fclose(fptr);
}	

void main()
{
	node root=NULL;
	int ele;
	while(fscanf(stdin,"%d",&ele)==1)
	{
		if(ele==EOF)
			break;
		root=createTree(root,ele);
	}
//	printinorder(root);
	checkBinary(root);
	makefile(root);
}
