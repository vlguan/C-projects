
// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <string.h>

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>

// User libraries
#include "LinkedList.h"


// **** Declare any function prototypes here ***
ListItem *CreateUnsortedList(void); //this function has been written for you
ListItem *SelectionSort(ListItem* list);
ListItem *InsertionSort(ListItem* list);

/* Students should not need to modify main! */
int main(void)
{
    BOARD_Init();

    printf("\n\nWelcome to vlguan's sort.c, compiled on %s %s.\n", __DATE__, __TIME__);
    
    //Do a timing test 
    printf("\nStarting stopwatch for selection sort...\n");
    ListItem *listToSort = CreateUnsortedList();
    __asm("nop"); //put a breakpoint on this line
    listToSort = SelectionSort(listToSort);
    __asm("nop"); //put a breakpoint on this line, and record the stopwatch time
    LinkedListPrint(listToSort);


    printf("\nStarting stopwatch for insertion sort...\n");
    listToSort = CreateUnsortedList();
    __asm("nop"); //put a breakpoint on this line
    listToSort = InsertionSort(listToSort);
    __asm("nop"); //put a breakpoint on this line, and record the stopwatch time
    LinkedListPrint(listToSort);

    while (1);
    return 0;
}

/**
 * SelectionSort() performs a selection sort on a passed to it.  The list is sorted alphabetically,
 * as determined by string.h's strcmp().  
 *
 * @param list Any element in a linked list to sort.  
 * @return a pointer to the head of the sorted list.
 * 
 * SelectionSort() assumes that no list items have a NULL data member.  It also assumes that 'list'
 * is a valid, non-NULL pointer to a linked list.
 * 
 * This function does not print.
 */
ListItem *SelectionSort(ListItem* list)
{

    list = LinkedListGetFirst(list);
    ListItem *min = list;
    while (list) {
        ListItem *min = list;
        ListItem *temp = list->nextItem;
        while (temp) {

            if (strcmp(temp->data, min->data) < 0) {

                min = temp;
            }
            temp = temp->nextItem;
        }
        LinkedListSwapData(min, list);

        list = list->nextItem;

    }
    list = LinkedListGetFirst(min);
    return list;
}

/**
 * InsertionSort() performs an insertion sort on a passed to it.  The list is sorted alphabetically,
 * as determined by string.h's strcmp().  
 *
 * @param list Any element in a linked list to sort.  
 * @return a pointer to the head of the sorted list.
 * 
 * SelectionSort() assumes that no list items have a NULL data member.  It also assumes that 'list'
 * is a valid, non-NULL pointer to a linked list.
 * 
 * This function does not print.
 */
ListItem *InsertionSort(ListItem* list)
{
    ListItem *sort = NULL;
    ListItem *current = LinkedListGetFirst(list);
    while (current) {
        ListItem *temp;
        ListItem *next = current->nextItem;
        if (sort == NULL || strcmp(sort->data,current->data) >= 0){
            current->nextItem = sort;
            sort = current;
        }
        else{
            temp = sort;
            while (temp->nextItem && strcmp (temp->nextItem->data, current->data) < 0){
                temp = temp->nextItem;
            }
            current->nextItem = temp->nextItem;
            temp->nextItem = current;
        }
        current = next;
    }
    list = sort;
    return list;
}

/* CreateUnsortedList() uses the functions in the LinkedList library to
 * generate an unsorted list.  
 * 
 * Students should not modify this function!
 * If an error occurs in this function, there is almost certainly 
 * a bug in your implementation of LinkedList.h.
 *   
 * @return A pointer to a ListItem which is the head of a new unsorted list. 
 */
ListItem *CreateUnsortedList(void)
/// <editor-fold defaultstate="collapsed" desc="CreateUnsortedList definition">
{
     char* wordList[] = {
            "decide",   "toothpaste",   "lowly",        "robin",        "reign",        "crowd",
            "pies",     "reduce",       "tendency",     "surround",     "finger",       "rake", 
            "alleged",  "hug",          "nest",         "punishment",   "eggnog",       "side", 
            "beef",     "exuberant",    "sort",         "scream",       "zip",          "hair", 
            "ragged",   "damage",       "thought",      "jump",         "frequent",     "substance",
            "head",     "step",         "faithful",     "sidewalk",     "pig",          "raspy",
            "juggle",   "shut",         "maddening",    "rock",         "telephone",    "selective",
            NULL};
//    char* wordList[] = {"D", "A", "C", "E", "B", NULL};


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
