sokoban: sokoban.c
	gcc -std=c11 -pedantic -Wall -Wextra -Werror -fstack-protector-strong -g sokoban.c -o sokoban