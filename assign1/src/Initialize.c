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
 */
void initializeDateTime(char *line, DateTime *dt) {
    char data[200];
    const char delim[] = ";:";

    if (line == NULL) {
        dt = NULL;
        return;
    }

    int len = strlen(line);

    // the line contains no characters in 'delim', or the capital letter T
    // which is necessary to differentiate the date and time parts of a DateTime
    if ((strcspn(line, delim) == len) || (strcspn(line, "T") == len)) {
        dt = NULL;
        return;
    }

    // ignore everything before (and including) the property name and ':' or ';'
    strcpy(data, line + strcspn(line, delim) + 1);

    // everything before the "T" character is the date
    strncpy(dt->date, data, 8);
    dt->date[9] = '\0';

    // the next 6 characters after the "T" character is the time
    strncpy(dt->time, data + strcspn(data, "T"), 6);
    dt->time[7] = '\0';

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
Property *initializeProperty(char *line) {
    char name[200], descr[1000];
    const char delim[] = ";:";
    Property *toReturn;

    // if these values are the same, then 'line' does not contain
    // any of the delimiting characters that are indicative of a property
    if (strcspn(line, delim) == strlen(line)) {
        printf("DEBUG: in newProperty: delim characters \"%s\" were not present in the line: \"%s\"\n", \
               delim, line);
        return NULL;
    }

    strcpy(name, strtok(line, delim));
    strcpy(descr, strtok(NULL, delim));

    printf("DEBUG: in newProperty: name=\"%s\", descr=\"%s\"\n", name, descr);

    toReturn = malloc(sizeof(Property) + strlen(descr) + 1);
    strcpy(toReturn->propName, name);
    strcpy(toReturn->propDescr, descr);

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
    toReturn->events = initializeList(printEvent, deleteEvent, compareEvents);
    toReturn->properties = initializeList(printProperty, deleteProperty, compareProperties);

    return toReturn;
}

