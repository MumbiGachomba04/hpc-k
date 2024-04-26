include $(dir $(lastword $(MAKEFILE_LIST)))config.mk
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

include $(dir $(lastword $(MAKEFILE_LIST)))target.mk
