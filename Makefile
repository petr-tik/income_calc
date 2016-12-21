OS := $(shell uname)

CC=gcc
CFLAGS=-ggdb3 -Wall
LIBS=-lm
STOCK_DLOAD_SCRIPT=download_stock_quotes.py
SRCS=$(wildcard src/*.c)

.PHONY: clean
.SECONDARY:
# secondary stops make deleting intermediary files .s and .i

OBJS = $(patsubst src/%.c, $(BUILD_DIR)/%.o, $(wildcard src/*.c))
# uses string formatting rules 
# define the C object files 
# take all .c in src/
# tokenize src/%.c, where % is the stem
# make the new list = build/5.o

Makefile: ;
SRC_DIR = src
BUILD_DIR = build

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.i: $(SRC_DIR)/%.c $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $< -E > $@

$(BUILD_DIR)/%.s: $(BUILD_DIR)/%.i
	$(CC) $(CFLAGS) -S $< -o $@

$(BUILD_DIR)/%.o: $(BUILD_DIR)/%.s
	$(CC) $(CFLAGS) -c $< -o $@

income_calc: CPPFLAGS+=-DDEBUG_LVL=1
income_calc: $(OBJS)
	#python $(STOCK_DLOAD_SCRIPT)
	$(CC) $(OBJS) -o $@ $(LIBS)

income_calc_test2: CPPFLAGS+=-DDEBUG_LVL=2
income_calc_test2: $(OBJS) 
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

income_calc_test3: CPPFLAGS+=-DDEBUG_LVL=3
income_calc_test3: $(OBJS) 
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)


install: income_calc
	sudo cp income_calc /usr/local/bin/income_calc

uninstall: clean
	sudo rm /usr/local/bin/income_calc

test_python: $(STOCK_DLOAD_SCRIPT)
	python -m unittest discover -s tests/

test: income_calc
	./test_calc.sh

clean:
	rm -r $(BUILD_DIR)
	rm income_calc*


