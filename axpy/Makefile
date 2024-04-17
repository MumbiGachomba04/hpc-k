PROGRAM=axpy
include ../kernel.mk

#CC=gcc

CFLAGS+=-DREAL=float 
LDFLAGS+=
INC+=
LIB+=

default: $(PROGRAM)

$(PROGRAM): $(PROGRAM).o $(MAIN)
	$(CC) $^ $(CFLAGS) $(LDFLAGS) $(LIB) -o $@ 

$(PROGRAM).o: $(PROGRAM).c
	$(BUILD_INIT)
	$(CC) -c $^ $(CFLAGS) $(INC) $(BUILD_INFO) -o $@
	$(BUILD_FINI)


