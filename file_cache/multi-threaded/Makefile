CC=gcc
CFLAGS=-I

all: fc.out

fc.out: testdriver.c file_cache.c
	$(CC) -pthread -o fc.out testdriver.c file_cache.c

clean:
	rm -rf fc.out
