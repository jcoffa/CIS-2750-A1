#####################################
#   Name: Joseph Coffa              #
#   Student #: 1007320              #
#   Due Date: February 4, 2019      #
#                                   #
#   Assignment 1, CIS*2750          #
#   Makefile                        #
#####################################

#############
# Variables #
#############

# files
LIBS = CalendarParser.h LinkedListAPI.h Parsing.h Initialize.h
SHARED = list cal parsing init

# update VPATH to search directories for files specified in rule preqrequisites
VPATH = src:include:bin

# compilation options
CC = gcc
CFLAGS = -std=c11 -Wall -Wpedantic -Iinclude -g
LDFLAGS := -L. -Lbin $(addprefix -l,$(SHARED))


##############
# Make Rules #
##############

all: list parser libparsing.so libinit.so main

list: liblist.so

parser: libcal.so
	
liblist.so: LinkedListAPI.o
	$(CC) -shared bin/LinkedListAPI.o -o bin/liblist.so

LinkedListAPI.o: LinkedListAPI.c LinkedListAPI.h
	$(CC) $(CFLAGS) -c -fpic src/LinkedListAPI.c -o bin/LinkedListAPI.o

libcal.so:: CalendarParser.o
	$(CC) -shared bin/CalendarParser.o -o bin/libcal.so

CalendarParser.o: CalendarParser.c CalendarParser.h
	$(CC) $(CFLAGS) -c -fpic src/CalendarParser.c -o bin/CalendarParser.o

libparsing.so: Parsing.o
	$(CC) -shared bin/Parsing.o -o bin/libparsing.so

Parsing.o: Parsing.c Parsing.h
	$(CC) $(CFLAGS) -c -fpic src/Parsing.c -o bin/Parsing.o

libinit.so: Initialize.o
	$(CC) -shared bin/Initialize.o -o bin/libinit.so

Initialize.o: Initialize.c Initialize.h
	$(CC) $(CFLAGS) -c -fpic src/Initialize.c -o bin/Initialize.o

main: main.c $(addprefix lib,$(addsuffix .so,$(SHARED)))
	$(CC) $(CFLAGS) $(LDFLAGS) src/main.c -o bin/main



#############
# Utilities #
#############

run:
	$(MAKE) main
	LD_LIBRARY_PATH=.:bin:$LD_LIBRARY_PATH ./bin/main

# make and run the main executable with valgrind
memory:
	$(MAKE) main
	LD_LIBRARY_PATH=.:bin:$LD_LIBRARY_PATH valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all ./bin/main

debug:
	$(MAKE) main
	LD_LIBRARY_PATH=.:bin:$LD_LIBRARY_PATH gdb ./bin/main

# removes all .o and .so files, and the main executable
clean:
	rm bin/*.o bin/*.so bin/main

