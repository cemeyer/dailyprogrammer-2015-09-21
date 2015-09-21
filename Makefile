FLAGS=	-Wall -Wextra -Wno-missing-field-initializers

ascii: main.c
	cc $(FLAGS) -g -O3 -std=gnu11 -o $@ $<
