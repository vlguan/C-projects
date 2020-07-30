// **** Include libraries here ****
// Standard libraries

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <stdio.h>

// User libraries
#include "stack.h"

int main()
{
    BOARD_Init();

    printf("\n###### Beginning vlguan's stack test harness: ####\n\n");

    //What follows is starter code.  You will need to modify it!
    
    printf("Does StackInit() set the currentItemIndex appropriately?\n");
    struct Stack stack = {};
    StackInit(&stack);
    if (stack.currentItemIndex == -1) {
        printf("passed\n");
    } else {
        printf("failed\n");
    }
    printf("Does StackPush() set the currentItemIndex appropriately?\n");
    
    StackPush(&stack, 5);
    if (StackGetSize(&stack) == 1){
        printf("passed\n");
    } else {
        printf("failed\n");
    }
    printf("Does StackPop() return the correct error?\n");
    StackInit(&stack);
    double x;
    if (StackPop(&stack, &x) == STANDARD_ERROR){
        printf("passed\n");
    }
    else{
        printf("failed\n");
    }
    printf("Does StackPop() return the correct value?\n");
    StackInit(&stack);
    StackPush(&stack, 5);
    StackPop(&stack, &x);
    if (x == 5){
        printf("passed\n");
    } else{
        printf("failed\n");
    }
    printf("Does StackIsEmpty() produce the correct boolean?\n");
    StackInit(&stack);
    if (StackIsEmpty(&stack) == TRUE){
        printf("passed\n");
    } else{
        printf("failed\n");
    }
    printf("Does StackIsFull() produce the correct boolean?\n");
    StackInit(&stack);
    int i;
    for (i = 0; i < 20; i++){
        StackPush(&stack, i);
    }
    if (StackIsFull(&stack) == TRUE){
        printf("passed\n");
    } else{
        printf("failed\n");
    }
    printf("Does StackGetSize return the correct size?\n");
    StackInit(&stack);
    StackPush(&stack, 1);
    if (StackGetSize(&stack) == 1){
        printf("passed\n");
    }else{
        printf("failed\n");
    }
    
    //test stackInit:
    while (1);
    return 0;
}


