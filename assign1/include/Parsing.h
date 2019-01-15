/************************************
 *  Name: Joseph Coffa              *
 *  Student #: 1007320              *
 *  Due Date: February 4, 2019      *
 *                                  *
 *  Assignment 1, CIS*2750          *
 *  Parsing.h                       *
 ************************************/

#ifndef PARSING_H
#define PARSING_H

#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

// TODO write functions


/*
 * Removes all leading and trailing whitespace from the given string.
 * Returns the updated string.
 */
char *trimWhitespace(char *string);

/*
 *
 */
char *concat(char *dest, char *src);

/*
 * Unfolds a single string containing the (CRLF)(single whitespace) sequence.
 * The string must contain only 1 null-terminator, and it must be at
 * the very end of the fold.
 *
 * Refer to page 9 of the RFC5545 iCal specification for more information on
 * folding and unfolding.
 */
void unfold(char *foldedString);

bool startsWith(const char *string, const char *start);

bool endsWith(const char *string, const char *end);
#endif // PARSING_H
