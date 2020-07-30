/*
 * File:   rpn.c
 * Author: vince
 *
 * Created on February 3, 2020, 2:59 PM
 */
#include "BOARD.h"
#include <xc.h>
#include "rpn.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "stack.h"

/* RPN_Evaluate() parses and evaluates a string that contains 
 * a valid Reverse Polish Notation string (no newlines!)  
 * @param:  rpn_string - a string in polish notation.  Tokens must be either 
 *          arithmetic operators or numbers.
 * @param:  result - a pointer to a double that will be modified to contain
 *          the return value of the rpn expression.
 * @return: error - if the rpn expression is invalid, 
 *          an appropriate error value is returned.
 * 
 * RPN_Evaluate supports the following basic arithmetic operations:
 *   + : addition
 *   - : subtraction
 *   * : multiplication
 *   / : division
 * Numeric tokens can be positive or negative, and can be integers or 
 * decimal floats.  RPN_Evaluate should be able to handle strings of 
 * at least 255 length.
 * */
int RPN_Evaluate(char * rpn_string, double * result){
    struct Stack stack = {};

    double x, y;
    char * split_rpn;
    split_rpn = strtok(rpn_string, " ");
    double num1;
    StackInit(&stack);
    while (split_rpn != NULL) {
        if (strcmp(split_rpn, "+") == 0) {
            if (StackIsEmpty(&stack) == TRUE) {
                return RPN_ERROR_STACK_UNDERFLOW;
            }
            StackPop(&stack, &x);
            if (StackIsEmpty(&stack) == TRUE) {
                return RPN_ERROR_STACK_UNDERFLOW;
            }
            StackPop(&stack, &y);
            *result = x + y;
            if (StackIsFull(&stack) == TRUE) {
                return RPN_ERROR_STACK_OVERFLOW;
            }
            StackPush(&stack, *result);
        }

        if (strcmp(split_rpn, "-") == 0) {

            if (StackIsEmpty(&stack) == TRUE) {
                return RPN_ERROR_STACK_UNDERFLOW;
            }
            StackPop(&stack, &x);
            if (StackIsEmpty(&stack) == TRUE) {
                return RPN_ERROR_STACK_UNDERFLOW;
            }
            StackPop(&stack, &y);
            *result = x - y;
            if (StackIsFull(&stack) == TRUE) {
                return RPN_ERROR_STACK_OVERFLOW;
            }
            StackPush(&stack, *result);
        }

        if (strcmp(split_rpn, "*") == 0) {
            if (StackIsEmpty(&stack) == TRUE) {
                return RPN_ERROR_STACK_UNDERFLOW;
            }
            StackPop(&stack, &x);
            if (StackIsEmpty(&stack) == TRUE) {
                return RPN_ERROR_STACK_UNDERFLOW;
            }
            StackPop(&stack, &y);
            *result = x * y;
            if (StackIsFull(&stack) == TRUE) {
                return RPN_ERROR_STACK_OVERFLOW;
            }
            StackPush(&stack, *result);
        }

        if (strcmp(split_rpn, "/") == 0) {
            if (StackIsEmpty(&stack) == TRUE) {
                return RPN_ERROR_STACK_UNDERFLOW;
            }
            StackPop(&stack, &x);
            if (StackIsEmpty(&stack) == TRUE) {
                return RPN_ERROR_STACK_UNDERFLOW;
            }
            StackPop(&stack, &y);
            if (y == 0) {
                return RPN_ERROR_DIVIDE_BY_ZERO;
            }
            *result = x / y;
            if (StackIsFull(&stack) == TRUE) {
                return RPN_ERROR_STACK_OVERFLOW;
            }
            StackPush(&stack, *result);
        }
        num1 = atof(split_rpn);
        if (StackIsFull(&stack) == TRUE) {
            return RPN_ERROR_STACK_OVERFLOW;
        }
        StackPush(&stack, num1);

        split_rpn = strtok(NULL, " ");
    }
    return RPN_NO_ERROR;
}

int ProcessBackspaces(char *rpn_sentence)
{
    return NULL;
}