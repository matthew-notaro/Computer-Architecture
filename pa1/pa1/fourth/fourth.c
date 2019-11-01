#include <stdio.h>
#include <stdlib.h>

int** allocate(int, int);
int** multiplyMats(int**, int, int, int**, int);
void printMat(int**, int, int);
void freeMat(int**, int);

/* allocates matrix given dimensions */
int** allocate(int row, int col){
	int i;
	int** mat = malloc(row*sizeof(int*));
	for(i = 0; i < row; i++)
		mat[i] = malloc(col*sizeof(int));
	return mat;
}


/* multiples mat1 and mat2 */
int** multiplyMats(int** mat1, int rows1,  int cols1, int** mat2, int cols2){
	int** result = allocate(rows1, cols2);
	int i, j, k;
	for(i = 0; i < rows1; i++){
		for(j = 0; j < cols2; j++){
			for(k = 0; k < cols1; k++)
				result[i][j] += mat1[i][k] * mat2[k][j];	
		}
	}
	return result;
}


/* prints out product mat */
void printMat(int** mat, int row, int col){
	int i, j;
	for(i = 0; i < row; i++){
		for(j = 0; j < col; j++){
			printf("%d", mat[i][j]);
			/* checks if number is end of row */
			if(j == col - 1)
				printf("\n");
			else
				printf("\t");
		}
	}
}


/* frees matrix memory row by row */
void freeMat(int** mat, int row){
	int i;
	for(i = 0; i < row; i++)
		free(mat[i]);
	free(mat);
}


int main(int argc, char** argv){
	if(argc < 2)
		return 0;
	int **mat1, **mat2, **mat3;
	int rows1, cols1, rows2, cols2, rows3, cols3;
	int i, j, temp;
	FILE* fp = fopen(argv[1], "r");
	/*file DNE*/
        if(fp == NULL){
                printf("error\n");
                return 0;
        }
        /* allocates mat1 */
        if(fscanf(fp,"%d\t%d\n", &rows1, &cols1) != EOF)
                mat1 = allocate(rows1, cols1);
        /* initializes mat1 */
        for(i = 0; i < rows1; i++){
                for(j = 0; j < cols1; j++){
                        if(j == cols1 - 1)
                                fscanf(fp, "%d\n", &temp);
                        else
                                fscanf(fp, "%d\t", &temp);
                        mat1[i][j] = temp;
                }
        }
	/* allocates mat2 and checks for right dimensions */
	if(fscanf(fp,"%d\t%d\n", &rows2, &cols2) != EOF){
                if(cols1 != rows2){
                        printf("bad-matrices\n");
                        freeMat(mat1, rows1);
			return 0;
                }
                mat2 = allocate(rows2, cols2);
        }
        /* initializes mat2 */
        for(i = 0; i < rows2; i++){
                for(j = 0; j < cols2; j++){
                        if(j == cols2 - 1)
                                fscanf(fp, "%d\n", &temp);
                        else
                                fscanf(fp, "%d\t", &temp);
                        mat2[i][j] = temp;
                }
        }
        fclose(fp);
	/* sets mat 3 dimensions and multiplies mat1 and mat2 */
	rows3 = rows1;
	cols3 = cols2;
	/*does multiplication and prints mat3 */
	mat3 = multiplyMats(mat1, rows1, cols1, mat2, cols2);
	/* frees memory */
	printMat(mat3, rows3, cols3);
	freeMat(mat1, rows1);
	freeMat(mat2, rows2);
	freeMat(mat3, rows3);
	return 0;
}
