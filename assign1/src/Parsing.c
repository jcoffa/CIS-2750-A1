/************************************
 *  Name: Joseph Coffa              *
 *  Student #: 1007320              *
 *  Due Date: February 4, 2019      *
 *                                  *
 *  Assignment 1, CIS*2750          *
 *  Parsing.c                       *
 ************************************/

#include "Parsing.h"


/*
 * To be used during createCalendar when something goes wrong or if the calendar
 * is determined to be invalid. Memory needs to be freed, files need to be closed,
 * and structures set to NULL.
 */
void cleanup(Calendar **toDelete, char *upperCopy, FILE *toClose) {
    if (toDelete != NULL) {
        deleteCalendar(*toDelete);
        *toDelete = NULL;
    }

    if (upperCopy != NULL) {
        free(upperCopy);
    }

    if (toClose != NULL) {
        fclose(toClose);
    }
}

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
 * Returns the unfolded line after removing all leading and trailing whitespace characters.
 */
char *readFold(char *unfolded, int size, FILE *fp) {
    char buf[size];
    bool foundFold = false;
    int sizeLeft = size;
    int bufLength;

    // clear 'unfolded' for more reliable concatenation
    strcpy(unfolded, "");

    while (fgets(buf, sizeLeft, fp)) {
        if (buf[0] == ';') {
            // lines that begin with a semicolon are comments and should be ignored
            continue;
        }

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
    char line[10000], *parse;
    ICalErrorCode error;
    bool dtStamp, dtStart, UID;
    parse = NULL;
    dtStamp = dtStart = UID = false;

    fprintf(stdout, "\tDEBUG: Started getEvent()\n");

    if ((error = initializeEvent(event)) != OK) {
        return error;
    }

    while (!feof(fp)) {
        if (parse != NULL) {
            free(parse);
        }

        readFold(line, 10000, fp);
        parse = strUpperCopy(line);
        fprintf(stdout, "\tDEBUG: in getEvent: unfolded, upper'd line: \"%s\"\n", parse);

        // This check can't be in the condition for the while loop, since
        // iCal files are case insensitive, and therefore the case must be
        // made uniform before checking.
        if (startsWith(parse, "END:VEVENT")) {
            fprintf(stdout, "\tDEBUG: in getEvent: line containd END:VEVENT\n");
            break;
        }

        if (startsWith(parse, ";")) {
            // lines that start with ';' are comments and should be ignored
            free(parse);
            parse = NULL;
            continue;
        } else if (startsWith(parse, "DTSTAMP")) {
            // creation date of event
            if (dtStamp) {
                fprintf(stdout, "\tDEBUG: in getEvent: found a second instance of a DTSTAMP property\n");
                deleteEvent(*event);
                *event = NULL;
                free(parse);
                return INV_EVENT;
            }
            dtStamp = true;

            DateTime stamp;
            if ((error = initializeDateTime(line, &stamp)) != OK) {
                fprintf(stdout, "DEBUG: in getEvent: initializeDateTime failed somehow\n");
                deleteEvent(*event);
                *event = NULL;
                free(parse);
                return error;
            }

            char *printDTS = printDate(&stamp);
            fprintf(stdout, "DEBUG: in getEvent: created DateTime Stamp: \"%s\"\n", printDTS);
            free(printDTS);

            (*event)->creationDateTime = stamp;
        } else if (startsWith(parse, "DTSTART")) {
            // start of event
            if (dtStart) {
                fprintf(stdout, "\tDEBUG: in getEvent: found a second instance of a DTSTART property\n");
                deleteEvent(*event);
                *event = NULL;
                free(parse);
                return INV_EVENT;
            }
            dtStart = true;

            DateTime start;
            if ((error = initializeDateTime(line, &start)) != OK) {
                fprintf(stdout, "DEBUG: in getEvent: initializeDateTime failed somehow\n");
                deleteEvent(*event);
                *event = NULL;
                free(parse);
                return error;
            }

            char *printDTStrt = printDate(&start);
            fprintf(stdout, "DEBUG: in getEvent: created DateTime Start: \"%s\"\n", printDTStrt);
            free(printDTStrt);

            (*event)->startDateTime = start;
        } else if (startsWith(parse, "UID")) {
            if (UID) {
                fprintf(stdout, "\tDEBUG: in getEvent: encountered a second UID property\n");
                deleteEvent(*event);
                *event = NULL;
                free(parse);
                return INV_EVENT;
            }
            UID = true;

            strcpy((*event)->UID, line + 4);
        } else if (startsWith(parse, "BEGIN:VALARM")) {
            Alarm *toAdd;
            if ((error = getAlarm(fp, &toAdd)) != OK) {
                fprintf(stdout, "\tDEBUG: in getEvent: encountered error when getting an Alarm\n");
                deleteEvent(*event);
                *event = NULL;
                free(parse);
                return error;
            }

            insertFront((*event)->alarms, (void *)toAdd);
        } else {
            Property *prop;
            if ((error = initializeProperty(line, &prop)) != OK) {
                fprintf(stdout, "DEBUG: in getEvent: initializeProperty failed somehow\n");
                deleteEvent(*event);
                *event = NULL;
                free(parse);
                return error;
            }

            if (prop == NULL) {
                fprintf(stdout, "\tDEBUG: in getEvent: encountered error when initializing property\n");
                deleteEvent(*event);
                *event = NULL;
                free(parse);
                return INV_CAL;
            }

            insertFront((*event)->properties, (void *)prop);
        }
    }

    free(parse);

    // the file can't end without hitting END:VEVENT (and also END:VCALENDAR)
    if (feof(fp)) {
        fprintf(stdout, "DEBUG: in getEvent: hit end of file before reaching an END:VEVENT\n");
        deleteAlarm(*event);
        *event = NULL;
        return INV_EVENT;
    }

    fprintf(stdout, "DEBUG: finished getEvent() successfully\n");
    return OK;
}


ICalErrorCode getAlarm(FILE *fp, Alarm **alarm) {
    char line[10000], *parse;
    bool trigger, action;
    ICalErrorCode error;
    parse = NULL;
    trigger = action = false;

    fprintf(stdout, "DEBUG: started getAlarm()\n");
    if ((error = initializeAlarm(alarm)) != OK) {
        fprintf(stdout, "DEBUG: in getAlarm: initializeAlarm() failed somehow\n");
        *alarm = NULL;
        return error;
    }

    while (!feof(fp)) {
        if (parse != NULL) {
            free(parse);
        }

        readFold(line, 10000, fp);
        parse = strUpperCopy(line);
        fprintf(stdout, "\tDEBUG: in getAlarm: unfolded, upper'd line: \"%s\"\n", parse);

        // This check can't be in the condition for the while loop, since
        // iCal files are case insensitive, and therefore the case must be
        // made uniform before checking.
        if (startsWith(parse, "END:VALARM")) {
            break;
        }

        if (startsWith(parse, ";")) {
            // lines that start with ';' are comments and should be ignored
            free(parse);
            continue;
        } else if (startsWith(parse, "TRIGGER")) {
            if (trigger) {
                fprintf(stdout, "\tDEBUG: in getAlarm: found a second instance of a TRIGGER property\n");
                deleteAlarm(*alarm);
                *alarm = NULL;
                free(parse);
                return INV_ALARM;
            }
            trigger = true;

            // -8 for the characters in 'TRIGGER:', +1 for null terminator
            (*alarm)->trigger = malloc(strlen(line) - 7);
            strcpy((*alarm)->trigger, line + 8);
            fprintf(stdout, "\tDEBUG: in getAlarm: trigger = \"%s\"\n", (*alarm)->trigger);
        } else if (startsWith(parse, "ACTION")) {
            if (action) {
                fprintf(stdout, "\tDEBUG: in getAlarm: found a second instance of a ACTION property\n");
                deleteAlarm(*alarm);
                *alarm = NULL;
                free(parse);
                return INV_ALARM;
            }
            action = true;

            strcpy((*alarm)->action, line + 7);
            fprintf(stdout, "\tDEBUG: in getAlarm: action = \"%s\"\n", (*alarm)->action);
        } else {
            Property *prop;
            if ((error = initializeProperty(line, &prop)) != OK) {
                fprintf(stdout, "DEBUG: in getAlarm: initializeProperty() failed somehow\n");
                deleteAlarm(*alarm);
                *alarm = NULL;
                free(parse);
                return error;
            }

            insertFront((*alarm)->properties, (void *)prop);
        }
    }

    free(parse);

    // the file can't end without hitting END:VALARM (and also END:VCALENDAR)
    if (feof(fp)) {
        fprintf(stdout, "DEBUG: in getAlarm: hit end of file before reaching an END:VALARM\n");
        deleteAlarm(*alarm);
        *alarm = NULL;
        return INV_ALARM;
    }

    fprintf(stdout, "DEBUG: finished getAlarm() successfully\n");
    return OK;
}

