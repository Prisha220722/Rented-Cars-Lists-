# CIS*2520 F26 Assignment 1 - Makefile
# Complete the four TODO items below. Every command line in a rule must
# start with a TAB (not spaces). Do not change the compiler or the flags.

CC = clang
CFLAGS = -std=c99 -Wall -pedantic
TARGET = a1rental

SRCS = a1.c a1_functions.c
OBJS = a1.o a1_functions.o

all: $(TARGET)
$(TARGET): $(OBJS) 
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

a1.o: a1.c a1.h 
	$(CC) $(CFLAGS) -c a1.c

a1_functions.o: a1_functions.c a1.h 
	$(CC) $(CFLAGS) -c a1_functions.c 

clean: 
	rm -f $(OBJS) $(TARGET)
.PHONY: all clean
