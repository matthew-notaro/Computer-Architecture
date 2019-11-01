#include <stdio.h>
#include <stdlib.h>

struct Node* insert(struct Node*, int);
struct Node* delete(struct Node*, int);
void print(struct Node*);
void freeList(struct Node*);

struct Node{
	int data;
	struct Node *next;
};

int size = 0;


/* inserts node into linked list */
struct Node* insert(struct Node *front, int num){
	/* finds greatest node < data */
	struct Node* ptr = front, *prev = NULL;
	while(ptr != NULL && num >= ptr->data){
		/* checks for duplicates */
		if(num == ptr->data)
			return front;
		prev = ptr;
		ptr = ptr->next;
	}
	/* declares and initializes new node */
	struct Node *temp = malloc(sizeof(struct Node));
	temp->data = num;
	temp->next = NULL;
	size++;
	/* insert at front of list */
	if(prev == NULL){
		temp->next = front;
		return temp;
	}
	/* inserts in middle or at end */
	prev->next = temp;
	temp->next = ptr;
	return front;
}

/* deletes node from linked list */
struct Node* delete(struct Node *front, int num){
	struct Node *ptr = front, *prev = NULL;
	/* finds desired num to be deleted */
	while(ptr != NULL){
		if(num == ptr->data)
			break;
		prev = ptr;
		ptr = ptr->next;
	}
	/* num not found */
	if(ptr == NULL)
		return front;
	size--;
	/* num is first node */
	if(prev == NULL)
		return front->next;
	/* regular deletion */
	prev->next = ptr->next;
	return front;
}

/* prints linked list size then node values */
void print(struct Node *front){
	struct Node* ptr = front;
	printf("%d\n", size);
	while(ptr != NULL){
		printf("%d", ptr->data);
		if(ptr->next != NULL)
			printf("\t");
		ptr = ptr->next;
	}
	printf("\n");
}

/* frees linked list */
void freeList(struct Node *front){
	struct Node* ptr = front, *prev = NULL;
	while(ptr != NULL){
		prev = ptr;
		ptr = ptr->next;
		free(prev);
	}
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
	struct Node *front = NULL;
	while(fscanf(fp,"%c\t%d\n", &letter, &num) != EOF){
		if(letter == 'i')
			front = insert(front, num);
		else
			front = delete(front, num);
	}
	/* closes file, prints output, frees memory */
	fclose(fp);
	print(front);
	freeList(front);
	return 0;
}
