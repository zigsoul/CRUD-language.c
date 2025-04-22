CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99
LDFLAGS =

.PHONY: all test clean

all: crud test_crud

crud: main.o crud.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_crud: test_crud.o crud.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

test: test_crud
	./test_crud

clean:
	rm -f *.o crud test_crud database.txt