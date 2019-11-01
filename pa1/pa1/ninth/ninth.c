#include <stdio.h>
#include <stdlib.h>

struct Node* insert(struct Node*, int, int);
void search(struct Node*, int, int);
struct Node* delete(struct Node*, int);
struct Node* deleteHelper(struct Node*, int);
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
	if(root == NULL){
		printf("absent\n");
		return;
	}
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


/* deletes node from BST */
struct Node* delete(struct Node* root, int num){
	if(root == NULL)
		printf("fail\n");
	/* num found, delete successful */
	else if(num == root->data){
		printf("success\n");
		/* node has 0 children */
		if(root->left == NULL && root->right == NULL){
			free(root);
			return NULL;
		}
		/* node has 2 children */
		else if(root->left != NULL && root->right != NULL){
			/* finds min value of right subtree */
			struct Node *ptr = root->right;
			while(ptr->left != NULL)
				ptr = ptr->left;
			root->data = ptr->data;
			/* deletes min value of right subtree */
			root->right = deleteHelper(root->right, root->data);
			return root;
		}
		/* node has 1 child */
		struct Node *temp = (root->left != NULL) ? root->left : root->right;
		free(root);
		return temp;
	}
	else if(num < root->data)
		root->left = delete(root->left, num);
	else
		root->right = delete(root->right, num);
	return root;
}

/* only 0 and 1 child cases so "success" is not printed twice */
struct Node* deleteHelper(struct Node* root, int num){
	if(num == root->data){
		/* node has 0 children */
		if(root->left == NULL && root->right == NULL){
			free(root);
			return NULL;
		}
		/* node has 1 child */
		struct Node *temp = (root->left != NULL) ? root->left : root->right;
		free(root);
		return temp;
	}
	else if(num < root->data)
		root->left = deleteHelper(root->left, num);
	else
		root->right = deleteHelper(root->right, num);
	return root;
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
		else if(letter == 'd')
			root = delete(root, num);
		else
			search(root, num, 1);
	}
	/* closes file, prints output, frees memory */
	fclose(fp);
	freeBST(root);
	return 0;
}
