// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//CMPE13 Support Library
#include "BOARD.h"
#include "stack.h"

// Microchip libraries
#include <xc.h>

// User libraries
#include "rpn.h"

int main()
{
    BOARD_Init();
    int error;

    printf("\n###### Beginning CRUZID's rpn test harness: ####\n\n");

    //What follows is starter code.  You will need to modify it!
    
    char test0[] = "1 1 +";
    int c = 0;
    double e = 0;
    double result0;
    double expected0 = 2;
    printf("Testing RPN_Evaluate with \"%s\"... \n", test0);
    error = RPN_Evaluate(test0, &result0);
    if (error) {
        printf("   Failed, RPN_Evaluate produced an error\n");
    } else if (result0 != expected0) {
        printf("   Failed, expected = %f , result = %f\n", expected0, result0);
    } else {
        printf("   Success!\n");
        c += 1;
    }
    char test1[] = "2 5 *";
    double result1;
    double expected1 = 10;
    printf("Testing RPN_Evaluate with \"%s\"... \n", test1);
    error = RPN_Evaluate(test1, &result1);
    if (error) {
        printf("   Failed, RPN_Evaluate produced an error\n");
    } else if (result1 != expected1) {
        printf("   Failed, expected = %f , result = %f\n", expected1, result1);
    } else {
        printf("   Success!\n");
        c += 1;
    }
    char test2[] = "1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21";
    double result2;
    int expected2 = RPN_ERROR_STACK_OVERFLOW;
    printf("Testing RPN_Evaluate with \"%s\"... \n", test2);
    error = RPN_Evaluate(test2, &result2);
    if (error == expected2) {
        printf("   Passed, RPN_Evaluate produced an error\n");
        c += 1;
    }
   
     else {
        printf("   Failed\n");
    }
    char test3[] = "1 1 + 1 2 -";
    double result3;
    double expected3 = 1;
    printf("Testing RPN_Evaluate with \"%s\"... \n", test3);
    error = RPN_Evaluate(test3, &result3);
    if (error) {
        printf("   Failed, RPN_Evaluate produced an error\n");
    } else if (result3 != expected3) {
        printf("   Failed, expected = %f , result = %f\n", expected3, result3);
    } else {
        printf("   Success!\n");
        c += 1;
    }
    char test4[] = "1 + +";
    double result4;
    int expected4 = RPN_ERROR_STACK_UNDERFLOW;
    printf("Testing RPN_Evaluate with \"%s\"... \n", test4);
    error = RPN_Evaluate(test4, &result4);
    if (error == expected4) {
        printf("   Success, RPN_Evaluate produced an error \n");
        c += 1;
    }
    else {
        printf("   Failed\n");
    }
    char test5[] = "2 0 /";
    double result5;
    int expected5 = RPN_ERROR_DIVIDE_BY_ZERO;
    printf("Testing RPN_Evaluate with \"%s\"... \n", test5);
    error = RPN_Evaluate(test5, &result5);
    if (error == expected5) {
        printf("   Success, RPN_Evaluate produced an error\n");
        c += 1;
    }
    else {
        printf("   Failed\n");
    }
    char test6[] = ".1 .1 +";
    double result6;
    double expected6 = .2;
    printf("Testing RPN_Evaluate with \"%s\"... \n", test6);
    error = RPN_Evaluate(test6, &result6);
    if (error) {
        printf("   Failed, RPN_Evaluate produced an error\n");
    } else if (result6 != expected6) {
        printf("   Failed, expected = %f , result = %f\n", expected6, result6);
    } else {
        printf("   Success!\n");
        c += 1;
    }
    char test7[] = "           ";
    double result7;
    int expected7 = RPN_ERROR_TOO_FEW_ITEMS_REMAIN;
    printf("Testing RPN_Evaluate with \"%s\"... \n", test7);
    error = RPN_Evaluate(test7, &result7);
    if (error == expected7) {
        printf("   Success, RPN_Evaluate produced an error\n");
        c += 1;
    }
    else {
        printf("   Failed\n");
    }
    char test8[] = "2 2 2 +";
    double result8;
    int expected8 = RPN_ERROR_TOO_MANY_ITEMS_REMAIN;
    printf("Testing RPN_Evaluate with \"%s\"... \n", test8);
    error = RPN_Evaluate(test8, &result8);
    if (error == expected8) {
        printf("   Success, RPN_Evaluate produced an error\n");
        c += 1;
    }
    else {
        printf("   Failed\n");
    }
    printf("-----------------------------------------\n");
    printf("%d of 8 cases\n", c);
    e = c / 8.0;
    e *= 100;
    printf("%.2f%% complete", e); 
    /*printf("Testing ProcessBackspaces:\n");
    char test_pb1[] = "123\b34";
    ProcessBackspaces(test_pb1);
    printf("result    : %s\n", test_pb1);
    printf("should be : 1234\n");
     * 
     * */

    while (1);
}


