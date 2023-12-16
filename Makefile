all: main

clean:
	rm -f main

main: main.c
	gcc -o main main.c -Wall -Werror
	