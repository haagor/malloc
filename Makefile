# Makefile pour malloc
#
#           Author: Erick Gallesio [eg@unice.fr]
#    Creation date: 11-May-2015 18:19 (eg)
# Last file update: 11-May-2015 23:38 (eg)
#
CC     = gcc
CFLAGS = -g -Wall -std=gnu99 -DMALLOC_DBG
EXE    = test-fake test-malloc


all: $(EXE)

test-malloc: my-malloc.o test-malloc.o list.o
	$(CC) $(CFLAGS) -o test-malloc my-malloc.o test-malloc.o list.o

test-fake: fake-malloc.o test-malloc.o
	$(CC) $(CFLAGS) -o test-fake fake-malloc.o test-malloc.o

clean:
	rm -f *~ $(EXE) *.o
