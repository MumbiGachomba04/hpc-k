.PHONY: test clean

$(HPCK_PROGRAM): $(HPCK_PROGRAM).o $(MAIN)
	@$(HPCK_BUILD) $(CC) $^ $(CFLAGS) $(LDFLAGS) $(LIB) -o $@ 

$(HPCK_PROGRAM).o: $(HPCK_PROGRAM).c
	$(BUILD_INIT)
	@$(HPCK_BUILD) $(CC) -c $^ $(CFLAGS) $(INC) $(BUILD_INFO) -o $@
	$(BUILD_FINI)

$(MAIN): ../tools/src/main.c
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
	$(RM) *.o
	$(RM) $(HPCK_PROGRAM)

