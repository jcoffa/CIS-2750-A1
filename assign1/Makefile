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
SHARED = liblist.so libcal.so libparsing.so

# directory names and paths
SRC = src
INCL = include
OUT = bin
VPATH = $(SRC):$(INCLUDE):$(OUT)
SEARCHDIRS := $(addprefix -I,$(INCL))	# Search for libraries in all include (INCL) directories

# compilation options
CC = gcc
CFLAGS := -std=c11 -Wall -Wpedantic $(SEARCHDIRS) -g
LDFLAGS := -L. $(addprefix -L,$(OUT)) $(addprefix -l,$(SHARED:.so= ))


##############
# Make Rules #
##############

all: list parser libparsing.so main

# TODO create shared library liblist.so in assign1/bin
list: LinkedListAPI.o
	$(CC) -shared $(OUT)/LinkedListAPI.o -o $(OUT)/liblist.so

LinkedListAPI.o: LinkedListAPI.c LinkedListAPI.h
	$(CC) $(CFLAGS) -c -fpic $(SRC)/LinkedListAPI.c -o $(OUT)/LinkedListAPI.o

# TODO create shared library libcal.so in assign1/bin
parser: CalendarParser.o
	$(CC) -shared $(OUT)/CalendarParser.o -o $(OUT)/libcal.so

CalendarParser.o: CalendarParser.c CalendarParser.h
	$(CC) $(CFLAGS) -c -fpic $(SRC)/CalendarParser.c -o $(OUT)/CalendarParser.o

libparsing.so: Parsing.o
	$(CC) -shared $(OUT)/Parsing.o -o $(OUT)/libparsing.so

Parsing.o: Parsing.c Parsing.h
	$(CC) $(CFLAGS) -c -fpic $(SRC)/Parsing.c -o $(OUT)/Parsing.o

main: main.c $(LIBS:.h=.c)
	$(CC) $(CFLAGS) $(addprefix $(SRC)/,$(LIBS:.h=.c)) $(SRC)/main.c -o $(OUT)/main
#main: main.c $(SHARED)
#	$(CC) $(LDFLAGS) $(CFLAGS) $(SRC)/main.c -o $(OUT)/main


#############
# Utilities #
#############

run:
	$(MAKE) main
	./$(OUT)/main

# make and run the main executable with valgrind
memory:
	$(MAKE) main
	valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all ./$(OUT)/main

debug:
	$(MAKE) main
	gdb ./$(OUT)/main

# removes all .o and .so files, and the main executable
clean:
	rm $(OUT)/*.o $(OUT)/*.so $(OUT)/main
