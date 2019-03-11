CC = gcc

CFLAGS = -Wall -Werror -Wextra -std=c99 -pedantic -I./include -g

EXEC = test

SRC = src/ini.c src/parser.c src/extract.c src/ast.c

VPATH = src/

OBJS = $(SRC:.c=.o)

all: $(EXEC)

check:
	tests/all_tests.sh

test: $(OBJS)
	$(CC) -o $@ $^

clean:
	rm $(OBJS) $(EXEC)
