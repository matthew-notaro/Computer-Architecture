#include <stdio.h>
#include <stdlib.h>

struct Node* insert(struct Node*, int, int);
void search(struct Node*, int, int);
void freeBST(struct Node*);

struct Node{
	int data;
	struct Node *left, *right;
};


/* inserts new node into BST */
struct Node* insert(struct Node *root, int num, int height){
	/* reaches end of BST and inserts */
	if(root == NULL){
		struct Node *temp = malloc(sizeof(struct Node));
		temp->data = num;
		temp->left = NULL;
		temp->right = NULL;
		printf("inserted %d\n", height);
		return temp;
	}
	/* find duplicate value */
	else if(num == root->data)
		printf("duplicate\n");
	/* recurse on left */
	else if(num < root->data)
		root->left = insert(root->left, num, height+1);
	/* recurse on right */
	else
		root->right = insert(root->right, num, height+1);
	return root;
}


/* searches for value in BST */
void search(struct Node* root, int num, int height){
	if(root == NULL)
		printf("absent\n");
	/* num == data */
	else if(num == root->data)
		printf("present %d\n", height);
	/* num < data */
	else if(num < root->data)
		search(root->left, num, height+1);
	/* num > data */
	else
		search(root->right, num, height+1);
}


/* frees up BST from bottom up */
void freeBST(struct Node* root){
	if(root == NULL)
		return;
	freeBST(root->left);
	freeBST(root->right);
	free(root);
}


int main(int argc, char** argv){
	FILE* fp = fopen(argv[1], "r");
	/*file DNE*/
	if(fp == NULL){
		printf("error\n");
		return 0;
	}
	/* reads from file line by line */
	char letter;
	int num;
	struct Node* root = NULL;
	while(fscanf(fp,"%c\t%d\n", &letter, &num) != EOF){
		if(letter == 'i')
			root = insert(root, num, 1);
		else
			search(root, num, 1);
	}
	/* closes file, prints output, frees memory */
	fclose(fp);
	freeBST(root);
	return 0;
}
