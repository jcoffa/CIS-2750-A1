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
LIBS = CalendarParser.h LinkedListAPI.h Parsing.h

# directory names and paths
VPATH = src:include:bin
OUT = bin
SRC = src
INCL = include
SEARCHDIRS := $(addprefix -I,$(INCL))	# Search for libraries in all include (INCL) directories

# compilation options
CC = gcc
CFLAGS := -std=c11 -Wall -Wpedantic $(SEARCHDIRS) -g


##############
# Make Rules #
##############

#all: list parser

# TODO create shared library liblist.so in assign1/bin
#list:

# TODO create shared library libcal.so in assign1/bin
#parser:

main: main.c $(LIBS)
	$(CC) $(CFLAGS) $(addprefix $(SRC)/,$(LIBS:.h=.c)) $(SRC)/main.c -o $(OUT)/main


#############
# Utilities #
#############

# make and run the main executable
run:
	$(MAKE) main
	./$(OUT)/main

# removes all .o and .so files, and the main executable
clean:
	rm $(OUT)/*.o $(OUT)/*.so $(OUT)/main
