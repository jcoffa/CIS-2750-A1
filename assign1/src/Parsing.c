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
 * Returns OK on a success, INV_FILE if imvalid line endings are found, and any other
 * relevant error if an error is found (for example, INV_CAL if an empty line is found)
 */
ICalErrorCode readFold(char *unfolded, int size, FILE *fp) {
    char buf[size];
    bool foundFold = false;
    bool allWhitespace;
    int sizeLeft = size;
    int bufLength;

    // clear 'unfolded' for more reliable concatenation
    strcpy(unfolded, "");

    while (fgets(buf, sizeLeft, fp)) {
        //fprintf(stdout, "\tDEBUG: in readFold: raw buf = \"%s\"\n", buf);

        // check if the line is entirely blank lines
        allWhitespace = true;
        for (int i = 0; i < strlen(buf); i++) {
            if (!isspace(buf[i])) {
                //fprintf(stdout, "\tDEBUG: in readFold: found non-whitespace character: %c - %d\n", buf[i], (unsigned int)buf[i]);
                allWhitespace = false;
                break;
            }
        }
        if (allWhitespace) {
            //fprintf(stdout, "\tDEBUG: in readFold: found an all-whitespace line\n");
            // blank lines are not allowed
            return INV_CAL;
        }

        if (!endsWith(buf, "\r\n")) {
            // line endings are incorrect
            return INV_FILE;
        }

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

    unfold(unfolded);
    trimWhitespace(unfolded);
    return OK;
}


/* XXX XXX XXX
 * Ok listen. I'm warning you right now, this function uses goto statements. But hear me out for a second.
 * There are LOTS of fail cases for this function. iCal files can go horribly wrong in tons of ways.
 * On a failure, memory needs to be freed, errors need to be returned, EVERY TIME an error with the
 * file is found. So those 3-4 cleanup statements are repeated several times in the entire function.
 * I personally am against making lots of similar-looking cleanup functions, since it clutters up
 * the function-space of the whole project.
 * And so, the goto statement reigns supreme (also, Nikitenko brought up how he thinks goto statements
 * aren't the complete devil 100% of the time, just 99% of the time, and can be useful for scenarios
 * precisely like this one.)
 * XXX XXX XXX */
ICalErrorCode getEvent(FILE *fp, Event **event) {
    char line[10000], *parse;
    ICalErrorCode error;
    bool dtStamp, dtStart, UID, endEvent;
    parse = NULL;
    dtStamp = dtStart = UID = false;

    //fprintf(stdout, "\tDEBUG: Started getEvent()\n");

    if ((error = initializeEvent(event)) != OK) {
        return error;
    }

    while (!feof(fp)) {
        if (parse != NULL) {
            free(parse);
            parse = NULL;
        }

        if ((error = readFold(line, 10000, fp)) != OK) {
            goto CLEANEV;
        }

        parse = strUpperCopy(line);
        //fprintf(stdout, "\tDEBUG: in getEvent: unfolded, upper'd line: \"%s\"\n", parse);

        // This check can't be in the condition for the while loop, since
        // iCal files are case insensitive, and therefore the case must be
        // made uniform (i.e. put through strUpper) before checking.
        if (strcmp(parse, "END:VEVENT") == 0) {
            //fprintf(stdout, "\tDEBUG: in getEvent: line containd END:VEVENT\n");
            endEvent = true;
            break;
        }

        if (strcmp(parse, "END:VCALENDAR") == 0) {
            //fprintf(stdout, "\tDEBUG: in getEvent: hit the end of the calendar before the end of the event");
            error = INV_EVENT;
            goto CLEANEV;
        }

        if (startsWith(parse, ";")) {
            // lines that start with ';' are comments and should be ignored
            free(parse);
            parse = NULL;
            continue;
        } else if (startsWith(parse, "DTSTAMP")) {
            // creation date of event
            if (dtStamp) {
                //fprintf(stdout, "\tDEBUG: in getEvent: found a second instance of a DTSTAMP property\n");
                error = INV_EVENT;
                goto CLEANEV;
            }
            dtStamp = true;

            DateTime stamp;
            if ((error = initializeDateTime(line, &stamp)) != OK) {
                //fprintf(stdout, "DEBUG: in getEvent: initializeDateTime failed somehow\n");
                goto CLEANEV;
            }

            //char *printDTS = printDate(&stamp);
            //fprintf(stdout, "DEBUG: in getEvent: created DateTime Stamp: \"%s\"\n", printDTS);
            //free(printDTS);

            (*event)->creationDateTime = stamp;
        } else if (startsWith(parse, "DTSTART")) {
            // start of event
            if (dtStart) {
                //fprintf(stdout, "\tDEBUG: in getEvent: found a second instance of a DTSTART property\n");
                error = INV_EVENT;
                goto CLEANEV;
            }
            dtStart = true;

            DateTime start;
            if ((error = initializeDateTime(line, &start)) != OK) {
                //fprintf(stdout, "DEBUG: in getEvent: initializeDateTime failed somehow\n");
                goto CLEANEV;
            }

            //char *printDTStrt = printDate(&start);
            //fprintf(stdout, "DEBUG: in getEvent: created DateTime Start: \"%s\"\n", printDTStrt);
            //free(printDTStrt);

            (*event)->startDateTime = start;
        } else if (startsWith(parse, "UID")) {
            if (UID) {
                //fprintf(stdout, "\tDEBUG: in getEvent: encountered a second UID property\n");
                error = INV_EVENT;
                goto CLEANEV;
            }

            // UID is empty
            if (strlen(line+4) == 0) {
                error = INV_EVENT;
                goto CLEANEV;
            }

            UID = true;

            strcpy((*event)->UID, line + 4);
        } else if (strcmp(parse, "BEGIN:VALARM") == 0) {
            Alarm *toAdd;
            if ((error = getAlarm(fp, &toAdd)) != OK) {
                //fprintf(stdout, "\tDEBUG: in getEvent: encountered error when getting an Alarm\n");
                goto CLEANEV;
            }

            insertFront((*event)->alarms, (void *)toAdd);
        } else if (strcmp(parse, "END:VALARM") == 0) {
            //fprintf(stdout, "\tDEBUG: in getEvent: found duplicate end of alarm: \"%s\"\n", line);
            error = INV_EVENT;
            goto CLEANEV;
        } else if (strcmp(parse, "BEGIN:VEVENT") == 0) {
            //fprintf(stdout, "\tDEBUG: in getEvent: found start of new event \"%s\"\n", line);
            error = INV_EVENT;
            goto CLEANEV;
        } else {
            Property *prop;
            if ((error = initializeProperty(line, &prop)) != OK) {
                //fprintf(stdout, "DEBUG: in getEvent: initializeProperty failed somehow\n");
                if (error != OTHER_ERROR) {
                    error = INV_EVENT;
                }
                goto CLEANEV;
            }

            if (prop == NULL) {
                //fprintf(stdout, "\tDEBUG: in getEvent: encountered error when initializing property\n");
                error = INV_CAL;
                goto CLEANEV;
            }

            insertFront((*event)->properties, (void *)prop);
        }
    }

    free(parse);
    parse = NULL;

    // the file can't end without hitting END:VEVENT (and also END:VCALENDAR)
    // along with a few other mandatory propeprties
    if (!UID || !dtStart || !dtStamp || !endEvent) {
        //fprintf(stdout, "\tDEBUG: in getEvent: missing a mandatory property: uid=%d, dtStart=%d, dtStamp=%d, endEvent=%d\n", UID, dtStart, dtStamp, endEvent);
        error = INV_EVENT;
        goto CLEANEV;
    }

    //fprintf(stdout, "DEBUG: finished getEvent() successfully\n");
    return OK;

    // Event cleanup
CLEANEV:    deleteEvent(*event);
            *event = NULL;
            if (parse != NULL) {
                free(parse);
            }
            return error;
}


/* XXX XXX XXX
 * Ok listen. I'm warning you right now, this function uses goto statements. But hear me out for a second.
 * There are LOTS of fail cases for this function. iCal files can go horribly wrong in tons of ways.
 * On a failure, memory needs to be freed, errors need to be returned, EVERY TIME an error with the
 * file is found. So those 3-4 cleanup statements are repeated several times in the entire function.
 * I personally am against making lots of similar-looking cleanup functions, since it clutters up
 * the function-space of the whole project.
 * And so, the goto statement reigns supreme (also, Nikitenko brought up how he thinks goto statements
 * aren't the complete devil 100% of the time, just 99% of the time, and can be useful for scenarios
 * precisely like this one.)
 * XXX XXX XXX */
ICalErrorCode getAlarm(FILE *fp, Alarm **alarm) {
    char line[10000], *parse;
    bool trigger, action, endAlarm;
    ICalErrorCode error;
    parse = NULL;
    trigger = action = false;

    //fprintf(stdout, "DEBUG: started getAlarm()\n");
    if ((error = initializeAlarm(alarm)) != OK) {
        //fprintf(stdout, "DEBUG: in getAlarm: initializeAlarm() failed somehow\n");
        *alarm = NULL;
        return error;
    }

    while (!feof(fp)) {
        if (parse != NULL) {
            free(parse);
            parse = NULL;
        }

        if ((error = readFold(line, 10000, fp)) != OK) {
            goto CLEANAL;
        }

        parse = strUpperCopy(line);
        //fprintf(stdout, "\tDEBUG: in getAlarm: unfolded, upper'd line: \"%s\"\n", parse);

        // This check can't be in the condition for the while loop, since
        // iCal files are case insensitive, and therefore the case must be
        // made uniform before checking.
        if (strcmp(parse, "END:VALARM") == 0) {
            //fprintf(stdout, "\tDEBUG: in getAlarm: hit END:VALARM\n");
            endAlarm = true;
            break;
        }

        if (strcmp(parse, "END:VCALENDAR") == 0) {
            //fprintf(stdout, "\tDEBUG: in getAlarm: hit END:VCALENDAR\n");
            error = INV_ALARM;
            goto CLEANAL;
        }

        if (startsWith(parse, ";")) {
            // lines that start with ';' are comments and should be ignored
            free(parse);
            parse = NULL;
            continue;
        } else if (startsWith(parse, "TRIGGER")) {
            if (trigger) {
                //fprintf(stdout, "\tDEBUG: in getAlarm: found a second instance of a TRIGGER property\n");
                error = INV_ALARM;
                goto CLEANAL;
            }
            trigger = true;

            if (strlen(line+8) == 0) {
                //fprintf(stdout, "\tDEBUG: in getAlarm: TRIGGER property is empty\n");
                error = INV_ALARM;
                goto CLEANAL;
            }

            // -8 for the characters in 'TRIGGER:', +1 for null terminator
            (*alarm)->trigger = malloc(strlen(line) - 7);
            strcpy((*alarm)->trigger, line + 8);
            //fprintf(stdout, "\tDEBUG: in getAlarm: trigger = \"%s\"\n", (*alarm)->trigger);
        } else if (startsWith(parse, "ACTION")) {
            if (action) {
                //fprintf(stdout, "\tDEBUG: in getAlarm: found a second instance of a ACTION property\n");
                error = INV_ALARM;
                goto CLEANAL;
            }
            action = true;

            if (strlen(line+7) == 0) {
                //fprintf(stdout, "\tDEBUG: in getAlarm: ACTION property is empty\n");
                error = INV_ALARM;
                goto CLEANAL;
            }

            strcpy((*alarm)->action, line + 7);
            //fprintf(stdout, "\tDEBUG: in getAlarm: action = \"%s\"\n", (*alarm)->action);
        } else if (strcmp(parse, "BEGIN:VALARM") == 0 || strcmp(parse, "BEGIN:VEVENT") == 0) {
            //fprintf(stdout, "\tDEBUG: in getAlarm: found a start of another alarm or event: \"%s\"\n", line);
            error = INV_ALARM;
            goto CLEANAL;
        } else {
            Property *prop;
            if ((error = initializeProperty(line, &prop)) != OK) {
                //fprintf(stdout, "DEBUG: in getAlarm: initializeProperty() failed somehow\n");
                if (error != OTHER_ERROR) {
                    error = INV_ALARM;
                }
                goto CLEANAL;
            }

            insertFront((*alarm)->properties, (void *)prop);
        }
    }

    free(parse);
    parse = NULL;

    // the file can't end without hitting END:VALARM (and also END:VCALENDAR)
    // and a few other mandatory properties
    if (!trigger || !action | !endAlarm) {
        error = INV_ALARM;
        goto CLEANAL;
    }

    return OK;

    // Alarm cleanup
CLEANAL:    deleteAlarm(*alarm);
            *alarm = NULL;
            if (parse != NULL) {
                free(parse);
            }
            return error;
}

