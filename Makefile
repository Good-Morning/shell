all:
	cc -Wall sources/*.c -o cmd
debug:
	cc -g -Wall -fsanitize=address sources/*.c -o cmd
