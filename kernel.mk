# Default binaries
CC=gcc
RM=rm -fr
ECHO=echo

# Default flags
CFLAGS=-O3 -Wall
LDFLAGS=
INC=-I../tools/inc
LIB=

MAIN=main.o

BUILD_H=build.h
BUILD_INFO=-include $(BUILD_H)

BUILD_INIT=\
	$(ECHO) char \* __hpck_compiler=\"$(CC)\"\;      > $(BUILD_H); \
	$(ECHO) char \* __hpck_cflags=\"$(CFLAGS)\"\;   >> $(BUILD_H); \
	$(ECHO) char \* __hpck_ldflags=\"$(LDFLAGS)\"\; >> $(BUILD_H); \
	$(ECHO) char \* __hpck_includes=\"$(INC)\"\;    >> $(BUILD_H); \
	$(ECHO) char \* __hpck_libraries=\"$(LIB)\"\;   >> $(BUILD_H); 

BUILD_FINI=\
	$(RM) $(BUILD_H)

target: default

.PHONY: clean

$(MAIN): ../tools/src/main.c
	$(CC) -c $< $(CFLAGS) $(INC) -o $@

clean:
	$(RM) *.o
	$(RM) $(PROGRAM)

