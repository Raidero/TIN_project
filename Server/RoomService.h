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
    bool isplayerready[MAX_PLAYER_COUNT]; //so that players can say "i'm ready to start playing game"

    bool isingame;
} Room;



extern Room* rooms[MAX_ROOM_COUNT];

Room* initRoom();
void disposeRoom(int roomid);
void checkIfRoomIsEmptyAndDispose(int roomid);
int findFreeRoomForAccount(AccountData* playerdata);
int connectAccountToRoom(Room* room, AccountData* playerdata, int index);
int createRoomForAccount(AccountData* playerdata);
char* refreshRoomService(AccountData* playerdata, int roomid);

int sweepPlayer(AccountData* playerdata, int roomid);	// function looks for a player and removes it from a room
//create function that check if all the players are still in the room
#endif // ROOM_SERVICE_H
