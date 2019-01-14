/************************************
 *  Name: Joseph Coffa              *
 *  Student #: 1007320              *
 *  Due Date: February 4, 2019      *
 *                                  *
 *  Assignment 1, CIS*2750          *
 *  main.c                          *
 ************************************/

// FIXME NO PATHS IN INCLUDE STATEMENTS
//       Use shared libraries after you figure out how to do that
#include "../../CalendarParser.h"
#include "../../LinkedListAPI.h"
#include "../include/Comparison.h"
#include "../include/Parsing.h"

int main() {
    // TODO test functions
    char s1[] = "   This has spaces.   ";
    char s2[] = "\tThis has a tab at the front, and also\there and after this\t";
    char s3[] = "           This has a mix of\tboth\t!    ";

    FILE *desc1 = fopen("t1.ics", "r");
    FILE *desc2 = fopen("t2.ics", "r");
    FILE *desc3 = fopen("t3.ics", "r");
    FILE *desc4 = fopen("t4.ics", "r");
    FILE *desc5 = fopen("../../Empty_Calendar/iCalendar Parser Test_3rdpsfoaumv3ddhncb2oc55fm4@group.calendar.google.com.ics", "r");

    printf("----------WHITESPACE TRIMMING----------\n");
    printf("\"%s\" -> \"%s\"\n", s1, trimWhitespace(s1));
    printf("\"%s\" -> \"%s\"\n", s2, trimWhitespace(s2));
    printf("\"%s\" -> \"%s\"\n", s3, trimWhitespace(s3));

    /*
    printf("\n----------UNFOLDING----------\n");
    // TODO read in lines from the file pointers, and use the unfold function to unfold
    //      the multiline descriptions into 1 string.
    char buffer1[80];
    char *toPrint;

    // TODO fix concat() function
    // TODO concatenate buffer into one string, pass into unfold()
    while (fgets(buffer1, 64, desc1)) {
        printf("%s", buffer1);
        toPrint = concat(toPrint, buffer1);
    }
    printf("CONCATENATED: %s\n\n", toPrint);

    while (fgets(buffer1, 64, desc2)) {
        printf("%s", buffer1);
        toPrint = concat(buffer2, buffer1);
    }
    printf("CONCATENATED: %s\n\n", toPrint);

    while (fgets(buffer1, 64, desc3)) {
        printf("%s", buffer1);
        toPrint = concat(buffer2, buffer1);
    }
    printf("CONCATENATED: %s\n\n", toPrint);

    while (fgets(buffer1, 64, desc4)) {
        printf("%s", buffer1);
        toPrint = concat(buffer2, buffer1);
    }
    printf("CONCATENATED: %s\n\n", toPrint);

    while (fgets(buffer1, 64, desc5)) {
        printf("%s", buffer1);
        toPrint = concat(buffer2, buffer1);
    }
    printf("CONCATENATED: %s\n\n", toPrint);

    free(toPrint);
    */

    fclose(desc1);
    fclose(desc2);
    fclose(desc3);
    fclose(desc4);
    fclose(desc5);

    printf("\n----------CREATE CALENDAR----------\n");
    Calendar *newCalendar;
    createCalendar("newCalendar.ics", &newCalendar);

    char *humanReadable = printCalendar(newCalendar);
    printf("\nCalendar created from file 'newCalendar.ics':\"%s\"\n", humanReadable);
    free(humanReadable);

    deleteCalendar(newCalendar);
    
    printf("\n----------STARTS WITH----------\n");
    printf("\"DESCRIPTION:Hi!\" and \"DESCRIPTION:\" = %d\n", startsWith("DESCRIPTION:Hi!", "DESCRIPTION:"));
    printf("\"PRODID:-//Google,...\" and \"PRODID:\" = %d\n", startsWith("PRODID:-//Google,...", "PRODID:"));
    printf("\"Nope\" and \"VERSION:\" = %d\n", startsWith("Nope", "VERSION:"));
    printf("\"Not this one either\" and \"VERSION:\" = %d\n", startsWith("Not this one either", "VERSION:"));
    printf("\"VERSION=2.0\" and \"VERSION:\" = %d\n", startsWith("VERSION=2.0", "VERSION:"));

    return EXIT_SUCCESS;
}
