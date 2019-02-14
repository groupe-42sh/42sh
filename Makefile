$(info $(SHELL))
CC = gcc
CD = -g
CFLAGS = -Werror -Wextra -Wall -pedantic -std=c99

SRC = src/parser/test.c
OBJS = $(SRC:.c=.o)
EXEC = test

all: $(EXEC)

d: $(OBJS)
	$(CC) $(CD) -o $@ $^

tests:
	tests/

test: $(OBJS)
	$(CC) -o $@ $^

.PHONY:clean

clean :
	$(RM) src/parser/*.o
	$(RM) test
