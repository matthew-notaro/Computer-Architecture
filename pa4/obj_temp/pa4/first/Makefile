all: first

first: first.c
	gcc -g -Wall -Werror -fsanitize=address -std=c11 first.c -lm -o first

clean:
	rm -f first
