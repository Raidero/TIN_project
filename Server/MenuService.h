#ifndef MENU_SERVICE_H
#define MENU_SERVICE_H

#include "RoomService.h"
#include "AccountService.h"

#define PLAYER_NOT_FOUND 1

int connectAccountToRoomService(Room* rooms, int numberofrooms, char* ip);
int setReadyToStartService();
int voteForHostService(Room* room, char* login);

#endif // MENU_SERVICE_H
