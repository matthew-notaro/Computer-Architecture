#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool solve();
bool backtrack();
int* getAllowedVals(int, int);
bool isValid();
bool isFull();
void printGrid();
void readFile(FILE *fp);
int charToInt(char);
char intToChar(int);

static int grid[16][16];

int main(int argc, char **argv){
	if(argc < 2)
		return 0;
	FILE *fp = fopen(argv[1], "r");
	readFile(fp);
	if(!isValid() || !solve())
		printf("no-solution\n");
/*	if(isValid()){
		if(!solve())
			printf("no-solution\n");
	}
	else
		printf("no-solution\n");
	printGrid();
*/	return 0;
}

/* attempts to fill in hexadoku with 100% certainty */
bool solve(){
	int i, j, k, numVals;
	bool didSomething = true;
	int *allowedVals;
	/* loops until change not made to grid in a full pass */
	while(didSomething){
		didSomething = false;
		/* for each empty index of grid, finds allowed values */
		for(i = 0; i < 16; i++){
			for(j = 0; j < 16; j++){
				if(grid[i][j] == -1){
					allowedVals = getAllowedVals(i, j);
					numVals = allowedVals[16];
					/* if only 1 allowed val, insert into grid */
					if(numVals == 1){
						for(k = 0; k < 16; k++){
							if(allowedVals[k] == 1){
								grid[i][j] = k;
								didSomething = true;
							}
						}
					}
					free(allowedVals);
				}
			}
		}
	}
	/* checks if grid is full and solved after loop */
	if(isFull() || backtrack()){
		printGrid();
		return true;
	}
	return false;
}


/* continues attempting to solve hexadoku by brute force guess and check with backtracking */
bool backtrack(){
	/* goes back and tries a different number if board invalid */
	if(!isValid())
		return false;
	/* looks for empty square */
	int i, j, row = 0, col = 0;
	for(i = 0; i < 16; i++){
		for(j = 0; j < 16; j++){
			if(grid[i][j] == -1){
				row = i;
				col = j;
			}
		}
	}
	/* returns 1 if full */
	if(row == 0 && col == 0)
		return true;
	int *allowedVals = getAllowedVals(row, col);
	for(i = 0; i < 16; i++){
		if(allowedVals[i]){
			grid[row][col] = i;
			if(backtrack())
				return false;
			grid[row][col] = -1;
		}
	}
	return false;
}


/* returns array with number of allowed values followed by the allowed values */
int* getAllowedVals(int row, int col){
	int i, j, numVals = 16;
	/* keep track of allowed values by setting value to false when found in row/col/box */
	int *allowedVals = malloc(17 * sizeof(int));
	for(i = 0; i < 17; i++)
		allowedVals[i] = 1;
	/* checks col */
	for(i = 0; i < 16; i++){
		if(grid[i][col] != -1 && allowedVals[grid[i][col]] == 1){
			allowedVals[grid[i][col]] = 0;
			numVals--;
		}
	}
	/* checks row */
	for(j = 0; j < 16; j++){
		if(grid[row][j] != -1 && allowedVals[grid[row][j]] == 1){
			allowedVals[grid[row][j]] = 0;
			numVals--;
		}
	}
	/* checks box */
	for(i = row/4*4; i < row/4*4 + 4; i++){
		for(j = col/4*4; j < col/4*4 + 4; j++){
			if(grid[i][j] != -1 && allowedVals[grid[i][j]] == 1){
				allowedVals[grid[i][j]] = 0;
				numVals--;
			}
		}
	}
	allowedVals[16] = numVals;
	return allowedVals;
}


/* checks if grid is a valid hexadoku grid */
bool isValid(){
	int i, j, box;
	/* checks rows */
	for(i = 0; i < 16; i++){
		char check[16] = {0};
		for(j = 0; j < 16; j++){
			if(grid[i][j] != -1){
				if(check[grid[i][j]])
					return false;
				check[grid[i][j]] = true;
			}
		}
	}
	/* checks cols */
	for(i = 0; i < 16; i++){
		char check[16] = {0};
		for(j = 0; j < 16; j++){
			if(grid[j][i] != -1){
				if(check[grid[j][i]])
					return false;
				check[grid[j][i]] = true;
			}
		}
	}
	/* checks boxes */
	for(box = 0; box < 16; box++){
		char check[16] = {0};
		/* checks within each box using some math */
		for(i = box/4*4; i < box/4*4 + 4; i++){
			for(j = box%4*4; j < box%4*4 + 4; j++){
				if(grid[i][j] != -1){
					if(check[grid[i][j]])
						return 0;
					check[grid[i][j]] = true;
				}
			}
		}
	}
	return true;
}


/* checks if grid is full */
bool isFull(){
	int i, j;
	for(i = 0; i < 16; i++){
		for(j = 0; j < 16; j++){
			if(grid[i][j] == -1)
				return false;
		}
	}
	return true;
}
/* prints out hexadoku grid */
void printGrid(){
	int i, j;
	for(i = 0; i < 16; i++){
		for(j = 0; j < 15; j++)
			printf("%c\t", intToChar(grid[i][j]));
		printf("%c\n", intToChar(grid[i][15]));
	}
}
/* reads hexadoku grid from test file */
void readFile(FILE *fp){
	int i, j;
	char temp;
	for(i = 0; i < 16; i++){
		for(j = 0; j < 15; j++){
			fscanf(fp, "%c\t", &temp);
			grid[i][j] = charToInt(temp);
		}
		fscanf(fp, "%c\n", &temp);
		grid[i][15] = charToInt(temp);
	}
	fclose(fp);
}
int charToInt(char c){
	if(c == '-')
		return -1;
	else if(c >= '0' && c <= '9')
		return c -'0';
	return c - 'A' + 10;
}
char intToChar(int i){
	if(i == -1)
		return '-';
	else if(i >= 0 && i <= 9)
		return i + '0';
	return i + 'A' - 10;
}
