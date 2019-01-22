/************************************
 *  Name: Joseph Coffa              *
 *  Student #: 1007320              *
 *  Due Date: February 4, 2019      *
 *                                  *
 *  Assignment 1, CIS*2750          *
 *  main.c                          *
 ************************************/

#include "CalendarParser.h"
#include "LinkedListAPI.h"
#include "Parsing.h"
#include "Initialize.h"

int main() {

    /*
    printf("----------WHITESPACE TRIMMING----------\n");
    char s1[] = "   This has spaces.   ";
    char s2[] = "\tThis has a tab at the front, and also\there and after this\t";
    char s3[] = "           This has a mix of\tboth\t!    ";

    printf("\"%s\" -> \"%s\"\n", s1, trimWhitespace(s1));
    printf("\"%s\" -> \"%s\"\n", s2, trimWhitespace(s2));
    printf("\"%s\" -> \"%s\"\n", s3, trimWhitespace(s3));
    */





    /*
    printf("\n----------CONCATENATION----------\n");
    char concatenated[200] = "";
    concat(concatenated, "This is the start of a concatenated string."); //  43
    printf("\"%s\"\n", concatenated);

    concat(concatenated, "But what if I add more to it?");  // 29
    printf("\"%s\"\n", concatenated);

    concat(concatenated, " You fool! You must add in the space yourself!"); // 46
    printf("\"%s\"\n", concatenated);
    */





    /*
    printf("\n----------UNFOLDING----------\n");
    char fold1[] = "This line ha\r\n s some folds in i\r\n t!\r\n";
    char fold2[] = "This line doesn't have any whatsoever.";
    char fold3[] = "This one has\r\n  a fold immediately preceding a space! Tricky...\r\n";
    char fold4[] = "Thi\r\n s one has lots\r\n  of folds \r\n in it!\r\n";

    printf("FOLDED:\n\"%s\"\n", fold1);
    unfold(fold1);
    printf("UNFOLDED: \"%s\"\n", fold1);
    printf("~~~~~~~~~~\n");
   
    printf("FOLDED:\n\"%s\"\n", fold2);
    unfold(fold2);
    printf("UNFOLDED: \"%s\"\n", fold2);
    printf("~~~~~~~~~~\n");

    printf("FOLDED:\n\"%s\"\n", fold3);
    unfold(fold3);
    printf("UNFOLDED: \"%s\"\n", fold3);
    printf("~~~~~~~~~~\n");

    printf("FOLDED:\n\"%s\"\n", fold4);
    unfold(fold4);
    printf("UNFOLDED: \"%s\"\n", fold4);
    */





    /*
    printf("\n----------UNFOLDING AN ICAL PROPERTY ACROSS MULTIPLE LINES----------\n");
    FILE *desc1 = fopen("/home/joseph/cis2750/a01/testFiles/t1.ics", "r");
    FILE *desc2 = fopen("/home/joseph/cis2750/a01/testFiles/t2.ics", "r");
    FILE *desc3 = fopen("/home/joseph/cis2750/a01/testFiles/t3.ics", "r");
    FILE *desc4 = fopen("/home/joseph/cis2750/a01/testFiles/t4.ics", "r");
    char buffer[80];
    char toPrint[200] = "";

    printf("~~~~~~~~~~ NEW FILE ~~~~~~~~~\n");
    if (desc1) {
        while (fgets(buffer, 79, desc1)) {
            printf("%s", buffer);
            concat(toPrint, buffer);
        }
        printf("CONCATENATED: %s\n", toPrint);
        unfold(toPrint);
        printf("UNFOLDED: \"%s\"\n", toPrint);
        strcpy(toPrint, "");
        fclose(desc1);
    } else {
        printf("Opening desc1 failed\n");
    }


    printf("\n~~~~~~~~~~ NEW FILE ~~~~~~~~~\n");
    if (desc2) {
        while (fgets(buffer, 79, desc2)) {
            printf("%s", buffer);
            concat(toPrint, buffer);
        }
        printf("CONCATENATED: %s\n", toPrint);
        unfold(toPrint);
        printf("UNFOLDED: \"%s\"\n", toPrint);
        strcpy(toPrint, "");
        fclose(desc2);
    } else {
        printf("Opening desc2 failed\n");
    }


    printf("\n~~~~~~~~~~ NEW FILE ~~~~~~~~~\n");
    if (desc3) {
        while (fgets(buffer, 79, desc3)) {
            printf("%s", buffer);
            concat(toPrint, buffer);
        }
        printf("CONCATENATED: %s\n", toPrint);
        unfold(toPrint);
        printf("UNFOLDED: \"%s\"\n", toPrint);
        strcpy(toPrint, "");
        fclose(desc3);
    } else {
        printf("Opening desc3 failed\n");
    }
    */


    /*
    printf("\n~~~~~~~~~~ NEW FILE ~~~~~~~~~\n");
    if (desc4) {
        while (fgets(buffer, 79, desc4)) {
            printf("%s", buffer);
            concat(toPrint, buffer);
        }
        printf("CONCATENATED: %s\n", toPrint);
        unfold(toPrint);
        printf("UNFOLDED: \"%s\"\n", toPrint);
        strcpy(toPrint, "");
        fclose(desc4);
    } else {
        printf("Opening desc4 failed\n");
    }
    */
    



    /*
    printf("\n----------CREATE CALENDAR----------\n");
    Calendar *newCalendar;
    ICalErrorCode error = createCalendar("/home/joseph/cis2750/a01/testFiles/newCalendar.ics", &newCalendar);
    if (error == OK) {
        char *humanReadable = printCalendar(newCalendar);
        printf("\nCalendar created from file 'newCalendar.ics':\"%s\"\n", humanReadable);
        free(humanReadable);

        deleteCalendar(newCalendar);
    } else {
        char *errorStr = printError(error);
        printf("Something went wrong when reading \"newCalendar.ics\": %s\n", errorStr);
        free(errorStr);
    }
    */
    




    /*
    printf("\n----------STARTS WITH----------\n");
    printf("\"DESCRIPTION:Hi!\" and \"DESCRIPTION:\" = %d\n", startsWith("DESCRIPTION:Hi!", "DESCRIPTION:"));
    printf("\"PRODID:-//Google,...\" and \"PRODID:\" = %d\n", startsWith("PRODID:-//Google,...", "PRODID:"));
    printf("\"Nope\" and \"VERSION:\" = %d\n", startsWith("Nope", "VERSION:"));
    printf("\"Not this one either\" and \"VERSION:\" = %d\n", startsWith("Not this one either", "VERSION:"));
    printf("\"VERSION=2.0\" and \"VERSION:\" = %d\n", startsWith("VERSION=2.0", "VERSION:"));
    printf("\"VERSION=2.0\" and \"\" = %d\n", startsWith("VERSION=2.0", ""));
    */





    /*
    printf("\n----------ENDS WITH----------\n");
    printf("\"This is a test\" and \"t\" = %d\n", endsWith("This is a test", "t"));
    printf("\"This is a test\" and \"test\" = %d\n", endsWith("This is a test", "test"));
    printf("\"This is a test\" and \"q\" = %d\n", endsWith("This is a test", "q"));
    printf("\"This is a test\" and \"But this one is much much longer\" = %d\n", endsWith("This is a test", "But this one is much much longer"));
    printf("\"This is a test\" and \"\" = %d\n", endsWith("This is a test", ""));
    */





    /*
    printf("\n----------TOUPPER FUNCTIONS----------\n");
    char randCase[] = "ThiS CaSE is RandoM. HOPEFUllY thE FuncTioN WorKS ThIS TImE!";
    printf("Before: \"%s\"\n", randCase);
    strUpper(randCase);
    printf("After: \"%s\"\n", randCase);

    char randCase2[] = "But WhaT If we MalloC() the StriNg insTEAD?";
    char *returnedRandCase = strUpperCopy(randCase2);
    printf("\nrandCase2: \"%s\"\nreturnedRandCase: \"%s\"", randCase2, returnedRandCase);
    free(returnedRandCase);
    */





    /*
    printf("\n\n\n----------FILE PARSE UNFOLDING----------\n");
    FILE *testUnfold = fopen("/home/joseph/cis2750/a01/testFiles/unfolding.ics", "r");
    char unfolded[2000];
    while (!feof(testUnfold)) {
        readFold(unfolded, 2000, testUnfold);
        printf("Unfolded line: \"%s\"\n", unfolded);
    }

    printf("\n");

    FILE *multLineProp = fopen("/home/joseph/cis2750/a01/testFiles/mLineProp1.ics", "r");
    while (!feof(multLineProp)) {
        readFold(unfolded, 2000 ,multLineProp);
        printf("Unfolded line: \"%s\"\n", unfolded);
    }

    printf("\n");

    FILE *testUnfold2 = fopen("/home/joseph/cis2750/a01/testFiles/newCalendar.ics", "r");
    while (!feof(testUnfold2)) {
        readFold(unfolded, 2000, testUnfold2);
        printf("Unfolded line: \"%s\"\n", unfolded);
    }

    fclose(testUnfold);
    fclose(multLineProp);
    fclose(testUnfold2);
    */





    /*
    printf("\n\n\n----------PROPERTY CREATION----------\n");
    char testProperty1[] = "DESCRIPTION:This is a test\r\n";
    Property *testProp = newProperty(testProperty1);
    char *printProp1 = printProperty(testProp);
    printf("Property: \"%s\"\n", printProp1);

    char testProperty2[] = "METHOD:Testmethod";
    Property *testProp2 = newProperty(testProperty2);
    char *printProp2 = printProperty(testProp2);
    printf("Property: \"%s\"\n", printProp2);

    free(printProp1);
    free(printProp2);

    free(testProp);
    free(testProp2);
    */





    printf("\n----------CALENDAR CREATION----------\n");
    // TODO test createCalendar, getEvent, and getAlarm



    return EXIT_SUCCESS;
}

