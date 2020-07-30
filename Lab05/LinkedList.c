/*
 * File:   LinkedList.c
 * Author: vince
 *
 * Created on February 8, 2020, 1:21 PM
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

/**
 * This is the struct that will hold an individual list item. This is a doubly-linked list and
 * so there is no need to have a separate list struct that holds all of the individual list items
 * as they're already chained together. 
 */

/**
 * This function starts a new linked list. Given an allocated pointer to data it will return a
 * pointer for a malloc()ed ListItem struct. If malloc() fails for any reason, then this function
 * returns NULL otherwise it should return a pointer to this new list item. data can be NULL.
 *
 * @param data The data to be stored in the first ListItem in this new list. Can be any valid 
 *             pointer value.
 * @return A pointer to the malloc()'d ListItem. May be NULL if an error occured.
 */
ListItem *LinkedListNew(char *data)
{
    ListItem *ptr = malloc(sizeof (ListItem));
    ptr->data = data;
    ptr->previousItem = NULL;
    ptr->nextItem = NULL;
    return ptr;
}

/**
 * This function allocates a new ListItem containing data and inserts it into the list directly
 * after item. It rearranges the pointers of other elements in the list to make this happen. If
 * passed a NULL item, CreateAfter() should still create a new ListItem, just with no previousItem.
 * It returns NULL if it can't malloc() a new ListItem, otherwise it returns a pointer to the new
 * item. The data parameter is also allowed to be NULL.
 *
 * @param item The ListItem that will be before the newly-created ListItem.
 * @param data The data the new ListItem will point to.
 * @return A pointer to the newly-malloc()'d ListItem.
 */
ListItem *LinkedListCreateAfter(ListItem *item, char *data)
{
    if (item == NULL) {
        ListItem *newptr;
        newptr = LinkedListNew(data);
        return newptr;
    }
    ListItem *temp1 = LinkedListNew(data);

    temp1->nextItem = item->nextItem;
    temp1->previousItem = item;
    item->nextItem = temp1;

    return item;
}

/**
 * This function will remove a list item from the linked list and free() the memory that the
 * ListItem struct was using. It doesn't, however, free() the data pointer and instead returns it
 * so that the calling code can manage it.  If passed a pointer to NULL, LinkedListRemove() should
 * return NULL to signal an error.
 *
 * @param item The ListItem to remove from the list.
 * @return The data pointer from the removed item. May be NULL.
 */
char *LinkedListRemove(ListItem *item)
{
    if (item == NULL) {
        return NULL;
    }
    char *temp1 = item->data;
    ListItem *temp2 = item->nextItem;\
    ListItem *temp3 = temp2;

    temp3->previousItem->nextItem = temp2->nextItem;

    free(temp2);

    return temp1;
}

/**
 * This function returns the total size of the linked list. This means that even if it is passed a
 * ListItem that is not at the head of the list, it should still return the total number of
 * ListItems in the list. A NULL argument will result in 0 being returned.
 *
 * @param list An item in the list to be sized.
 * @return The number of ListItems in the list (0 if `list` was NULL).
 */
int LinkedListSize(ListItem *list)
{

    int c = 0;
    if (list == NULL) {
        return c;
    } else {
        list = LinkedListGetFirst(list);
        c++;
        while (list->nextItem != NULL) {
            c++;
            list = list->nextItem;

        }
    }
    return c;
}

/**
 * This function returns the head of a list given some element in the list. If it is passed NULL,
 * it will just return NULL. If given the head of the list it will just return the pointer to the
 * head anyways for consistency.
 *
 * @param list An element in a list.
 * @return A pointer to the first element in the list. Or NULL if provided an invalid list.
 */
ListItem *LinkedListGetFirst(ListItem *list)
{
    if (list == NULL) {
        return NULL;
    } else {
        while (list->previousItem != NULL) {
            list = list->previousItem;
        }

    }
    return list;
}

/**
 * This function operates identically to LinkedListGetFirst(), but returns
 * the tail of a linked list.
 * 
 * @param list An element in a list.
 * @return A pointer to the last element in the list. Or NULL if provided an invalid list.
 */
ListItem *LinkedListGetLast(ListItem *list)
{
    if (list == NULL) {
        return NULL;
    } else {
        while (list->nextItem != NULL) {
            list = list->nextItem;
        }

    }
    return list;
}

/**
 * LinkedListSwapData() switches the data pointers of the two provided ListItems. This is most
 * useful when trying to reorder ListItems but when you want to preserve their location. This
 * function should return STANDARD_ERROR if either arguments are NULL, otherwise it should return
 * SUCCESS. If one or both of the data pointers are NULL in the given ListItems, it still does
 * perform the swap and returns SUCCESS.
 *
 * @param firstItem One of the items whose data will be swapped.
 * @param secondItem Another item whose data will be swapped.
 * @return SUCCESS if the swap worked or STANDARD_ERROR if it failed.
 */
int LinkedListSwapData(ListItem *firstItem, ListItem *secondItem)
{
    if (firstItem == NULL || secondItem == NULL) {
        return STANDARD_ERROR;
    }
    char* temp = firstItem->data;
    firstItem->data = secondItem->data;
    secondItem->data = temp;
    return SUCCESS;

}

/**
 * LinkedListPrint() prints out the complete list to stdout. This function prints out the given
 * list, starting at the head if the provided pointer is not the head of the list, like "[STRING1,
 * STRING2, ... ]" If LinkedListPrint() is called with a NULL list it does nothing, returning
 * STANDARD_ERROR. If passed a valid pointer, prints the list and returns SUCCESS.
 *
 * @param list Any element in the list to print.
 * @return SUCCESS or STANDARD_ERROR if passed NULL pointers.
 */
int LinkedListPrint(ListItem *list)
{
    if (list == NULL) {
        return STANDARD_ERROR;
    }

    while (list) {
        printf("%s ", list->data);
        list = list->nextItem;
    }
    return SUCCESS;
}
