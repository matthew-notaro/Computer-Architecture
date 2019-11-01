#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv){
	if(argc < 2)
		return 0;
	int i;
	/* prints last char of each word inputted from command line */
	for(i = 1; i < argc; i++)
		printf("%c", argv[i][strlen(argv[i]) - 1]);
	printf("\n");
	return 0;
}
