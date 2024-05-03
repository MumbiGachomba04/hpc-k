HPCK_FMK=$(shell readlink -f $(lastword $(MAKEFILE_LIST)))
HPCK_MAK=$(shell readlink -f $(firstword $(MAKEFILE_LIST)))
HPCK_DIR=$(shell dirname $(HPCK_FMK))

HPCK_BUILD=$(HPCK_DIR)/tools/bin/hpck-build

HPCK_PROGRAM_DIR=$(shell dirname $(HPCK_MAK))
HPCK_PROGRAM_TMP=$(subst $(HPCK_DIR)/,,$(HPCK_PROGRAM_DIR))
HPCK_PROGRAM=$(subst /,-,$(HPCK_PROGRAM_TMP))

