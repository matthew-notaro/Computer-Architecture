#include <stdlib.h>
#include <stdio.h>

struct Node** insert(struct Node**, int);
void search(struct Node**, int);
void freeTable(struct Node**);

struct Node{
	int data;
	struct Node* next;
};


/* inserts num into hash table and checks for duplicates */
struct Node** insert(struct Node** hashTable, int num){
	int hashIndex = abs(num % 1000);
	struct Node* ptr = hashTable[hashIndex];
	/* traverses chain to check for dupes */
	while(ptr != NULL){
		if(ptr->data == num){
			printf("duplicate\n");
			return hashTable;
		}
		ptr = ptr->next;
	}
	/* inserts num at front of chain */
	struct Node* temp = malloc(sizeof(struct Node));
	temp->data = num;
	temp->next = hashTable[hashIndex];
	hashTable[hashIndex] = temp;
	printf("inserted\n");
	return hashTable;
}


/* searches hash table for num */
void search(struct Node **hashTable, int num){
	int hashIndex = abs(num % 1000);
	struct Node* ptr = hashTable[hashIndex];
	/* traverses chain to check if present */
	while(ptr != NULL){
		if(num == ptr->data){
			printf("present\n");
			return;
		}
		ptr = ptr->next;
	}
	printf("absent\n");
}


/*frees memory for each chain and then the table */
void freeTable(struct Node **hashTable){
	int i;
	for(i = 0; i < 1000; i++){
		/* traverses each chain and frees node by node */
		struct Node* ptr = hashTable[i];
		struct Node* prev = NULL;
		while(ptr != NULL){
			prev = ptr;
			ptr = ptr->next;
			free(prev);
		}
	}
	free(hashTable);
}


int main(int argc, char** argv){
	FILE* fp = fopen(argv[1], "r");
	/* file DNE */
	if(fp == NULL){
		printf("error\n");
		return 0;
	}
	struct Node** hashTable = malloc(1000*sizeof(struct Node*));
	char letter;
	int num;
	while(fscanf(fp,"%c\t%d\n", &letter, &num) != EOF){
		if(letter == 'i')
			hashTable = insert(hashTable, num);
		else
			search(hashTable, num);
	}
	fclose(fp);
	freeTable(hashTable);
	return 0;
}
