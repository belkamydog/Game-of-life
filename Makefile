app:
	gcc -Wall -Werror -Wextra  game_of_life.c  -lncurses
clean:
	rm -rf *.out