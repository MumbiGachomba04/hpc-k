PROGRAM=matmul

CC=gcc
RM=rm -fr

CFLAGS=-O3
LDFLAGS=

.PHONY: clean

default: $(PROGRAM)

matmul: matmul.o
	$(CC) $^ $(CFLAGS) $(LDFLAGS) -o $@ 

matmul.o: matmul.c
	$(CC) -c $^ $(CFLAGS) -o $@

clean:
	$(RM) *.o
	$(RM) $(PROGRAM)

