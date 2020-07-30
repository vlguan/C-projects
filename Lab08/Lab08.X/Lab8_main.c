//Vince Guan
//Will Phyo

// **** Include libraries here ****
// Standard libraries
#include "UNIXBOARD.h"
#include <stdint.h>
#include "Player.h"
#include <stdio.h>
#include <string.h>
#include "Game.h"
//CSE013E Support Library




// User libraries


// **** Set any macros or preprocessor directives here ****

// **** Declare any data types here ****

// **** Define any global or external variables here ****

// **** Declare any function prototypes here ****

void exitschar()
{
    //prints out the directions avaliable to exit from
    int leave = GameGetCurrentRoomExits();
    printf("Exits:");
    if (leave & GAME_ROOM_EXIT_NORTH_EXISTS) printf(" north");
    if (leave & GAME_ROOM_EXIT_SOUTH_EXISTS) printf(" south");
    if (leave & GAME_ROOM_EXIT_EAST_EXISTS) printf(" east");
    if (leave & GAME_ROOM_EXIT_WEST_EXISTS) printf(" west");
    printf("\n");
}

int main()
{
    printf("Hewoo would uwu wike to pway some dnd by vlguan\n");


    /******************************** Your custom code goes below here ********************************/
    // gets title length
    char title[GAME_MAX_ROOM_TITLE_LENGTH + 1];
    // gets room desc length
    char desc[GAME_MAX_ROOM_DESC_LENGTH + 1];
    char enterhere;
    if (GameInit() == STANDARD_ERROR) FATAL_ERROR();

    //returns the roomtitle
    GameGetCurrentRoomTitle(title);
    //returns the description
    GameGetCurrentRoomDescription(desc);

    //displays the title and descriptions
    printf("\n%s\n%s\n\n", title, desc);
    exitschar();
    printf("Choose a direction: n s e w q(north south east west quit)\n");
    enterhere = getchar();
    getchar();
    //checks if the correct exits are pushed when leaving the room
    while (enterhere != 'q') {
        if (enterhere == 'n') {
            if (GameGoNorth() == STANDARD_ERROR) printf("Oops wrong way!\n");
        } else if (enterhere == 's') {
            if (GameGoSouth() == STANDARD_ERROR) printf("Oops wrong way!\n");
        } else if (enterhere == 'e') {
            if (GameGoEast() == STANDARD_ERROR) printf("Oops wrong way!\n");
        } else if (enterhere == 'w') {
            if (GameGoWest() == STANDARD_ERROR) printf("Oops wrong way!\n");
        } else if (enterhere == 'q') {
            break;
        }
        GameGetCurrentRoomTitle(title);
        GameGetCurrentRoomDescription(desc);
        printf("\n%s\n%s\n\n", title, desc);
        exitschar();
        printf("Choose a direction: n s e w q(north south east west quit)\n");
        enterhere = getchar();
        getchar();
    }
    exit(EXIT_SUCCESS);



    /**************************************************************************************************/
}

