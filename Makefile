OS := $(shell uname)

CC=gcc
CFLAGS=-g -Wall
LIBS=-lm
STOCK_DLOAD_SCRIPT=download_stock_quotes.py
SRCS=parser.c income_calc.c #uses implicit rules to make object files
OBJS=$(SRCS:.c=.o)
# define the C object files 
#
# This uses Suffix Replacement within a macro:
#   $(name:string1=string2)
#         For each word in 'name' replace 'string1' with 'string2'
# Below we are replacing the suffix .c of all words in the macro SRCS
# with the .o suffix

.PHONY: clean

default: income_calc

%.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@

income_calc: $(OBJS)
	python $(STOCK_DLOAD_SCRIPT)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LIBS)

test: income_calc
	./test_calc.sh

clean:
	rm $(OBJS)


