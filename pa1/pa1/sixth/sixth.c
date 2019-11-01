#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int isVowel(char);
int findVowelIndex(char*);

int isVowel(char letter){
	return (letter == 'a' || letter == 'e' || letter == 'i' || 
		letter == 'o' || letter == 'u' || letter == 'A' ||
		letter == 'E' || letter == 'I' || letter == 'O' ||
		letter == 'U')  ? 1 : 0; 
}

/* returns index of 1st vowel - returns -1 for words without vowels */
int findVowelIndex(char* word){
	int i;
	for(i = 0; i < strlen(word); i++){
		if(isVowel(word[i]))
			return i;
	}
	return -1;
}

int main(int argc, char** argv){
	if(argc < 2)
		return 0;
	int vowelIndex, i, j;
	/* visits each word in command line */
	for(i = 1; i < argc; i++){
		/* prints spaces between words */
		if(i != 1)
			printf(" ");
		vowelIndex = findVowelIndex(argv[i]);
		/* 1st = vowel: print word + yay */
		if(vowelIndex == 0)
			printf("%syay", argv[i]);
		else if(vowelIndex == -1)
			printf("%say", argv[i]);
		/* 1st != vowel: move nonvowels until 1st vowel to end + ay */
		else{
			for(j = vowelIndex; j < strlen(argv[i]); j++)
				printf("%c", argv[i][j]);
			for(j = 0; j < vowelIndex; j++)
				printf("%c", argv[i][j]);
			printf("ay");
		}
	}
	printf("\n");
	return 0;
}
