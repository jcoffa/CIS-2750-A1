/************************************
 *  Name: Joseph Coffa              *
 *  Student #: 1007320              *
 *  Due Date: February 4, 2019      *
 *                                  *
 *  Assignment 1, CIS*2750          *
 *  Initialize.c                    *
 ************************************/

#include "Initialize.h"
#include "Parsing.h"

/*
 * Populates the DateTime structure 'dt' with data retrieved from the string 'line',
 * which should come from an iCalendar file.
 * Returns OK if no errors occurred, INV_DT if the line contains malformed DateTime data,
 * and OTHER_ERROR if the line is NULL.
 */
ICalErrorCode initializeDateTime(const char *line, DateTime *dt) {
    char data[300];
    const char delimData[] = ";:";
    const char delimTime[] = "Tt";

    if (line == NULL) {
        dt = NULL;
        return OTHER_ERROR;
    }

    int len = strlen(line);
    int colonIndex = strcspn(line, delimData);
    int tIndex = strcspn(line, delimTime);

    // the line contains no characters in 'delim', or the letter 't'
    // which is necessary to differentiate the date and time parts of a DateTime
    // or it follows FORM #3 of the DateTime forms (as stated in section 3.3.5
    // of the RFC 5545 iCal specification)
    if (colonIndex == len || tIndex == len || !isdigit(line[colonIndex+1])) {
        dt = NULL;
        return INV_DT;
    }

    // ignore everything before (and including) the property name and ':' or ';'
    strcpy(data, line + colonIndex + 1);

    // the first 8 characters is the date
    strncpy(dt->date, data, 8);
    (dt->date)[8] = '\0';   // strncpy does not automatically null-terminate

    // the next 6 characters after the "T" character is the time
    //fprintf(stdout, "\tDEBUG: in initializeDateTime, data + strcspn(data, \"%s\") = \"%s\"\n", delimTime, data + strcspn(data, delimTime)+1);
    strncpy(dt->time, data + strcspn(data, delimTime)+1, 6);
    (dt->time)[6] = '\0';   // strncpy does not automatically null-terminate

    dt->UTC = (endsWith(line, "Z") || endsWith(line, "z"));

    return OK;
}

/*
 * Allocates memory for a Property structure and populates it with data retrieved from the
 * string 'line', which should come from an iCalendar file.
 * Everything leading up to the first ':' or ';' becomes the propName, and everything after
 * becomes the propDescr.
 * Returns OK if no errors occurred, OTHER_ERROR if malloc fails or 'line' is NULL,
 * and INV_CAL if either the name or description is blank.
 */
ICalErrorCode initializeProperty(const char *line, Property **prop) {
    char name[200], descr[2000];
    const char delim[] = ";:";
    int firstDelim, length;

    if (line == NULL) {
        return OTHER_ERROR;
    }

    if ((length = strlen(line)) == 0) {
        return OTHER_ERROR;
    }

    //fprintf(stdout, "\tDEBUG: in initializeProperty: line = \"%s\"\n", line);
    firstDelim = strcspn(line, delim);

    // if these values are the same, then 'line' does not contain
    // any of the delimiting characters that are indicative of a property
    if (firstDelim == length) {
        // DateTime is malformed
        //fprintf(stdout, "\tDEBUG: in initializeProperty: delim characters \"%s\" were not present in the line: \"%s\"\n", delim, line);
        return INV_CAL;
    }

    strncpy(name, line, firstDelim);
    name[firstDelim] = '\0';    // strncpy does not automatically null-terminate

    strcpy(descr, line + firstDelim + 1);

    if (strlen(name) == 0 || strlen(descr) == 0) {
        // name or property value is missing
        return INV_CAL;
    }

    //fprintf(stdout, "DEBUG: in initializeProperty: name=\"%s\", descr=\"%s\"\n", name, descr);
    *prop = malloc(sizeof(Property) + strlen(descr) + 1);
    if (*prop == NULL) {
        // malloc failed
        return OTHER_ERROR;
    }

    strcpy((*prop)->propName, name);
    strcpy((*prop)->propDescr, descr);

    return OK;
}

/*
 * Allocates memory for an Alarm structure, and initializes its Property List.
 * Alarms have multiple properties across multiple lines, so their data
 * must be entered manually.
 * Memory is not allocated for its 'trigger', as it is dynamically allocated
 * to perfectly fit the length of its string.
 * Returns OK if no errors occurred, and OTHER_ERROR if any malloc calls fail.
 */
ICalErrorCode initializeAlarm(Alarm **alm) {
    *alm = malloc(sizeof(Alarm));
    if (*alm == NULL) {
        // malloc failed
        return OTHER_ERROR;
    }

    strcpy((*alm)->action, "");
    (*alm)->trigger = NULL;
    (*alm)->properties = initializeList(printProperty, deleteProperty, compareProperties);

    if ((*alm)->properties == NULL) {
        // list initialization failed
        return OTHER_ERROR;
    }

    return OK;
}

/*
 * Allocates memory for an Event structure, and initializes its Property List.
 * Events have mutliple properties across multiple lines, so their data
 * must be entered manually.
 * Returns OK if no errors occurred, and OTHER_ERROR if any malloc calls fail.
 */
ICalErrorCode initializeEvent(Event **evt) {
    *evt = malloc(sizeof(Event));
    if (*evt == NULL) {
        // malloc failed
        return OTHER_ERROR;
    }

    strcpy((*evt)->UID, "");
    strcpy((*evt)->creationDateTime.date, "");
    strcpy((*evt)->creationDateTime.time, "");
    (*evt)->creationDateTime.UTC = false;

    strcpy((*evt)->startDateTime.date, "");
    strcpy((*evt)->startDateTime.time, "");
    (*evt)->startDateTime.UTC = false;

    (*evt)->properties = initializeList(printProperty, deleteProperty, compareProperties);
    (*evt)->alarms = initializeList(printAlarm, deleteAlarm, compareAlarms);

    if ((*evt)->properties == NULL || (*evt)->alarms == NULL) {
        // list initialization failed
        return OTHER_ERROR;
    }

    return OK;
}

/*
 * Allocates memory for a Calendar structure, and initializes all of its Lists.
 * Calendar's have multiple properties across multiple lines, so their data
 * must be entered manually.
 * Returns OK if no errors occurred, and OTHER_ERROR if any malloc calls fail.
 */
ICalErrorCode initializeCalendar(Calendar **cal) {
    *cal = malloc(sizeof(Calendar));
    if (*cal == NULL) {
        // malloc failed
        return OTHER_ERROR;
    }

    (*cal)->version = 0.0;
    strcpy((*cal)->prodID, "");
    (*cal)->events = initializeList(printEvent, deleteEvent, compareEvents);
    (*cal)->properties = initializeList(printProperty, deleteProperty, compareProperties);

    if ((*cal)->events == NULL || (*cal)->properties == NULL) {
        // list initialization failed
        return OTHER_ERROR;
    }

    return OK;
}

