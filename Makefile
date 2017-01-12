OS := $(shell uname)

CC=gcc #default compiler, clang used for analysis
CFLAGS=-ggdb3 -Wall #for gcc
#CFLAGS=-Wall # for clang
LIBS=-lm
STOCK_DLOAD_SCRIPT=download_stock_quotes.py

.PHONY: clean
.SECONDARY:
# secondary stops make deleting intermediary files .s and .i

Makefile: ; # otherwise makefile starts applying rules to itself

# dirs for source code, build objs (.s, .o files) and executables
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin

SRC_FILES=$(wildcard src/*.c)
OBJ_FILES = $(patsubst src/%.c, $(BUILD_DIR)/%.o, $(wildcard src/*.c))
# uses string formatting rules 
# define the C object files 
# take all .c in src/
# tokenize src/%.c, where % is the stem
# make the new list = build/5.o
HDR_FILES=$(wildcard include/*.h)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# 1st stage: preprocessing - from source to inlined source files
$(BUILD_DIR)/%.i: $(SRC_DIR)/%.c $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $< -E > $@

# 2nd stage: compilation - making assembly files from inline source
$(BUILD_DIR)/%.s: $(BUILD_DIR)/%.i
	$(CC) $(CFLAGS) -S $< -o $@

# 3rd stage: assembly of .s files into object files
$(BUILD_DIR)/%.o: $(BUILD_DIR)/%.s
	$(CC) $(CFLAGS) -c $< -o $@

# clang static code analysis
analyze: CC=clang
analyze: CFLAGS=--analyze
analyze: $(SRC_FILES) $(HDR_FILES)
	$(CC) $(CFLAGS) $(SRC_FILES) $(HDR_FILES)


income_calc: CPPFLAGS+=-DDEBUG_LVL=1
income_calc: $(OBJ_FILES) $(BIN_DIR)
	#python $(STOCK_DLOAD_SCRIPT)
	$(CC) $(CFLAGS) $(OBJ_FILES) -o $(BIN_DIR)/$@ $(LIBS)

#used for testing mathematical correctness
income_calc_test2: CPPFLAGS+=-DDEBUG_LVL=2
income_calc_test2: $(OBJ_FILES) $(BIN_DIR)
	$(CC) $(CFLAGS) $(OBJ_FILES) -o $(BIN_DIR)/$@ $(LIBS)

# verbose executable compiled 
income_calc_test3: CPPFLAGS+=-DDEBUG_LVL=3
income_calc_test3: $(OBJ_FILES) $(BIN_DIR)
	$(CC) $(CFLAGS) $(OBJ_FILES) -o $(BIN_DIR)/$@ $(LIBS)

install: SHELL := /bin/bash
install: income_calc
	if [ $(shell whoami) == "root" ] ; then \
		cp $(BIN_DIR)/income_calc /usr/local/bin/income_calc; \
	else\
		sudo cp $(BIN_DIR)/income_calc /usr/local/bin/income_calc; \
	fi

uninstall: SHELL := /bin/bash
uninstall: 
	if [ $(shell whoami) == "root" ] ; then \
		rm /usr/local/bin/income_calc; \
	else\
		sudo rm /usr/local/bin/income_calc; \
	fi


all: # makes all artefacts, installs main into executable path
	make install
	make clean_build
	make income_calc_test2
	make clean_build
	make income_calc_test3
	make clean_build

test_python: $(STOCK_DLOAD_SCRIPT)
	python -m unittest discover -s tests/

test: income_calc
	./test_calc.sh
test_unit_parser: CPPFLAGS+=-DDEBUG_LVL=3
#test_unit_parser: CPPFLAGS+=-DDEBUG_LVL=3
test_unit_parser: src/parser.c tests/test_parser.c tests/test_parser.h include/parser.h
	$(CC) $(CFLAGS) $^ -o $(BIN_DIR)/$@

clean_build:
	-rm -r $(BUILD_DIR)

clean_all:
	-rm -r $(BUILD_DIR) $(BIN_DIR)
	-rm *.plist

docker_build:
	make clean_all
	docker build -t petr_tik/income_calc:new .

docker_run:
	# takes the most recent image from the income_calc repo
	docker run -it $(shell docker images petr_tik/income_calc -q | head -n 1) /bin/bash

docker_build_and_run_new: docker_build
	make docker_run
