#####################################
#   Name: Joseph Coffa              #
#   Student #: 1007320              #
#   Due Date: February 4, 2019      #
#                                   #
#   Assignment 1, CIS*2750          #
#   Makefile                        #
#####################################

VPATH = src:include:bin

LIBS = CalendarParser.h LinkedListAPI.h Parsing.h
OUT = bin
SRC = src
INCL = include

CC = gcc
CFLAGS = -std=c11 -Wall -Wpedantic -I$(INCL) -g

#all: list parser

#list:

#parser:

main: main.c $(LIBS)
	$(CC) $(CFLAGS) $(addprefix $(SRC)/,$(LIBS:.h=.c)) $(SRC)/main.c -o $(OUT)/main

#############
# Utilities #
#############
clean:
	rm $(OUT)/* $(OUT)/main
