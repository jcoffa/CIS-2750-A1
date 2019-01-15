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

    printf("----------WHITESPACE TRIMMING----------\n");
    printf("\"%s\" -> \"%s\"\n", s1, trimWhitespace(s1));
    printf("\"%s\" -> \"%s\"\n", s2, trimWhitespace(s2));
    printf("\"%s\" -> \"%s\"\n", s3, trimWhitespace(s3));





    printf("\n----------CONCATENATION----------\n");
    char concatenated[200] = "";
    concat(concatenated, "This is the start of a concatenated string."); //  43
    printf("\"%s\"\n", concatenated);

    concat(concatenated, "But what if I add more to it?");  // 29
    printf("\"%s\"\n", concatenated);

    concat(concatenated, " You fool! You must add in the space yourself!"); // 46
    printf("\"%s\"\n", concatenated);





    printf("\n----------UNFOLDING----------\n");
    char fold1[] = "This line ha\r\n s some folds in i\r\n t!";
    char fold2[] = "This line doesn't have any whatsoever.";
    char fold3[] = "This one has\r\n  a fold immediately preceding a space! Tricky...";
    char fold4[] = "Thi\r\n s one has lots\r\n  of folds \r\n in it!";

    printf("FOLDED: \"%s\"\n", fold1);
    unfold(fold1);
    printf("UNFOLDED: \"%s\"\n", fold1);
    printf("~~~~~~~~~~\n");
   
    printf("FOLDED: \"%s\"\n", fold2);
    unfold(fold2);
    printf("UNFOLDED: \"%s\"\n", fold2);
    printf("~~~~~~~~~~\n");

    printf("FOLDED: \"%s\"\n", fold3);
    unfold(fold3);
    printf("UNFOLDED: \"%s\"\n", fold3);
    printf("~~~~~~~~~~\n");

    printf("FOLDED: \"%s\"\n", fold4);
    unfold(fold4);
    printf("UNFOLDED: \"%s\"\n", fold4);





    printf("\n----------UNFOLDING AN ICAL PROPERTY ACROSS MULTIPLE LINES----------\n");
    char buffer[80];
    char toPrint[200];

    printf("~~~~~~~~~~ NEW FILE ~~~~~~~~~\n");
    while (fgets(buffer, 64, desc1)) {
        printf("%s", buffer);
        concat(toPrint, buffer);
    }
    printf("CONCATENATED: %s\n", toPrint);
    unfold(toPrint);
    printf("UNFOLDED: \"%s\"\n", toPrint);
    strcpy(toPrint, "");


    printf("\n~~~~~~~~~~ NEW FILE ~~~~~~~~~\n");
    while (fgets(buffer, 64, desc2)) {
        printf("%s", buffer);
        concat(toPrint, buffer);
    }
    printf("CONCATENATED: %s\n", toPrint);
    unfold(toPrint);
    printf("UNFOLDED: \"%s\"\n", toPrint);
    strcpy(toPrint, "");


    printf("\n~~~~~~~~~~ NEW FILE ~~~~~~~~~\n");
    while (fgets(buffer, 64, desc3)) {
        printf("%s", buffer);
        concat(toPrint, buffer);
    }
    printf("CONCATENATED: %s\n", toPrint);
    unfold(toPrint);
    printf("UNFOLDED: \"%s\"\n", toPrint);
    strcpy(toPrint, "");


    printf("\n~~~~~~~~~~ NEW FILE ~~~~~~~~~\n");
    while (fgets(buffer, 64, desc4)) {
        printf("%s", buffer);
        concat(toPrint, buffer);
    }
    printf("CONCATENATED: %s\n", toPrint);
    unfold(toPrint);
    printf("UNFOLDED: \"%s\"\n", toPrint);
    strcpy(toPrint, "");


    fclose(desc1);
    fclose(desc2);
    fclose(desc3);
    fclose(desc4);





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
    printf("\"VERSION=2.0\" and \"\" = %d\n", startsWith("VERSION=2.0", ""));





    printf("\n----------ENDS WITH----------\n");
    printf("\"This is a test\" and \"t\" = %d\n", endsWith("This is a test", "t"));
    printf("\"This is a test\" and \"test\" = %d\n", endsWith("This is a test", "test"));
    printf("\"This is a test\" and \"q\" = %d\n", endsWith("This is a test", "q"));
    printf("\"This is a test\" and \"But this one is much much longer\" = %d\n", endsWith("This is a test", "But this one is much much longer"));
    printf("\"This is a test\" and \"\" = %d\n", endsWith("This is a test", ""));

    return EXIT_SUCCESS;
}
