/************************************
 *  Name: Joseph Coffa              *
 *  Student #: 1007320              *
 *  Due Date: February 4, 2019      *
 *                                  *
 *  Assignment 1, CIS*2750          *
 *  CalendarParser.c                *
 ************************************/

// FIXME NO PATHS IN INCLUDE STATEMENTS
//       Use shared libraries after you figure out how to do that
#include "../../CalendarParser.h"
#include "../../LinkedListAPI.h"
#include "../include/Parsing.h"

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
    *obj = malloc(sizeof(Calendar));
    (*obj)->events = initializeList(printEvent, deleteEvent, compareEvents);
    (*obj)->properties = initializeList(printProperty, deleteProperty, compareProperties);

    char line[90], buffer[1100] = "";
    while (fgets(line, 90, fin)) {
        printf("\tDEBUG: in createCalendar: version=%d, prodID=%d, method=%d\n", version, prodID, method);
        // TODO only operate on the second last line read, instead of the
        // most recent one (since folding has to be taken into account, and you don't
        // know if a line is folded until you read the next line).
        // If the most recent line read is folded, and the one before it wasn't,
        // it is now known where the fold starts.
        if (startsWith(line, " ")) {
            // TODO concatenate, keep reading until one doesn't start with a space,
            // after which unfold the line and store the information where it needs to go.
            //concat(buffer, line + 1); // +1 to ignore leading space from the fold
            printf("\tDEBUG: in cteayeCalendar: discovered line that is indicitive of a fold: \"%s\"\n", line);
            continue;
        }

        // TODO handle that whole 'fold' nonsense described in the comment above
        // unfold(buffer);
        
        if (startsWith(line, "VERSION:")) {
            if (version) {
                deleteCalendar(*obj);
                return DUP_VER;
            }

            printf("DEBUG: in createCalendar: found VERSION line: \"%s\"\n", line);
            // +8 to start conversion after the 'VERSION:' part of the string
            (*obj)->version = strtof(trimWhitespace(line + 8), NULL);
            printf("DEBUG: in createCalendar: set version to %f\n", (*obj)->version);
            version = true;
        } else if (startsWith(line, "PRODID:")) {
            if (prodID) {
                deleteCalendar(*obj);
                return DUP_PRODID;
            }

            // +7 to only copy characters past 'PRODID:' part of the string
            printf("DEBUG: in createCalendar: found PRODID line: \"%s\"\n", line);
            strcpy((*obj)->prodID, line + 7);
            printf("DEBUG: in createCalendar: set product ID to\"%s\"\n", (*obj)->prodID);
            prodID = true;
        } else if (startsWith(line, "METHOD:")) {
            if (method) {
                deleteCalendar(*obj);
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
        deleteCalendar(*obj);
        return INV_PRODID;
    }
    if (!version) {
        deleteCalendar(*obj);
        return INV_PRODID;
    }

    return OK;
}


/** Function to delete all calendar content and free all the memory.
 *@pre Calendar object exists, is not null, and has not been freed
 *@post Calendar object had been freed
 *@return none
 *@param obj - a pointer to a Calendar struct
**/
void deleteCalendar(Calendar* obj) {
    // FIXME this leaks memory due to the freeList() functions
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
    //char *eventListStr = toString(obj->events);
    //char *propertyListStr = toString(obj->properties);

    // A neat little function I found that allows for string creation using printf
    // format specifiers. Makes stringing information together in a string like this
    // much easier than using strcat() repeatedly!
    //snprintf(toReturn, 2000, "Calendar: {VERSION=%.2f, PRODID=%s, EVENTS={%s}, PROPERTIES={%s}}", \
    //         obj->version, obj->prodID, eventListStr, propertyListStr);
    snprintf(toReturn, 2000, "Calendar: {VERSION=%.2f, PRODID=%s}", obj->version, obj->prodID);

    // TODO get strings from event and properties Lists, save them both
    // into variables, concatenate them to the main string, and then
    // free the event and property List string variables.
    // I've tested it out, and freeing a dynamically allocated string after
    // it has been concatenated onto another string is safe, correctly
    // frees the memory, and does not break the newly concatenated string.
    //free(eventListStr);
    //free(propertyListStr);

    toReturn = realloc(toReturn, strlen(toReturn) + 1);
    return toReturn;
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
    // TODO
    printf("\tDEBUG: deleteEvent is unimplemented\n");
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
    // TODO
    printf("\tDEBUG: printEvent is unimplemented\n");
    char *toReturn = malloc(50);
    strcpy(toReturn, "printEvent - TODO");
    return realloc(toReturn, strlen(toReturn) + 1);
}


/*
 */
void deleteAlarm(void* toBeDeleted) {
    // TODO
    printf("\tDEBUG: deleteAlarms is unimplemented\n");
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
    // TODO
    printf("\tDEBUG: printAlarm is unimplemented\n");
    char *toReturn = malloc(50);
    strcpy(toReturn, "printAlarm - TODO");
    return realloc(toReturn, strlen(toReturn) + 1);
}


/*
 */
void deleteProperty(void* toBeDeleted) {
    // TODO
    printf("\tDEBUG: deleteProperty is unimplemented\n");
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
    // TODO
    printf("\tDEBUG: printProperty is unimplemented\n");
    char *toReturn = malloc(50);
    strcpy(toReturn, "printProperty - TODO");
    return realloc(toReturn, strlen(toReturn) + 1);
}


/*
 */
void deleteDate(void* toBeDeleted) {
    // TODO
    printf("\tDEBUG: deleteDate is unimplemented\n");
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
    // TODO
    printf("\tDEBUG: printDate is unimplemented\n");
    char *toReturn = malloc(50);
    strcpy(toReturn, "printDate - TODO");
    return realloc(toReturn, strlen(toReturn) + 1);
}
