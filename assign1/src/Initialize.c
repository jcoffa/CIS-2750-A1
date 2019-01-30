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


// TODO return ICaleErrorCodes and pass in address of structure to function
// and allocate memory there (like in createCalendar)

/*
 * Populates the DateTime structure 'dt' with data retrieved from the string 'line',
 * which should come from an iCalendar file.
 */
void initializeDateTime(const char *line, DateTime *dt) {
    char data[200];
    const char delimData[] = ";:";
    const char delimTime[] = "Tt";

    if (line == NULL) {
        dt = NULL;
        return;
    }

    printf("\tDEBUG: in initializeDateTime: line = \"%s\"\n", line);

    int len = strlen(line);

    // the line contains no characters in 'delim', or the letter t
    // which is necessary to differentiate the date and time parts of a DateTime
    if ((strcspn(line, delimData) == len) || (strcspn(line, delimTime) == len)) {
        dt = NULL;
        return;
    }

    // ignore everything before (and including) the property name and ':' or ';'
    strcpy(data, line + strcspn(line, delimData) + 1);

    // the first 8 characters is the date
    strncpy(dt->date, data, 8);
    (dt->date)[8] = '\0';   // strncpy does not automatically null-terminate

    // the next 6 characters after the "T" character is the time
    strncpy(dt->time, data + strcspn(data, delimTime)+1, 6);
    (dt->time)[7] = '\0';   // strncpy does not automatically null-terminate

    dt->UTC = (endsWith(line, "Z") || endsWith(line, "z"));
}

/*
 * Allocates memory for a Property structure and populates it with data retrieved from the
 * string 'line', which should come from an iCalendar file.
 * Everything leading up to the first ':' or ';' becomes the propName, and everything after
 * becomes the propDescr.
 * Returns NULL if the line contains no ':' or ';' characters.
 * Returns a pointer to the newly allocated Property otherwise.
 */
Property *initializeProperty(const char *line) {
    char name[200], descr[2000], *parse, *token;
    const char delim[] = ";:";
    Property *toReturn;

    if (line == NULL) {
        return NULL;
    }

    if (strlen(line) == 0) {
        return NULL;
    }

    printf("\tDEBUG: in initializeProperty: line = \"%s\"\n", line);

    // if these values are the same, then 'line' does not contain
    // any of the delimiting characters that are indicative of a property
    if (strcspn(line, delim) == strlen(line)) {
        printf("\tDEBUG: in initializeProperty: delim characters \"%s\" were not present in the line: \"%s\"\n", \
               delim, line);
        return NULL;
    }

    // make a copy that can be safely modified
    parse = malloc(strlen(line) + 1);
    strcpy(parse, line);
    printf("\tDEBUG: in initializeProperty: parse = \"%s\"\n", parse);

    // property name is mandatory, and cannot be empty
    token = strtok(parse, delim);
    if (token != NULL) {
        strcpy(name, token);
    } else {
        free(parse);
        return NULL;
    }

    // sometimes a property description can be empty, so we have to account for that
    token = strtok(NULL, delim);
    if (token != NULL) {
        strcpy(descr, token);
    } else {
        strcpy(descr, "");
    }

    printf("DEBUG: in initializeProperty: name=\"%s\", descr=\"%s\"\n", name, descr);

    toReturn = malloc(sizeof(Property) + strlen(descr) + 1);
    strcpy(toReturn->propName, name);
    strcpy(toReturn->propDescr, descr);

    free(parse);

    return toReturn;
}

/*
 * Allocates memory for an Alarm structure, and initializes its Property List.
 * Alarms have multiple properties across multiple lines, so their data
 * must be entered manually.
 * Memory is not allocated for its 'trigger', as it is dynamically allocated
 * to perfectly fit the length of its string.
 * Returns a pointer to the newly allocated Alarm structure.
 */
Alarm *initializeAlarm() {
    Alarm *toReturn = malloc(sizeof(Alarm));

    strcpy(toReturn->action, "");
    toReturn->trigger = NULL;
    toReturn->properties = initializeList(printProperty, deleteProperty, compareProperties);

    return toReturn;
}

/*
 * Allocates memory for an Event structure, and initializes its Property List.
 * Events have mutliple properties across multiple lines, so their data
 * must be entered manually.
 * Returns a pointer to the newly allocated Event structure.
 */
Event *initializeEvent() {
    Event *toReturn = malloc(sizeof(Event));

    toReturn->properties = initializeList(printProperty, deleteProperty, compareProperties);
    toReturn->alarms = initializeList(printAlarm, deleteAlarm, compareAlarms);

    return toReturn;
}

/*
 * Allocates memory for a Calendar structure, and initializes all of its Lists.
 * Calendar's have multiple properties across multiple lines, so their data
 * must be entered manually.
 * Returns a pointer to the newly allocated Calendar structure.
 */
Calendar *initializeCalendar() {
    Calendar *toReturn = malloc(sizeof(Calendar));

    toReturn->version = 0.0;
    strcpy(toReturn->prodID, "");
    toReturn->events = initializeList(printEvent, deleteEvent, compareEvents);
    toReturn->properties = initializeList(printProperty, deleteProperty, compareProperties);

    return toReturn;
}

