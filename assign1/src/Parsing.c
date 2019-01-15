/************************************
 *  Name: Joseph Coffa              *
 *  Student #: 1007320              *
 *  Due Date: February 4, 2019      *
 *                                  *
 *  Assignment 1, CIS*2750          *
 *  Parsing.c                       *
 ************************************/

// FIXME NO PATHS IN INCLUDE STATEMENTS
//       Use shared libraries after you figure out how to do that
#include "../include/Parsing.h"

// TODO write functions


/*
 * Removes all leading and trailing whitespace from the given string.
 * Returns the updated string.
 */
char *trimWhitespace(char *string) {
    // trim leading whitespace
    while (isspace(*string)) {
        string++;
    }

    // check if the given string was entirely whitespace (and is now empty)
    if (*string == '\0') {
        return string;
    }

    // get the end of the string before the null terminator
    char *endString = string + strlen(string) - 1;

    // trim trailing whitespace
    while (isspace(*endString)) {
        endString--;
    }

    // add null terminator back in
    *(endString+1) = '\0';

    return string;
}


/*
 * Conctenates src to the end of dest, removing dest's null terminator and replacing
 * it with the first chracter of src. src's null terminator is kept, and it becomes
 * the new end of the string. dest must have enough space to contain both strings!
 * For best results, dest must be an initialized string, but it can be empty
 * (i.e. just a null terminator).
 * Returns a pointer to the resulting string in dest.
 */
char *concat(char *dest, char *src) {
    if (strlen(dest) == 0) {
        return strcpy(dest, src);
    }

    return strcat(dest, src);
}


/*
 * Unfolds a single string containing zero or more (CRLF)(single whitespace) sequences.
 * The string must contain only 1 null-terminator, and it must be at
 * the very end of the fold.
 *
 * Refer to page 9 of the RFC5545 iCal specification for more information on
 * folding and unfolding.
 */
void unfold(char *foldedString) {
    char *foldedCopy = foldedString;
    char *overwrite = foldedString;

    while (*foldedCopy != '\0') {
        if (*foldedCopy == '\r') {    // a fold is starting
            foldedCopy += 3; // skip over the \r, the \n, and the whitespace character
        } else {
            *overwrite = *foldedCopy;
            overwrite++;
            foldedCopy++;
        }
    }

    *overwrite = '\0';

    // an edge case where the last CRLF sequence was not properly removed
    foldedString[strcspn(foldedString, "\r\n")] = '\0';
}


bool startsWith(const char *string, const char *start) {
    int lengthString = strlen(string);
    int lengthStart = strlen(start);

    if (lengthString < lengthStart) {
        return false;
    }

    return strncmp(string, start, lengthStart) == 0;
}


bool endsWith(const char *string, const char *end) {
    int lengthString = strlen(string);
    int lengthEnd = strlen(end);

    if (lengthString < lengthEnd) {
        return false;
    }

    return strncmp(string + lengthString - lengthEnd, end, lengthEnd) == 0;
}

