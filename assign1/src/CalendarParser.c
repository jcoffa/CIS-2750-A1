/************************************
 *  Name: Joseph Coffa              *
 *  Student #: 1007320              *
 *  Due Date: February 4, 2019      *
 *                                  *
 *  Assignment 1, CIS*2750          *
 *  CalendarParser.c                *
 ************************************/

#include "CalendarParser.h"
#include "LinkedListAPI.h"
#include "Parsing.h"

/** Function to create a Calendar object based on the contents of an iCalendar file.
 *@pre File name cannot be an empty string or NULL.  File name must have the .ics extension.
       File represented by this name must exist and must be readable.
 *@post Either:
        A valid calendar has been created, its address was stored in the variable obj, and OK was returned
		or
		An error occurred, the calendar was not created, all temporary memory was freed, obj was set to NULL, and the
		appropriate error code was returned
 *@return the error code indicating success or the error encountered when parsing the calendar
 *@param fileName - a string containing the name of the iCalendar file
 *@param a double pointer to a Calendar struct that needs to be allocated
**/
ICalErrorCode createCalendar(char* fileName, Calendar** obj) {
    Calendar *cal = *obj;   // one less * I have to remember to type
    FILE *fin;
    bool version, prodID, method;
    version = prodID = method = false;

    // file must end with the .ics extension
    if (!endsWith(fileName, ".ics")) {
        return INV_FILE;
    }

    fin = fopen(fileName, "r");

    // Check that file was found/opened correctly
    if (fin == NULL) {
        // On a failure, the obj argument is set to NULL and an error code is returned
        *obj = NULL;
        return INV_FILE;
    }

    // initialize the Calendar object
    cal = malloc(sizeof(Calendar));
    cal->events = initializeList(printEvent, deleteEvent, compareEvents);
    cal->properties = initializeList(printProperty, deleteProperty, compareProperties);

    // Most arrays in the various structures used in this program have a maximum length of
    // 1000 characters, so I'm assuming most lines won't go much longer than that (it is not
    // guaranteed that all lines are folded at 75 chars, since the iCal speciifcation
    // states that line folding is only recommended, and not a requirement.
    char line[2000];

    while (!feof(fin)) {
        printf("\tDEBUG: in createCalendar: version=%d, prodID=%d, method=%d\n", version, prodID, method);
        readFold(line, 2000, fin);
        
        if (startsWith(line, ";")) {
            // lines starting with a semicolon (;) are comments, and
            // should be ignored
            continue;
        }
        
        if (startsWith(line, "VERSION:")) {
            if (version) {
                deleteCalendar(cal);
                return DUP_VER;
            }

            printf("DEBUG: in createCalendar: found VERSION line: \"%s\"\n", line);
            // +8 to start conversion after the 'VERSION:' part of the string
            cal->version = strtof(trimWhitespace(line + 8), NULL);
            printf("DEBUG: in createCalendar: set version to %f\n", cal->version);
            version = true;
        } else if (startsWith(line, "PRODID:")) {
            if (prodID) {
                deleteCalendar(cal);
                return DUP_PRODID;
            }

            // +7 to only copy characters past 'PRODID:' part of the string
            printf("DEBUG: in createCalendar: found PRODID line: \"%s\"\n", line);
            strcpy(cal->prodID, line + 7);
            printf("DEBUG: in createCalendar: set product ID to\"%s\"\n", cal->prodID);
            prodID = true;
        } else if (startsWith(line, "METHOD:")) {
            if (method) {
                deleteCalendar(cal);
                return OTHER_ERROR;
            }

            printf("DEBUG: in createCalendar: found METHOD line: \"%s\"\n", line);
            // TODO add method to the generic property List
            method = true;
        } else {
            printf("DEBUG: in createCalendar: found non-mandatory property: \"%s\"\n", line);
            // TODO add property to the generic property List
        }
    }
    
    fclose(fin);

    // A calendar object requires a ProductID and a Version number.
    // If the file does not have one, it is invalid.
    if (!prodID) {
        deleteCalendar(cal);
        return INV_PRODID;
    }
    if (!version) {
        deleteCalendar(cal);
        return INV_VER;
    }

    // the file has been parsed, mandatory properties have been found,
    // and the calendar is valid
    return OK;
}


/** Function to delete all calendar content and free all the memory.
 *@pre Calendar object exists, is not null, and has not been freed
 *@post Calendar object had been freed
 *@return none
 *@param obj - a pointer to a Calendar struct
**/
void deleteCalendar(Calendar* obj) {
    freeList(obj->events);
    freeList(obj->properties);
    free(obj);
}


/** Function to create a string representation of a Calendar object.
 *@pre Calendar object exists, is not null, and is valid
 *@post Calendar has not been modified in any way, and a string representing the Calendar contents has been created
 *@return a string contaning a humanly readable representation of a Calendar object
 *@param obj - a pointer to a Calendar struct
**/
char* printCalendar(const Calendar* obj) {
    char *toReturn = malloc(2000);
    char *eventListStr = toString(obj->events);
    char *propertyListStr = toString(obj->properties);

    // A neat little function I found that allows for string creation using printf
    // format specifiers. Makes stringing information together in a string like this
    // much easier than using strcat() repeatedly!
    snprintf(toReturn, 2000, "Calendar: {VERSION=%.2f, PRODID=%s, EVENTS={%s\n} End EVENTS, PROPERTIES={%s\n} End PROPERTIES}", \
             obj->version, obj->prodID, eventListStr, propertyListStr);

    free(eventListStr);
    free(propertyListStr);

    return realloc(toReturn, strlen(toReturn) + 1);
}


/** Function to "convert" the ICalErrorCode into a humanly redabale string.
 *@return a string containing a humanly readable representation of the error code by indexing into
          the descr array using the error code enum value as an index
 *@param err - an error code
**/
char* printError(ICalErrorCode err) {
    // FIXME this doesn't do what the description specificly says;
    // no array named 'desc' is found anywhere in the assignment outline,
    // so I can't exactly index it.
    char *toReturn = malloc(200);
    
    switch ((int)err) {
        case OK:
            strcpy(toReturn, "OK: No errors");
            break;

        case INV_FILE:
            strcpy(toReturn, "INV_FILE: File not found, or could not be opened");
            break;

        case INV_VER:
            strcpy(toReturn, "INV_VER: The VERSION: property of the Calendar was not found or contained data that could not be coerced into a floating point number");
            break;

        case DUP_VER:
            strcpy(toReturn, "DUP_VER: The VERSION: property of the Calendar was found in more than one location");
            break;

        case INV_PRODID:
            strcpy(toReturn, "INV_PRODID: The PRODID: property of the Calendar was not found or had invalid syntax");
            break;

        case DUP_PRODID:
            strcpy(toReturn, "DUP_PRODID: The PRODID: property of the Calendar was found in more than one location");
            break;

        case INV_EVENT:
            strcpy(toReturn, "INV_EVENT: An Event in the Calendar contains invalid information, has syntax errors, or does not contain all required data for an Event");
            break;

        case INV_DT:
            strcpy(toReturn, "INV_DT: Any of the possible Date-Time (DT__) properties of an element in the Calendar contains invalid information, has syntax errors, or does not contain all required data");
            break;

        case INV_ALARM:
            strcpy(toReturn, "INV_ALARM: An Alarm in the Calendar contains invalid information, has syntax errors, or does not contain all required data for an Alarm");
            break;

        case WRITE_ERROR:
            strcpy(toReturn, "WRITE_ERROR: Some data in a Calendar object could not be written to a file properyl or at all");
            break;

        case OTHER_ERROR:
            strcpy(toReturn, "OTHER_ERROR: No details available");
            break;
    }

    return realloc(toReturn, strlen(toReturn) + 1);
}


/** Function to writing a Calendar object into a file in iCalendar format.
 *@pre Calendar object exists, is not null, and is valid
 *@post Calendar has not been modified in any way, and a file representing the
        Calendar contents in iCalendar format has been created
 *@return the error code indicating success or the error encountered when parsing the calendar
 *@param obj - a pointer to a Calendar struct
 **/
ICalErrorCode writeCalendar(char* fileName, const Calendar* obj);


/** Function to validating an existing a Calendar object
 *@pre Calendar object exists and is not null
 *@post Calendar has not been modified in any way
 *@return the error code indicating success or the error encountered when validating the calendar
 *@param obj - a pointer to a Calendar struct
 **/
ICalErrorCode validateCalendar(const Calendar* obj);


// ************* List helper functions - MUST be implemented ***************

/*
 */
void deleteEvent(void* toBeDeleted) {
    if (!toBeDeleted) {
        return;
    }

    Event *ev = (Event *)toBeDeleted;

    // DateTime objects don't need to be freed (they aren't
    // stored as pointers in an Event struct)
    freeList(ev->properties);
    freeList(ev->alarms);

    free(ev);
}

/*
 */
int compareEvents(const void* first, const void* second) {
    // TODO
    printf("\tDEBUG: compareEvents is unimplemented\n");
    return 0;
}

/*
 */
char* printEvent(void* toBePrinted) {
    if (!toBePrinted) {
        return NULL;
    }

    Event *ev = (Event *)toBePrinted;

    // DateTime's and Lists have their own print functions
    char *createStr = printDate(&(ev->creationDateTime));
    char *startStr = printDate(&(ev->startDateTime));
    char *propsStr = toString(ev->properties);
    char *alarmsStr = toString(ev->alarms);

    int length = strlen(createStr) + strlen(startStr) + strlen(propsStr) + strlen(alarmsStr) + 130;
    char *toReturn = malloc(length);

    snprintf(toReturn, length, "EventUID: \"%s\", EventCreate: \"%s\", EventStart: \"%s\", EventProps: \"%s\", EventAlarms: \"%s\"", \
             ev->UID, createStr, startStr, propsStr, alarmsStr);

    // Free dynamically allocated print strings
    free(createStr);
    free(startStr);
    free(propsStr);
    free(alarmsStr);

    return realloc(toReturn, strlen(toReturn) + 1);
}


/*
 */
void deleteAlarm(void* toBeDeleted) {
    if (!toBeDeleted) {
        return;
    }

    Alarm *al = (Alarm *)toBeDeleted;

    free(al->trigger);
    freeList(al->properties);

    free(al);
}

/*
 */
int compareAlarms(const void* first, const void* second) {
    // TODO
    printf("\tDEBUG: compareAlarms is unimplemented\n");
    return 0;
}

/*
 */
char* printAlarm(void* toBePrinted) {
    if (!toBePrinted) {
        return NULL;
    }

    Alarm *al = (Alarm *)toBePrinted;

    // Lists have their own print function
    char *props = toString(al->properties);

    int length = strlen(al->action) + strlen(al->trigger) + strlen(props) + 80;
    char *toReturn = malloc(length);

    snprintf(toReturn, length, "AlarmAction: \"%s\", AlarmTrigger: \"%s\", AlarmProps: \"%s\"", \
             al->action, al->trigger, props);

    // Free dynamically allocated print string
    free(props);

    return realloc(toReturn, strlen(toReturn) + 1);
}


/*
 */
void deleteProperty(void* toBeDeleted) {
    // Properties are alloc'd in one block and none of their
    // members needs to be freed in any special way, so no
    // type casting or calling other functions is necessary
    if (toBeDeleted) {
        free(toBeDeleted);
    }
}

/*
 */
int compareProperties(const void* first, const void* second) {
    // TODO
    printf("\tDEBUG: compareProperties is unimplemented\n");
    return 0;
}

/*
 */
char* printProperty(void* toBePrinted) {
    if (!toBePrinted) {
        return NULL;
    }

    Property *prop = (Property *)toBePrinted;

    int length = strlen(prop->propDescr) + 100;
    char *toReturn = malloc(length);

    snprintf(toReturn, length, "PropName: \"%s\", PropDescr: \"%s\"", prop->propName, prop->propDescr);

    return realloc(toReturn, strlen(toReturn) + 1);
}


/*
 */
void deleteDate(void* toBeDeleted) {
    // DateTimes are alloc'd in one block and none of their
    // members needs to be freed in any special way, so no
    // type casting or calling other functions is necessary
    if (toBeDeleted) {
        free(toBeDeleted);
    }
}

/*
 */
int compareDates(const void* first, const void* second) {
    // TODO
    printf("\tDEBUG: compareDates is unimplemented\n");
    return 0;
}

/*
 */
char* printDate(void* toBePrinted) {
    if (!toBePrinted) {
        return NULL;
    }

    DateTime *dt = (DateTime *)toBePrinted;

    // 9 chars for date,+ 7 for time, + 70 for rest of string = 86 bytes, round to 90
    int length = 90;
    char *toReturn = malloc(length);

    snprintf(toReturn, length, "Date (YYYYMMDD): \"%s\", Time (HHMMSS): \"%s\", UTC?: %s", \
             dt->date, dt->time, (dt->UTC) ? "Yes" : "No");

    return realloc(toReturn, strlen(toReturn) + 1);
}

