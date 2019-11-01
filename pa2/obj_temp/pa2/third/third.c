#include <stdio.h>
#include <stdlib.h>

double **allocateMat(int, int);
double **multiplyMats(double**, int, int, double**, int);
double **transposeMat(double**, int, int);
double **invertMat(double**, int);
double **oneShot(double**, double**, int, int);
void freeMat(double** , int);
void printMat(double**, int, int);

int main(int argc, char **argv){
	if(argc < 3)
		return 0;
	/* training file format:
	   K\n (number of attributes)
	   N\n (number of training examples in training data set)
	   (N+1)xK matrix with the +1 being price of house at end of row
	   */
	FILE *trainFile = fopen(argv[1], "r");
	FILE *testFile = fopen(argv[2], "r");
	int K, N, M, i, j;
	/* K - # of attributes, N - # of training examples in training data set */
	fscanf(trainFile, "%d\n", &K);
	fscanf(trainFile, "%d\n", &N);
	fscanf(testFile, "%d\n", &M);
	/* X: attributes from all examples in training data */
	double **X = allocateMat(N, K + 1);
	/* Y: prices of houses in training data */
	double **Y = allocateMat(N, 1);
	/* W: weights for each attribute */
	double **W;
	/* scans training file */
	for(i = 0; i < N; i++){
		/* 1st col - all 1s */
		X[i][0] = 1;
		for(j = 0; j < K + 1; j++){
			if(j == K)
				fscanf(trainFile, "%lf\n", &Y[i][0]);
			else
				fscanf(trainFile, "%lf,", &X[i][j + 1]);
		}
	}
	/* scanns test file */
	double **testAtts = allocateMat(M, K + 1);
	for(i = 0; i < M; i++){
		testAtts[i][0] = 1;
		for(j = 1; j < K + 1; j++){
			if(j == K)
				fscanf(testFile, "%lf\n", &testAtts[i][j]);
			else
				fscanf(testFile, "%lf,", &testAtts[i][j]);
		}
	}
	/* finds weights with matrix manipulation */
	W = oneShot(X, Y, K, N);
	double **result = multiplyMats(testAtts, M, K + 1, W, 1);	
	for(i = 0; i < M; i++)
		printf("%.0lf\n", result[i][0]);
	freeMat(X, N);
	freeMat(Y, N);
	freeMat(W, K + 1);
	freeMat(testAtts, M);
	freeMat(result, M);
	return 0;
}

/* returns matrix W that contains weights */
double **oneShot(double **X, double **Y, int K, int N){
	double **W = allocateMat(K + 1, 1);
	double **X_Transpose = transposeMat(X, N, K + 1);
	/* Broken down to explicitly free space allocated by function calls
	   W = ((X^T)(X))^-1(X^T)(Y)	
	   A = (X^T)(X)
	   B = A^-1
	   C = A(X^T)
	   W = B(Y)
	   */
	double **A = multiplyMats(X_Transpose, K + 1, N, X, K + 1);
	double **B = invertMat(A, K + 1);
	double **C = multiplyMats(B, K + 1, K + 1, X_Transpose, N);
	W = multiplyMats(C, K + 1, N, Y, 1);
	freeMat(X_Transpose, K + 1);
	freeMat(A, K + 1);
	freeMat(B, K + 1);
	freeMat(C, K + 1);
	return W;
}

/* 
   X(W) = Y
   W = ((X^T)(X))^-1(X^T)(Y)
   W = inverse of X and X transpose times X transpose times Y
   X -   Nx(K+1)
   X^T - (K+1)xN
   Y -   Nx1
   W -   (K+1)x1
   M -   MxK
   A = ((X^T)(X))^-1 - (K+1)x(K+1)
   B = A(X^T) = (K+1)xN
   W = B(Y) = (K+1)x1
   */


/* returns inverted square matrix through Gauss-Jordan elimination given mat and row */
double **invertMat(double **mat, int rows){
	int cols = rows * 2;
	double **RREF = allocateMat(rows, cols);
	int i, j, k;
	double multiple, pivot;
	/* creates augmented matrix */
	for(i = 0; i < rows; i++){
		for(j = 0; j < cols; j++){
			/* copies values from mat */
			if(j < rows)
				RREF[i][j] = mat[i][j];
			else if(i + rows == j)
				RREF[i][j] = 1;
			else
				RREF[i][j] = 0;
		}
	}
	/* REF */
	for(i = 0; i < rows; i++){
		pivot = RREF[i][i];
		/* creates leading 1 */
		if(pivot != 1){
			for(k = i; k < cols; k++)
				RREF[i][k] /= pivot;
		}
		/* adds multiples of row to lower rows so all 0s are under leading 1 */
		for(j = i + 1; j < rows; j++){
			multiple = -1 * RREF[j][i];
			for(k = i; k < cols; k++)
				RREF[j][k] += multiple*RREF[i][k];
		}	
	}
	/* RREF */
	/* iterates through previous rows for each row */
	for(i = rows - 1; i >= 0; i--){
		/* iterates through row by row, making 0s above 1s */
		/* RREF[i][j] == 1 */
		for(j = i - 1; j >= 0; j--){
			multiple = -1 * RREF[j][i];
			/* iterates through each element in the row */
			for(k = i; k < cols; k++)
				RREF[j][k] += multiple * RREF[i][k];
		}
	}
	double **result = allocateMat(rows, rows);
	for(i = 0; i < rows; i++){
		for(j = rows; j < cols; j++)
			result[i][j - rows] = RREF[i][j];
	}
	freeMat(RREF, rows);
	return result;
}


/* returns transposed given matrix from given dims */
double **transposeMat(double **mat, int rows, int cols){
	double **trans = allocateMat(cols, rows);
	int i, j;
	for(i = 0; i < rows; i++){
		for(j = 0; j < cols; j++)
			trans[j][i] = mat[i][j];
	}
	return trans;
}


/* returns allocated memory for a matrix given dims */
double **allocateMat(int row, int col){
	double **mat = malloc(row*sizeof(double*));
	int i;
	for(i = 0; i < row; i++)
		mat[i] = malloc(col*sizeof(double));
	return mat;
}
/* returns mat1 * mat2 given mats, dims of mat1, col of mat2 */
double** multiplyMats(double** mat1, int rows1,  int cols1, double** mat2, int cols2){
	double** result = allocateMat(rows1, cols2);
	int i, j, k;
	for(i = 0; i < rows1; i++){
		for(j = 0; j < cols2; j++){
			for(k = 0; k < cols1; k++)
				result[i][j] += mat1[i][k] * mat2[k][j];	
		}
	}
	return result;
}
/* frees matrix memory row by row */
void freeMat(double** mat, int row){
	int i;
	for(i = 0; i < row; i++)
		free(mat[i]);
	free(mat);
}
/* prints given matrix with given dims */
void printMat(double** mat, int row, int col){
	int i, j;
	for(i = 0; i < row; i++){
		for(j = 0; j < col - 1; j++)
			printf("%lf\t", mat[i][j]);
		printf("%lf\n", mat[i][col-1]);
	}
}
