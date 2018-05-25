#ifndef MENU_SERVICE_H
#define MENU_SERVICE_H
#include <stdio.h>
#include <string.h>

#include "RoomService.h"
#include "AccountService.h"
#include "Defines.h"


int connectAccountToRoomService(int accountid, bool* ishost);
int setReadyToStartService(uint32_t ip, int roomid);
int voteForHostService(char* login, int roomid); //fully done, but i'm not sure about this one, votecounter in Room struct seems odd

#endif // MENU_SERVICE_H
