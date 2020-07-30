/*
 * File:   LinkedListTest.c
 * Author: vince
 *
 * Created on February 8, 2020, 1:22 PM
 */


#include "xc.h"
// Standard libraries
#include <stdio.h>
#include <string.h>

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>

// User libraries
#include "LinkedList.h"
//function
ListItem *CreateUnsortedList(void);

int main(void)
{
    BOARD_Init();
    int c = 0;
    printf("\n\nWelcome to vlguan's Linked List Test Harness, compiled on %s %s.\n", __DATE__, __TIME__);

    printf("This will demonstrate that LinkedListNew() and LinkedListCreateAfter() return the list:\n");
    ListItem *listToSort = CreateUnsortedList();
    listToSort = LinkedListCreateAfter(listToSort, "me");
    ListItem *temp = LinkedListGetFirst(listToSort);
    if (strcmp(temp->data, "D") == 0) {
        printf("Passed\n");
        
    } else {
        printf("Failed\n");
    }
    if (strcmp(temp->nextItem->data, "me") == 0) {
        printf("Passed\n");
        
    } else {
        printf("Failed\n");
    }
    printf("This will demonstrate that LinkedListRemove() removes an element of the linked list:\n");
    LinkedListRemove(temp);
    if (strcmp(temp->data, "D") == 0) {
        printf("Passed\n");
        
    } else {
        printf("Failed\n");
    }
    LinkedListRemove(temp);
    if (strcmp(temp->nextItem->data, "E") == 0) {
        printf("Passed\n");
        
    } else {
        printf("Failed\n");
    }
    
    printf("This will demonstrate that LinkedListGetFirst() finds first item:\n");
    temp = LinkedListGetFirst(temp);
    if (strcmp(temp->data, "D") == 0) {
        printf("Passed\n");
        
    } else {
        printf("Failed\n");
    }
    temp = LinkedListGetFirst(temp);
    printf("This will demonstrate that LinkedListGetLast() finds last item:\n");
    temp = LinkedListGetLast(temp);
    if (strcmp(temp->data, "A") == 0) {
        printf("Passed\n");
        
    } else {
        printf("Failed\n");
    }

    temp = LinkedListCreateAfter(temp, "me");
    temp = LinkedListGetLast(temp);
    if (strcmp(temp->data, "me") == 0) {
        printf("Passed\n");
        
    } else {
        printf("Failed\n");
    }
    printf("This will demonstrate that LinkedListSize() measures size of the linked list:\n");
    listToSort = CreateUnsortedList();
    int x = LinkedListSize(listToSort);
    if (x == 5) {
        printf("Passed\n");
        
    } else {
        printf("Failed\n");
    }
    temp = LinkedListCreateAfter(listToSort, "me");
    x = LinkedListSize(listToSort);
    if (x == 6) {
        printf("Passed\n");
        
    } else {
        printf("Failed\n");
    }
    printf("This will demonstrate that LinkedSwapData works:\n");
    printf("\nBefore Swap:\n");
    LinkedListPrint(temp);
    printf("\nAfter Swap:\n");
    LinkedListSwapData(temp, temp->nextItem->nextItem);
    LinkedListPrint(temp);
    printf("\nBefore Swap:\n");
    LinkedListPrint(temp);
    printf("\nAfter Swap:\n");
    LinkedListSwapData(temp, temp->nextItem->nextItem->nextItem);
    LinkedListPrint(temp);
    printf("\nThis will demonstrate that LinkedListPrint() prints:\n");
    LinkedListPrint(temp);
    while (1);
}

ListItem *CreateUnsortedList(void)
/// <editor-fold defaultstate="collapsed" desc="CreateUnsortedList definition">
{
    //     char* wordList[] = {
    //            "decide",   "toothpaste",   "lowly",        "robin",        "reign",        "crowd",
    //            "pies",     "reduce",       "tendency",     "surround",     "finger",       "rake", 
    //            "alleged",  "hug",          "nest",         "punishment",   "eggnog",       "side", 
    //            "beef",     "exuberant",    "sort",         "scream",       "zip",          "hair", 
    //            "ragged",   "damage",       "thought",      "jump",         "frequent",     "substance",
    //            "head",     "step",         "faithful",     "sidewalk",     "pig",          "raspy",
    //            "juggle",   "shut",         "maddening",    "rock",         "telephone",    "selective",
    //            NULL};
    char* wordList[] = {"D", "A", "C", "E", "B", NULL};


    int i = 0;
    ListItem* head = LinkedListNew(wordList[i]);
    ListItem* tail = head;
    for (i = 1; wordList[i] != NULL; i++) {
        tail = LinkedListCreateAfter(tail, wordList[i]);
        if (tail == NULL) {
            printf("ERROR:  Heap full! Please increase heap size.\n");
            FATAL_ERROR();
        }
    }
    return head;
}
// </editor-fold>
