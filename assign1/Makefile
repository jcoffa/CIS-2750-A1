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
SRC = src
OUT = bin
INCL = include
VPATH = $(SRC):$(INCL):$(OUT)

# compilation options
CC = gcc
CFLAGS := -std=c11 -Wall -Wpedantic $(addprefix -I,$(INCL)) -g
LDFLAGS := -L. -L$(OUT) $(addprefix -l,$(SHARED))


##############
# Make Rules #
##############

all: list parser libparsing.so libinit.so

list: liblist.so

parser: libcal.so
	
liblist.so: LinkedListAPI.o
	$(CC) -shared $(OUT)/LinkedListAPI.o -o $(OUT)/liblist.so

LinkedListAPI.o: LinkedListAPI.c LinkedListAPI.h
	$(CC) $(CFLAGS) -c -fpic $(SRC)/LinkedListAPI.c -o $(OUT)/LinkedListAPI.o

libcal.so: CalendarParser.o
	$(CC) -shared $(OUT)/CalendarParser.o -o $(OUT)/libcal.so

CalendarParser.o: CalendarParser.c CalendarParser.h
	$(CC) $(CFLAGS) -c -fpic $(SRC)/CalendarParser.c -o $(OUT)/CalendarParser.o

libparsing.so: Parsing.o
	$(CC) -shared $(OUT)/Parsing.o -o $(OUT)/libparsing.so

Parsing.o: Parsing.c Parsing.h
	$(CC) $(CFLAGS) -c -fpic $(SRC)/Parsing.c -o $(OUT)/Parsing.o

libinit.so: Initialize.o
	$(CC) -shared $(OUT)/Initialize.o -o $(OUT)/libinit.so

Initialize.o: Initialize.c Initialize.h
	$(CC) $(CFLAGS) -c -fpic $(SRC)/Initialize.c -o $(OUT)/Initialize.o



#############
# Utilities #
#############

# removes all .o and .so files, and the main executable
clean:
	rm $(OUT)/*.o $(OUT)/*.so

