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
#include "Initialize.h"

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
    ICalErrorCode error;
    bool version, prodID, method, beginCal, endCal;
    char *parse;
    version = prodID = method = beginCal = endCal = false;

    // filename can't be null or an empty string, and must end with the '.ics' extension
    if (fileName == NULL || strcmp(fileName, "") == 0 || !endsWith(fileName, ".ics")) {
        *obj = NULL;
        return INV_FILE;
    }

    fin = fopen(fileName, "r");

    // Check that file was found/opened correctly
    if (fin == NULL) {
        // On a failure, the obj argument is set to NULL and an error code is returned
        *obj = NULL;
        return INV_FILE;
    }

    if ((error = initializeCalendar(obj)) != OK) {
        return error;
    }

    char line[10000];
    while (!feof(fin)) {
        fprintf(stderr, "DEBUG: in createCalendar: version=%d, prodID=%d, method=%d, beginCal=%d, endCal=%d\n", \
               version, prodID, method, beginCal, endCal);
        readFold(line, 10000, fin);
        parse = strUpperCopy(line);

        fprintf(stderr, "DEBUG: in createCalendar: unfolded, capitalized line: \"%s\"\n", parse);
        
        if (startsWith(parse, ";")) {
            // lines starting with a semicolon (;) are comments, and
            // should be ignored
            free(parse);
            printf("\n");
            continue;
        }

        // Check if the END:VCALENDAR has been hit. If it has, and there is still more file to be read,
        // then something has gone wrong.
        if (endCal) {
            cleanup(obj, parse, fin);
            return INV_CAL;
        }

        // The first non-commented line must be BEGIN:VCALENDAR
        if (!beginCal && !startsWith(parse, "BEGIN:VCALENDAR")) {
            fprintf(stderr, "DEBUG: in createCalendar: file does not start with BEGIN:VCALENDAR\n");
            cleanup(obj, parse, fin);
            return INV_CAL;
        } else if (!beginCal) {
            fprintf(stderr, "DEBUG: in createCalendar: First non-comment line was BEGIN:VCALENDAR\n");
            beginCal = true;
            free(parse);
            continue;
        }
        

        // add properties, alarms, events, and other elements to the calendar
        if (startsWith(parse, "VERSION:")) {
            if (version) {
                cleanup(obj, parse, fin);
                return DUP_VER;
            }

            fprintf(stderr, "DEBUG: in createCalendar: found VERSION line: \"%s\"\n", line);
            char *endptr;
            // +8 to start conversion after the 'VERSION:' part of the string
            (*obj)->version = strtof(line + 8, &endptr);

            if (strlen(line + 8) == 0 || line+8 == endptr) {
                // VERSION property contains no data after the ':', or the data
                // could not be converted into a number
                cleanup(obj, parse, fin);
                return INV_VER;
            }

            fprintf(stderr, "DEBUG: in createCalendar: set version to %f\n", (*obj)->version);
            version = true;
        } else if (startsWith(parse, "PRODID:")) {
            if (prodID) {
                cleanup(obj, parse, fin);
                return DUP_PRODID;
            }

            // PRODID: contains no information
            if (strlen(line + 7) == 0) {
                cleanup(obj, parse, fin);
                return INV_PRODID;
            }

            // +7 to only copy characters past 'PRODID:' part of the string
            fprintf(stderr, "DEBUG: in createCalendar: found PRODID line: \"%s\"\n", line);
            strcpy((*obj)->prodID, line + 7);
            fprintf(stderr, "DEBUG: in createCalendar: set product ID to\"%s\"\n", (*obj)->prodID);
            prodID = true;
        } else if (startsWith(parse, "METHOD:")) {
            if (method) {
                cleanup(obj, parse, fin);
                return INV_CAL;
            }

            // METHOD: contains no information
            if (strlen(line + 7) == 0) {
                cleanup(obj, parse, fin);
                return INV_CAL;
            }

            fprintf(stderr, "DEBUG: in createCalendar: found METHOD line: \"%s\"\n", line);
            Property *methodProp;
            if ((error = initializeProperty(line, &methodProp)) != OK) {
                // something happened, and the property could not be created properly
                cleanup(obj, parse, fin);
                return INV_CAL;
            }

            insertFront((*obj)->properties, (void *)methodProp);
            method = true;
        } else if (startsWith(parse, "END:VCALENDAR")) {
            endCal = true;
        } else if (startsWith(parse, "BEGIN:VCALENDAR")) {
            cleanup(obj, parse, fin);
            return INV_CAL;
        } else if (startsWith(parse, "BEGIN:VEVENT")) {
            Event *event;
            if ((error = getEvent(fin, &event)) != OK) {
                // something happened, and the event could not be created properly
                cleanup(obj, parse, fin);
                return error;
            }

            insertFront((*obj)->events, (void *)event);
        } else if (startsWith(parse, "BEGIN:VALARM")) {
            // there can't be an alarm for an entire calendar
            fprintf(stderr, "DEBUG: in createCalendar: found an alarm not in an event\n");
            cleanup(obj, parse, fin);
            return INV_ALARM;
        } else {
            fprintf(stderr, "DEBUG: in createCalendar: found non-mandatory property: \"%s\"\n", line);
            Property *prop;
            if ((error = initializeProperty(line, &prop)) != OK) {
                // something happened, and the property could not be created properly
                cleanup(obj, parse, fin);
                return INV_CAL;
            }

            insertFront((*obj)->properties, (void *)prop);
        }

        free(parse);
        printf("\n");   // DEBUG:
    }
    fclose(fin);


    printf("\nDEBUG: in createCalendar after parsing file:\n\tversion=%d, prodID=%d, method=%d, beginCal=%d, endCal=%d\n", \
           version, prodID, method, beginCal, endCal);

    // Calendars require a few mandatory elements. If one does not have
    // any of these properties/lines, it is invalid.
    if (!prodID) {
        cleanup(obj, parse, NULL);
        return INV_PRODID;
    }
    if (!version) {
        cleanup(obj, parse, NULL);
        return INV_VER;
    }
    if (!endCal) {
        cleanup(obj, parse, NULL);
        return INV_CAL;
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
    char *toReturn = malloc(20000);

    // check for malloc failing
    if (toReturn == NULL) {
        fprintf(stderr, "CALL TO MALLOC FAILED IN printCalendar()\n");
        return NULL;
    }

    if (obj == NULL) {
        printf("OBJ IS NULL\n");
        return NULL;
    }

    char *eventListStr = toString(obj->events);
    char *propertyListStr = toString(obj->properties);

    // A neat little function I found that allows for string creation using printf
    // format specifiers. Makes stringing information together in a string like this
    // much easier than using strcat() repeatedly!
    snprintf(toReturn, 20000, "Start CALENDAR: {VERSION=%.2f, PRODID=%s, Start EVENTS={%s\n} End EVENTS, Start PROPERTIES={%s\n} End PROPERTIES}, End CALENDAR", \
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
            strcpy(toReturn, "Invalid files");
            break;

        case INV_VER:
            strcpy(toReturn, "Invalid version");
            break;

        case DUP_VER:
            strcpy(toReturn, "Duplicate version");
            break;

        case INV_PRODID:
            strcpy(toReturn, "Invalid product ID");
            break;

        case DUP_PRODID:
            strcpy(toReturn, "Duplicate product ID");
            break;

        case INV_EVENT:
            strcpy(toReturn, "Invalid event");
            break;

        case INV_DT:
            strcpy(toReturn, "Invalid date-time");
            break;

        case INV_ALARM:
            strcpy(toReturn, "Invalid alarm");
            break;

        case WRITE_ERROR:
            strcpy(toReturn, "Write error");
            break;

        case OTHER_ERROR:
            strcpy(toReturn, "Other error");
            break;

        default:
            sprintf(toReturn, "Unknown error: Found error with value %d", err);
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
    Event *e1 = (Event *)first;
    Event *e2 = (Event *)second;

    return strcmp(e1->UID, e2->UID);
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

    snprintf(toReturn, length, "EventUID: \"%s\", EventCreate: \"%s\", EventStart: \"%s\", EVENT_PROPERTIES: {%s\n} End EVENT_PROPERTIES, Start EVENT_ALARMS: {%s\n} End EVENT_ALARMS", \
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
 * Compares the 'action' properties of two alarms.
 */
int compareAlarms(const void* first, const void* second) {
    Alarm *a1 = (Alarm *)first;
    Alarm *a2 = (Alarm *)second;

    return strcmp(a1->action, a2->action);
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

    int length = strlen(al->action) + strlen(al->trigger) + strlen(props) + 100;
    char *toReturn = malloc(length);

    snprintf(toReturn, length, "AlarmAction: \"%s\", AlarmTrigger: \"%s\", Start ALARM_PROPERTIES: {%s\n} End ALARM_PROPERTIES", \
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
 * Compares the names of two properties.
 */
int compareProperties(const void* first, const void* second) {
    Property *p1 = (Property *)first;
    Property *p2 = (Property *)second;

    return strcmp(p1->propName, p2->propName);
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
 * Compare dates, then times if the dates are the same, then UTC values
 * if the times are the same as well.
 */
int compareDates(const void* first, const void* second) {
    DateTime *dt1 = (DateTime *)first;
    DateTime *dt2 = (DateTime *)second;
    int cmp;

    // if dates are the same, then compare times instead
    if ((cmp = strcmp(dt1->date, dt2->date)) == 0) {
        // if times are also the same, then compare UTC instead
        if ((cmp = strcmp(dt1->time, dt2->time)) == 0) {
            return dt1->UTC - dt2->UTC;
        }
        // if they are not, then return the time comparison below
    }
    // if they are not, then return the date comparison below

    return cmp;
}

/*
 */
char* printDate(void* toBePrinted) {
    if (!toBePrinted) {
        return NULL;
    }

    DateTime *dt = (DateTime *)toBePrinted;

    // 9 chars for date, + 7 for time, + 70 for rest of string = 86 bytes, round to 100 to be safe
    int length = 100;
    char *toReturn = malloc(length);

    snprintf(toReturn, length, "Date (YYYYMMDD): \"%s\", Time (HHMMSS): \"%s\", UTC?: %s", \
             dt->date, dt->time, (dt->UTC) ? "Yes" : "No");

    return realloc(toReturn, strlen(toReturn) + 1);
}

