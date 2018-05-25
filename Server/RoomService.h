#ifndef ROOM_SERVICE_H
#define ROOM_SERVICE_H
#include <stdio.h>
#include <string.h>
#include "AccountService.h"
#include "Defines.h"

typedef struct
{
    //short capacity;
    //short currentsize; //why? because we can just check if players[i] is null, or if not, that will tell us how many player exists, < MAX_PLAYER_COUNT
    AccountData* players[MAX_PLAYER_COUNT];
    char isplayerready[MAX_PLAYER_COUNT]; //so that players can say "i'm ready to start playing game"
    uint8_t hostindex;

    bool isingame;
} Room;



extern Room* rooms[MAX_ROOM_COUNT];

void initRoomService();
Room* initRoom();
void disposeRoom(int roomid);
void disposeAllRooms();
void checkIfRoomIsEmptyAndDispose(int roomid);
int findFreeRoomForAccount(int accountid);
int connectAccountToRoom(Room* room, int accountid, int index);
int createRoomForAccount(int accountid);
int refreshRoomService(int accountid, int roomid, char* loginlist);
char* refreshReadyUpService(int accountid, int roomid);
int toggleReadyService(int accountid, int roomid);
int exitRoomService(int accountid, int roomid);

int sweepPlayer(int accountid, int roomid);	// function looks for a player and removes it from a room
//create function that check if all the players are still in the room
#endif // ROOM_SERVICE_H
