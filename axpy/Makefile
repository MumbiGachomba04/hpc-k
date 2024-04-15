PROGRAM=axpy

CC=gcc
RM=rm -fr

CFLAGS=-O3 -DREAL=float
LDFLAGS=
INC=
LIB=

.PHONY: clean

default: $(PROGRAM)

$(PROGRAM): $(PROGRAM).o main.o
	$(CC) $^ $(CFLAGS) $(LDFLAGS) $(LIB) -o $@ 

$(PROGRAM).o: $(PROGRAM).c
	$(CC) -c $< $(CFLAGS) $(INC) -o $@

main.o: main.c
	$(CC) -c $< $(CFLAGS) $(INC) -o $@

clean:
	$(RM) *.o
	$(RM) $(PROGRAM)

