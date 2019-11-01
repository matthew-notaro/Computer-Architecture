all: seventh 

seventh: seventh.c
	gcc -Wall -Werror -fsanitize=address -std=c11 seventh.c -o seventh
clean:
	rm -f seventh
