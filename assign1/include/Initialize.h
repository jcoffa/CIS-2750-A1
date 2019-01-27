/************************************
 *  Name: Joseph Coffa              *
 *  Student #: 1007320              *
 *  Due Date: February 4, 2019      *
 *                                  *
 *  Assignment 1, CIS*2750          *
 *  Initialize.h                    *
 ************************************/

#ifndef INITIALIZE_H
#define INITIALIZE_H

#include <stdlib.h>
#include <string.h>

#include "CalendarParser.h"
#include "LinkedListAPI.h"

/*
 */
void initializeDateTime(const char *line, DateTime *dt);

/*
 * Allocates memory for a Property structure and populates it with data retrieved from the
 * string 'line', which should come from an iCalendar file.
 * Everything leading up to the first ':' or ';' becomes the propName, and everything after
 * becomes the propDescr.
 * Returns NULL if the line contains no ':' or ';' characters.
 * Returns a pointer to the newly allocated Property otherwise.
 */
Property *initializeProperty(const char *line);

/*
 * Allocates memory for an Alarm structure, and initializes its Property List.
 * Alarms have multiple properties across multiple lines, so their data
 * must be entered manually.
 * Memory is not allocated for its 'trigger', as it is dynamically allocated
 * to perfectly fir the length of its string.
 * Returns a pointer to the newly allocated Alarm structure.
 */
Alarm *initializeAlarm();

/*
 * Allocates memory for an Event structure, and initializes its Property List.
 * Events have mutliple properties across multiple lines, so their data
 * must be entered manually.
 * Returns a pointer to the newly allocated Event structure.
 */
Event *initializeEvent();

/*
 * Allocates memory for a Calendar structure, and initializes all of its Lists.
 * Calendar's have multiple properties across multiple lines, so their data
 * must be entered manually.
 * Returns a pointer to the newly allocated Calendar structure.
 */
Calendar *initializeCalendar();

#endif
