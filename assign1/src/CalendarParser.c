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
    FILE *fin = fopen(fileName, "r");

    // Check that file was found/opened correctly
    if (fin == NULL) {
        // On a failure, the obj argument is set to NULL and an error code is returned
        *obj = NULL;
        return INV_FILE;
    }

    *obj = malloc(sizeof(Calendar));
    // TODO initialize the List of events and the List of properties.

    char line[90];
    while (fgets(line, 90, fin)) {
        if (startsWith(line, " ")) {
            // TODO concatenate, keep reading until one doesn't start with a space,
            // after which unfold the line and store the information where it needs to go.
            continue;
        }

        // remove the trailing newline
        line[strcspn(line, "\r\n")] = '\0';
        
        if (startsWith(line, "VERSION:")) {
            printf("DEBUG: in createCalendar: found VERSION line: \"%s\"\n", line);
            // +8 to start conversion after the 'VERSION:' part of the string
            (*obj)->version = strtof(trimWhitespace(line + 8), NULL);
            printf("DEBUG: in createCalendar: set version to %f\n", (*obj)->version);
        } else if (startsWith(line, "PRODID:")) {
            // +7 to only copy characters past 'PRODID:' part of the string
            printf("DEBUG: in createCalendar: found PRODID line: \"%s\"\n", line);
            strcpy((*obj)->prodID, line+7);
            printf("DEBUG: in createCalendar: set product ID to\"%s\"\n", (*obj)->prodID);
        } else {
            printf("DEBUG: in createCalendar: found line that was unaccounted for: \"%s\"\n", line);
        }
    }
    
    fclose(fin);
    return OK;
}


/** Function to delete all calendar content and free all the memory.
 *@pre Calendar object exists, is not null, and has not been freed
 *@post Calendar object had been freed
 *@return none
 *@param obj - a pointer to a Calendar struct
**/
void deleteCalendar(Calendar* obj) {
    // TODO free the event List and the property List
    free(obj);
}


/** Function to create a string representation of a Calendar object.
 *@pre Calendar object exists, is not null, and is valid
 *@post Calendar has not been modified in any way, and a string representing the Calendar contents has been created
 *@return a string contaning a humanly readable representation of a Calendar object
 *@param obj - a pointer to a Calendar struct
**/
char* printCalendar(const Calendar* obj) {
    char *toReturn = malloc(1100);

    // A neat little function I found that allows for string creation using printf
    // format specifiers. Makes stringing information together in a string like this
    // much easier than using strcat() repeatedly!
    snprintf(toReturn, 1100, "Calendar: {VERSION=%.2f, PRODID=%s}", obj->version, obj->prodID);

    toReturn = realloc(toReturn, strlen(toReturn) + 1);
    return toReturn;
}


/** Function to "convert" the ICalErrorCode into a humanly redabale string.
 *@return a string containing a humanly readable representation of the error code by indexing into
          the descr array using the error code enum value as an index
 *@param err - an error code
**/
char* printError(ICalErrorCode err);


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
void deleteEvent(void* toBeDeleted);
int compareEvents(const void* first, const void* second);
char* printEvent(void* toBePrinted);

void deleteAlarm(void* toBeDeleted);
int compareAlarms(const void* first, const void* second);
char* printAlarm(void* toBePrinted);

void deleteProperty(void* toBeDeleted);
int compareProperties(const void* first, const void* second);
char* printProperty(void* toBePrinted);

void deleteDate(void* toBeDeleted);
int compareDates(const void* first, const void* second);
char* printDate(void* toBePrinted);
