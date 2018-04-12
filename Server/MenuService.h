#ifndef MENU_SERVICE_H
#define MENU_SERVICE_H

#include "RoomService.h"
#include "AccountService.h"

int connectAccountToRoomService(Room* rooms, int numberofrooms, char* ip);
int setReadyToStartService();
void voteForHostService(Room* room, char* login);

#endif // MENU_SERVICE_H
