//Vince Guan
//Will Phyo

#include "UNIXBOARD.h"
#include <stdint.h>
#include "Player.h"
#include <stdio.h>
#include <string.h>

// Define how big the player's inventory is.
#define MAX 255
static int CURRENT_SIZE = 0;

static char inventory[INVENTORY_SIZE + 1];

/**
 * Adds the specified item to the player's inventory if the inventory isn't full.
 * @param item The item number to be stored: valid values are 0-255.
 * @return SUCCESS if the item was added, STANDARD_ERRROR if the item couldn't be added.
 */
int AddToInventory(uint8_t item)
{
    int i = 0;
    if (item > MAX || item < 0) return STANDARD_ERROR;

    while (i < INVENTORY_SIZE) {
        printf("%d is in the inventory\n", inventory[i]);
        if (i == CURRENT_SIZE) break;
        ++i;
    }
    inventory[i] = item;
    CURRENT_SIZE++;
    return SUCCESS;



}

/**
 * Check if the given item exists in the player's inventory.
 * @param item The number of the item to be searched for: valid values are 0-255.
 * @return SUCCESS if it was found or STANDARD_ERROR if it wasn't.
 */
int FindInInventory(uint8_t item)
{
    //	int i = 0;
    if (item > MAX || item < 0) return STANDARD_ERROR;
    for (unsigned int i = 0; i < INVENTORY_SIZE; i++) {

        if (inventory[i] == item) {
            return SUCCESS;
        }
    }
    return STANDARD_ERROR;
}
