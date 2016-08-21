OS := $(shell uname)

CC=gcc
CFLAGS=-g -Wall
SRCS=income_calc.c parser.c
OBJS=$(SRCS:.c=.o)
# define the C object files 
#
# This uses Suffix Replacement within a macro:
#   $(name:string1=string2)
#         For each word in 'name' replace 'string1' with 'string2'
# Below we are replacing the suffix .c of all words in the macro SRCS
# with the .o suffix
LIBS=-lm

default: test

%.o: %.c $(DEPS)
        $(CC) $(CFLAGS) -c -o $@ $<
# -c just compiles object files without linking
# $@ macro takes the 

income_calc: $(OBJS)
        $(CC) $(CFLAGS) -o $@
