#ifndef MENU_SERVICE_H
#define MENU_SERVICE_H

#include "RoomService.h"
#include "AccountService.h"
#include "Defines.h"


int connectAccountToRoomService(AccountData* account);
int setReadyToStartService(uint32_t ip, unsigned short roomid);
int voteForHostService(char* login, unsigned short roomid); //fully done, but i'm not sure about this one, votecounter in Room struct seems odd

#endif // MENU_SERVICE_H
