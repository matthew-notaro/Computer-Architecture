#include <stdio.h>
#include <stdlib.h>

int isRTP(int num);
int isPrime(int num);

int main(int argc, char** argv){
	FILE *fp = fopen(argv[1], "r");
	/* file DNE */
	if(fp == NULL)
		return 0;
	int i, numNums = 0;
	/* checks for empty files */
	if(fscanf(fp, "%d\n", &numNums) == EOF)
		return 0;
	for(i = 0; i < numNums; i++){
		int num;
		fscanf(fp, "%d\n", &num);
		if(isRTP(num) == 1)
			printf("yes\n");
		else
			printf("no\n");
	}
	fclose(fp);
	return 0;
}


/* checks if num is right truncatable prime */
int isRTP(int num){
	if(num == 0)
		return 0;
	while(num != 0){
		if(isPrime(num) == 0)
			return 0;
		num /= 10;
	}
	return 1;
}


/* checks if num is prime */
int isPrime(int num){
	int i;
	if(num < 2)
		return 0;
	for(i = 2; i <= num / 2; i++){
		if(num % i == 0)
			return 0;
	}
	return 1;
}
