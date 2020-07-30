//Vince Guan
//Will Phyo

#include "UNIXBOARD.h"
#include <stdint.h>
#include "Player.h"
#include <stdio.h>
#include <string.h>
#include "Game.h"
#define GAME_ROOM_NUM_LENGTH 2
#define GAME_ROOM_FOLDER_LENGTH 10
#define RPG_QUAVO 3
#define ALL8BITS 0xFF
//Datatypes

typedef struct RoomData {
    int Rnum; //roomnumber
    int Tlens; //title length
    int Dlens; //desc length
    char title[GAME_MAX_ROOM_TITLE_LENGTH + 1];
    char desc[GAME_MAX_ROOM_DESC_LENGTH + 1];
    int north;
    int south;
    int east;
    int west;
} RoomData;
static RoomData Room;
int profile, i, currentI, tempI, tempE;
int itemlen, descLen, titleLen, roomItemLen;
int check;
FILE *fileptr;

//help me function

void exitParse(int profile, int tempE, FILE *fileptr)
{
    //parses each direction with 0xff
    //norf
    profile = fread(&tempE, sizeof (char), 1, fileptr);
    if (profile != 1) FATAL_ERROR();
    Room.north = tempE & ALL8BITS;

    //east
    profile = fread(&tempE, sizeof (char), 1, fileptr);
    if (profile != 1) FATAL_ERROR();
    Room.east = tempE & ALL8BITS;

    //souf
    profile = fread(&tempE, sizeof (char), 1, fileptr);
    if (profile != 1) FATAL_ERROR();
    Room.south = tempE & ALL8BITS;
    //west
    profile = fread(&tempE, sizeof (char), 1, fileptr);
    if (profile != 1) FATAL_ERROR();
    Room.west = tempE & ALL8BITS;




}

void CreateRoom(int room)
{

    itemlen = descLen = titleLen = roomItemLen = check = 0;
    char fileNamaewa[GAME_ROOM_FOLDER_LENGTH + GAME_MAX_ROOM_TITLE_LENGTH + GAME_ROOM_NUM_LENGTH + 1];
    //grabs the text files
    sprintf(fileNamaewa, "./RoomFiles/room%d.txt", room);
    fileptr = fopen(fileNamaewa, "r");
    fseek(fileptr, RPG_QUAVO, SEEK_SET); //fseek puts pointer at end of RPG
    profile = fread(&titleLen, sizeof (char), 1, fileptr);
    if (profile == FALSE) FATAL_ERROR();
    //transfers title length over
    Room.Tlens = titleLen;

    profile = fread(&Room.title, sizeof (char), titleLen, fileptr);
    if (profile != titleLen) FATAL_ERROR();
    //sets the title length to null after use
    Room.title[titleLen] = 0;

    while (!check) {

        profile = fread(&itemlen, sizeof (char), 1, fileptr);
        if (profile == FALSE) FATAL_ERROR();
        check = 1;
        //item check
        while (i < itemlen) {
            //checks entire inventory for needed items
            profile = fread(&currentI, sizeof (char), 1, fileptr);
            if (profile == FALSE) FATAL_ERROR();
            if (FindInInventory(currentI) == STANDARD_ERROR) check = 0;
            printf("%i\n", check);
            i++; //increments

        }
        i = 0;
        profile = fread(&descLen, sizeof (char), 1, fileptr);
        if (profile == FALSE) FATAL_ERROR();
        Room.Dlens = descLen;

        //description
        profile = fread(&Room.desc, sizeof (char), descLen, fileptr);

        if (profile == FALSE) FATAL_ERROR();
        Room.desc[descLen] = 0;

        profile = fread(&roomItemLen, sizeof (char), 1, fileptr);
        if (profile == FALSE) FATAL_ERROR();
        //checks if the item is the length and then deleting it from the txt file
        while (i < roomItemLen) {
            profile = fread(&tempI, sizeof (char), 1, fileptr);
            if (profile == FALSE) FATAL_ERROR();
            if (check == TRUE) AddToInventory(tempI);
            i++;
        }

        i = 0;
        exitParse(profile, tempE, fileptr);

    }

    fclose(fileptr);
}

/**
 * These function transitions between rooms. Each call should return SUCCESS if the current room has
 * an exit in the correct direction and the new room was able to be loaded, and STANDARD_ERROR
 * otherwise.
 * @return SUCCESS if the room CAN be navigated to and changing the current room to that new room
 *         succeeded.
 */
int GameGoNorth(void)
{
    int x = STANDARD_ERROR;

    if (!Room.north) return x;
    int destination = Room.north;
    CreateRoom(destination);
    //checks if destination is the current number
    if (Room.Rnum == destination) x = SUCCESS;
    return x;
}

/**
 * @see GameGoNorth
 */
int GameGoEast(void)
{
    int x = STANDARD_ERROR;

    if (!Room.east) return x;
    int destination = Room.east;
    CreateRoom(destination);
    Room.Rnum = destination;
    //checks if destination is the current number
    if (Room.Rnum == destination) x = SUCCESS;
    return x;

}

/**
 * @see GameGoNorth
 */
int GameGoSouth(void)
{
    int x = STANDARD_ERROR;

    if (!Room.south) return x;
    int destination = Room.south;
    CreateRoom(destination);
    Room.Rnum = destination;
    //checks if destination is the current number
    if (Room.Rnum == destination) x = SUCCESS;
    return x;

}

/**
 * @see GameGoNorth
 */
int GameGoWest(void)
{
    int x = STANDARD_ERROR;

    if (!Room.west) return x; //checks if room is not west and returns error if not
    int destination = Room.west;
    CreateRoom(destination); //load the current destination
    Room.Rnum = destination;
    //checks if destination is the current number
    if (Room.Rnum == destination) x = SUCCESS;
    return x;

}

/**
 * This function sets up anything that needs to happen at the start of the game. This is just
 * setting the current room to STARTING_ROOM and loading it. It should return SUCCESS if it succeeds
 * and STANDARD_ERROR if it doesn't.
 * @return SUCCESS or STANDARD_ERROR
 */
int GameInit(void)
{

    //starts the functions to create rooms
    CreateRoom(STARTING_ROOM);

    Room.Rnum = STARTING_ROOM;

    if (Room.Rnum == STARTING_ROOM) return SUCCESS;
    return STANDARD_ERROR;

}

/**
 * Copies the current room title as a NULL-terminated string into the provided character array.
 * Only a NULL-character is copied if there was an error so that the resultant output string
 * length is 0.
 * @param title A character array to copy the room title into. Should be GAME_MAX_ROOM_TITLE_LENGTH+1
 *             in length in order to allow for all possible titles to be copied into it.
 * @return The length of the string stored into `title`. Note that the actual number of chars
 *         written into `title` will be this value + 1 to account for the NULL terminating
 *         character.
 */
int GameGetCurrentRoomTitle(char *title)
{
    //displays title
    strcpy(title, Room.title);
    return Room.Tlens;

}

/**
 * GetCurrentRoomDescription() copies the description of the current room into the argument desc as
 * a C-style string with a NULL-terminating character. The room description is guaranteed to be less
 * -than-or-equal to GAME_MAX_ROOM_DESC_LENGTH characters, so the provided argument must be at least
 * GAME_MAX_ROOM_DESC_LENGTH + 1 characters long. Only a NULL-character is copied if there was an
 * error so that the resultant output string length is 0.
 * @param desc A character array to copy the room description into.
 * @return The length of the string stored into `desc`. Note that the actual number of chars
 *          written into `desc` will be this value + 1 to account for the NULL terminating
 *          character.
 */
int GameGetCurrentRoomDescription(char *desc)
{
    //displays the discription
    strcpy(desc, Room.desc);

    return Room.Dlens;

}

/**
 * This function returns the exits from the current room in the lowest-four bits of the returned
 * uint8 in the order of NORTH, EAST, SOUTH, and WEST such that NORTH is in the MSB and WEST is in
 * the LSB. A bit value of 1 corresponds to there being a valid exit in that direction and a bit
 * value of 0 corresponds to there being no exit in that direction. The GameRoomExitFlags enum
 * provides bit-flags for checking the return value.
 *
 * @see GameRoomExitFlags
 *
 * @return a 4-bit bitfield signifying which exits are available to this room.
 */
uint8_t GameGetCurrentRoomExits(void)
{
    uint8_t temp = 0;
    //check where the rooms exits are
    if (Room.north) temp |= GAME_ROOM_EXIT_NORTH_EXISTS;
    if (Room.south) temp |= GAME_ROOM_EXIT_SOUTH_EXISTS;
    if (Room.east) temp |= GAME_ROOM_EXIT_EAST_EXISTS;
    if (Room.west) temp |= GAME_ROOM_EXIT_WEST_EXISTS;
    return temp;
}
