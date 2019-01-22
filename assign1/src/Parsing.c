/************************************
 *  Name: Joseph Coffa              *
 *  Student #: 1007320              *
 *  Due Date: February 4, 2019      *
 *                                  *
 *  Assignment 1, CIS*2750          *
 *  Parsing.c                       *
 ************************************/

#include <stdio.h>

#include "Parsing.h"

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
char *concat(char *dest, const char *src) {
    if (strlen(dest) == 0) {
        return strcpy(dest, src);
    }

    return strcat(dest, src);
}


/*
 * Unfolds a single string containing zero or more (CRLF)(single whitespace) sequences.
 * The string must contain only 1 null-terminator, and it must be at
 * the very end of the string (as expected). If multiple null-terminated lines are strung
 * together without the middle null-terminator(s) removed, this function will only
 * remove folds up until the first null-terminator.
 *
 * Refer to page 9 of the RFC5545 iCal specification for more information on
 * folding and unfolding.
 */
void unfold(char *foldedString) {
    char *foldedCopy = foldedString;
    char *overwrite = foldedString;

    while (*foldedCopy != '\0') {
        if (*foldedCopy == '\r' && *(foldedCopy+1) == '\n') {    // a fold is starting
            foldedCopy += 2; // skip over the \r and the \n

            // The next character should be either whitespace, or a null-terminator.
            // We only want to increment this pointer if it is whitespace.
            if (isspace(*foldedCopy)) {
                foldedCopy += 1;
            }
        } else {
            // The current character is not indicative of a fold, so it
            // is copied into the passed string.
            *overwrite = *foldedCopy;

            // prepare to look at the next character
            overwrite++;
            foldedCopy++;
        }
    }

    // all folds have been dealt with, the string is terminated
    *overwrite = '\0';
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


void strUpper(char *string) {
    char *iter = string;

    while (*iter) {
        *iter = toupper((unsigned char) *iter);
        iter += 1;
    }
}


char *strUpperCopy(const char *string) {
    char *toReturn = malloc(strlen(string) + 1);

    strcpy(toReturn, string);
    strUpper(toReturn);
    return toReturn;
}

/*
 * Reads in at most 'size' bytes from 'fp' and stores it in the buffer pointed to by 'unfolded'.
 * Continually reads lines as long as folded lines are encountered. Stops when a line
 * without a fold is read, or if the end of the file is reached.
 * Folded lines are concatenated together, and then unfolded to make one single line with no
 * CRLF(whitesapce) sequences.
 * 'unfold' must be large enough to hold the entire string.
 *
 * Returns the unfolded line after removing all leading and trailing whitespace characters..
 */
char *readFold(char *unfolded, int size, FILE *fp) {
    char buf[size];
    bool foundFold = false;
    int sizeLeft = size;
    int bufLength;

    // clear 'unfolded' for more reliable concatenation
    strcpy(unfolded, "");

    while (fgets(buf, sizeLeft, fp)) {
        bufLength = strlen(buf);
        sizeLeft -= bufLength;
        // there were folded lines, but then a non-folded line was found
        // (i.e. end of fold was found)
        if (foundFold && !isspace(buf[0])) {
            // return to the previous line
            fseek(fp, -bufLength, SEEK_CUR);
            break;
        }

        // the line must have begun with a space, meaning there is a fold present
        concat(unfolded, buf);
        foundFold = true;
    }

    if (sizeLeft <= 0) {
        printf("\t--- ERROR --- readFold exceeded read size of %d characters\n", size);
    }

    unfold(unfolded);
    return trimWhitespace(unfolded);
}


ICalErrorCode getEvent(FILE *fp, Event **event) {
    char line[2000];
    bool dtStamp, dtStart, UID;
    dtStamp = dtStart = UID = false;

    *event = newEvent();

    while (strcmp(readFold(line, 20000, fp), "END:VEVENT") != 0 && !feof(fp)) {
        if (startsWith(line, ";")) {
            // lines that start with ';' are comments and should be ignored
            continue;
        } else if (startsWith(line, "DTSTAMP")) {
            // creation date of alarm
            if (dtStamp) {
                deleteEvent(*event);
                return INV_EVENT;
            }
            dtStamp = true;

            (*event)->creationDateTime = newDateTime(line + strcspn(line, ":;") + 1);
        } else if (startsWith(line, "DTSTART")) {
            // start of event
            if (dtStart) {
                deleteEvent(*event);
                return INV_EVENT;
            }
            dtStart = true;

            (*event)->startDateTime = newDateTime(line + strcspn(line, ":;") + 1);
        } else if (startsWith(line, "UID")) {
            if (UID) {
                deleteEvent(*event);
                return INV_EVENT;
            }
            UID = true;

            strcpy((*event)->UID, line + 4);
        } else if (startsWith(line, "BEGIN:VALARM")) {
            Alarm *toAdd;
            if (getAlarm(fp, &toAdd) == OK) {
                 insertFront((*event)->alarms, (void *)toAdd);
            } else {
                return INV_ALARM;
            }
        } else {
           insertFront((*event)->properties, (void *)newProperty(line));
        }
    }

    return OK;
}


ICalErrorCode getAlarm(FILE *fp, Alarm **alarm) {
    char line[2000];
    bool trigger, action;
    trigger = action = false;

    *alarm = newAlarm();

    while (strcmp(readFold(line, 2000, fp), "END:VALARM") != 0 && !feof(fp)) {
        if (startsWith(line, ";")) {
            // lines that start with ';' are comments and should be ignored
            continue;
        } else if (startsWith(line, "TRIGGER")) {
            // -8 for the characters in 'TRIGGER:', +1 for null terminator
            if (trigger) {
                deleteAlarm(*alarm);
                return INV_ALARM;
            }
            trigger = true;

            (*alarm)->trigger = malloc(strlen(line) - 7);
            strcpy((*alarm)->trigger, line + 8);
        } else if (startsWith(line, "ACTION")) {
            if (action) {
                deleteAlarm(*alarm);
                return INV_ALARM;
            }
            action = true;

            strcpy((*alarm)->action, line + 7);
        } else {
            insertFront((*alarm)->properties, (void *)newProperty(line));
        }
    }

    return OK;
}

