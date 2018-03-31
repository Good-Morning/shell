all:
	cc -O3 -Wall sources/*.c -o cmd
debug:
	cc -g -Wall -fsanitize=address sources/*.c -o cmd
run:
	./cmd
