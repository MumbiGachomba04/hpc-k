HPCK_FMK=$(shell readlink -f $(lastword $(MAKEFILE_LIST)))
HPCK_MAK=$(shell readlink -f $(firstword $(MAKEFILE_LIST)))
HPCK_DIR=$(shell dirname $(HPCK_FMK))

HPCK_PROGRAM_DIR=$(shell dirname $(HPCK_MAK))
HPCK_PROGRAM_TMP=$(subst $(HPCK_DIR)/,,$(HPCK_PROGRAM_DIR))
HPCK_PROGRAM=$(subst /,-,$(HPCK_PROGRAM_TMP))

default: $(HPCK_PROGRAM)

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

$(HPCK_PROGRAM): $(HPCK_PROGRAM).o $(MAIN)
	$(CC) $^ $(CFLAGS) $(LDFLAGS) $(LIB) -o $@ 

$(HPCK_PROGRAM).o: $(HPCK_PROGRAM).c
	$(BUILD_INIT)
	$(CC) -c $^ $(CFLAGS) $(INC) $(BUILD_INFO) -o $@
	$(BUILD_FINI)

.PHONY: test clean

$(MAIN): ../tools/src/main.c
	$(CC) -c $< $(CFLAGS) $(INC) -o $@

test:
	@$(ECHO) Suite root dir : $(HPCK_DIR)
	@$(ECHO) Common Makefile: $(HPCK_FMK)
	@$(ECHO) Target Makefile: $(HPCK_MAK)
	@$(ECHO) Kernel version : \"$(HPCK_PROGRAM)\"

clean:
	$(RM) *.o
	$(RM) $(HPCK_PROGRAM)

