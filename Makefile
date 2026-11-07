CC = clang

main: main.c b64.c
	$(CC) -Wall -Wextra -ggdb -O0 main.c b64.c -o main
