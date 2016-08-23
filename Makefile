OS := $(shell uname)

CC=gcc
CFLAGS=-g -Wall
LIBS=-lm

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
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LIBS)

clean:
	rm $(OBJS)


