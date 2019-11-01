#include <stdio.h>
#include <stdlib.h>

int** initialize(FILE*, int);
int checkSums(int**, int);
int checkNums(int**, int);
void freeMat(int**, int);


/* initializes and populates matrix from file */
int** initialize(FILE* fp, int n){
	int i, j, temp;
	/* allocates mat */
	int** mat = malloc(n*sizeof(int*));
        for(i = 0; i < n; i++)
        	mat[i] = malloc(n*sizeof(int));
        /* initializes mat */
        for(i = 0; i < n; i++){
                for(j = 0; j < n; j++){
                        if(j == n - 1)
                                fscanf(fp, "%d\n", &temp);
                        else
                                fscanf(fp, "%d\t", &temp);
                        mat[i][j] = temp;
                }
        }
	return mat;
}


/* checks if sum of any row = col = main diagonal */
int checkSums(int **mat, int n){
	int sum = 0, temp = 0, i, j;
	/* use main diagonal as base line sum */
	for(i = 0; i < n; i++)
		sum += mat[i][i];
	/* checks other main diagonal */
	for(i = 0; i < n; i++)
		temp += mat[i][n-i-1];
	if(sum != temp)
		return 0;
	temp = 0;
	/* check rows */
	for(i = 0; i < n; i++){
		for(j = 0; j < n; j++)
			temp += mat[i][j];
		if(sum != temp)
			return 0;
		temp = 0;
	}
        /* check cols */
        for(i = 0; i < n; i++){
                for(j = 0; j < n; j++)
                        temp += mat[j][i];
                if(sum != temp)
	              return 0;
	        temp = 0;
        }
	return 1;
}


/* checks if nums 1 to n^2 are present in size n matrix */
int checkNums(int** mat, int n){
	int* arr = malloc((n*n)*sizeof(int));
	int i, j, arePresent = 1;
	/* marks in arr[num-1] if num present to account for indexing */
	for(i = 0; i < n; i++){
		for(j = 0; j < n; j++){
			/* checks for nums > n*n */
			if(mat[i][j] > n*n)
				return 0;
			arr[mat[i][j] - 1] = 1;
		}
	}
	/* checks if all arr[i] == 1 */
	for(i = 0; i < n*n; i++){
		if(arr[i] == 0){
			arePresent = 0;
			break;
		}
	}
	free(arr);
	return arePresent;
}


/* frees memory for matrix */
void freeMat(int** mat, int n){
	int i;
	for(i = 0; i < n; i++)
		free(mat[i]);
	free(mat);
}


int main(int argc, char** argv){
	FILE*fp = fopen(argv[1], "r");
	/*file DNE*/
        if(fp == NULL){
                printf("error\n");
                return 0;
        }
	/* reads matrix size and initializes */
	int n;
	fscanf(fp, "%d\n", &n);
	int** mat = initialize(fp, n);
	/* runs function to check if magic */
	if(checkSums(mat, n) == 1 && checkNums(mat, n) == 1)
		printf("magic\n");
	else	
		printf("not-magic\n");
	freeMat(mat, n);
	return 0;
}
