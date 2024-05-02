HPCK_FMK=$(shell readlink -f $(lastword $(MAKEFILE_LIST)))
HPCK_MAK=$(shell readlink -f $(firstword $(MAKEFILE_LIST)))
HPCK_DIR=$(shell dirname $(HPCK_FMK))

HPCK_BUILD=$(HPCK_DIR)/tools/bin/hpck-build

HPCK_PROGRAM_DIR=$(shell dirname $(HPCK_MAK))
HPCK_PROGRAM_TMP=$(subst $(HPCK_DIR)/,,$(HPCK_PROGRAM_DIR))
HPCK_PROGRAM=$(subst /,-,$(HPCK_PROGRAM_TMP))

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

