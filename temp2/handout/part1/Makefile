UTIL_DIR=../utils
TEST_DIR=../test
UTIL_SRC=$(wildcard $(UTIL_DIR)/mem*.c) callinfo.c
UTIL_DEP=$(UTIL_SRC) $(wildcard $(UTIL_DIR)/mem*.h) callinfo.h

SRC=memtrace.c
LIB=libmemtrace.so

ifeq (run,$(firstword $(MAKECMDGOALS)))

  RUN_ARG := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))

  ifndef RUN_ARG
    $(error Syntax: make run <testcase>. See make help for details)
  endif

  ifeq ("$(wildcard $(TEST_DIR)/$(RUN_ARG))","")
    $(error Testcase not found. See handout for details)
  endif

  $(eval $(RUN_ARG):;@:)
endif

help:
	@echo "make <command> where <command> is one of"
	@echo ""
	@echo "  help                This help screen."
	@echo "  compile             Compile the memtrace library."
	@echo "  run <testcase>      Run memtrace with one of the testcases provided in ../test/"
	@echo ""

compile: memtrace.c $(DEPS)
	$(CC) -I. -I $(UTIL_DIR) -o $(LIB) -shared -fPIC $< $(UTIL_SRC) -ldl -lunwind

.PHONY: run
run: compile
	@LD_PRELOAD=./$(LIB) $(TEST_DIR)/$(RUN_ARG)

clean:
	@rm -rf $(LIB) *.o

