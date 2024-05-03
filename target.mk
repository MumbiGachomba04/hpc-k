.PHONY: test clean build

PROGRAM_O=$(HPCK_PROGRAM).o
PROGRAM_C=$(HPCK_PROGRAM).c

MAIN_FILE_C=../tools/src/main.c
MAIN_FILE_O=main.o

BUILD_FILE_C=build.c
BUILD_FILE_O=build.o

OBJECTS=$(PROGRAM_O) $(MAIN_FILE_O) $(BUILD_FILE_O)

$(HPCK_PROGRAM): $(OBJECTS)
	@$(HPCK_BUILD) $(CC) $^ $(CFLAGS) $(LDFLAGS) $(LIB) -o $@ 

$(PROGRAM_O): $(PROGRAM_C) Makefile $(HPCK_DIR)/kernel.mk
	@$(HPCK_BUILD) $(CC) -c $< $(CFLAGS) $(INC) -o $@

$(BUILD_FILE_O): $(PROGRAM_O)
	@$(ECHO) char \* __hpck_compiler=\"$(CC)\"\;      > $(BUILD_FILE_C);
	@$(ECHO) char \* __hpck_cflags=\"$(CFLAGS)\"\;   >> $(BUILD_FILE_C);
	@$(ECHO) char \* __hpck_ldflags=\"$(LDFLAGS)\"\; >> $(BUILD_FILE_C);
	@$(ECHO) char \* __hpck_includes=\"$(INC)\"\;    >> $(BUILD_FILE_C);
	@$(ECHO) char \* __hpck_libraries=\"$(LIB)\"\;   >> $(BUILD_FILE_C); 
	@$(HPCK_BUILD) $(CC) -c $(BUILD_FILE_C) $(CFLAGS) $(INC) -o $@

$(MAIN_FILE_O): $(MAIN_FILE_C)
	@$(HPCK_BUILD) $(CC) -c $< $(CFLAGS) $(INC) -o $@

test:
	@$(ECHO) Suite root dir.... $(HPCK_DIR)
	@$(ECHO) Common Makefile... $(HPCK_FMK)
	@$(ECHO) Current Makefile.. $(HPCK_MAK)
	@$(ECHO) Kernel version.... \"$(HPCK_PROGRAM)\"
	@$(ECHO) Compiler.......... $(CC)
	@$(ECHO) Compiler flags.... $(CFLAGS)
	@$(ECHO) Linker flags...... $(LDFLAGS)
	@$(ECHO) Includes.......... $(INC)
	@$(ECHO) Libraries......... $(LIB)

clean:
	$(RM) $(OBJECTS)
	$(RM) $(HPCK_PROGRAM)
	$(RM) $(BUILD_FILE_C)

